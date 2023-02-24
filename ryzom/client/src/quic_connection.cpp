// Ryzom - MMORPG Framework <http://dev.ryzom.com/projects/ryzom/>
// Copyright (C) 2023  Jan BOON (Kaetemi) <jan.boon@kaetemi.be>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "stdpch.h"
#include "quic_connection.h"

#include "nel/misc/mutex.h"
#include "nel/misc/atomic.h"
#include "nel/misc/buf_fifo.h"
#include "nel/misc/string_view.h"
#include "nel/misc/string_common.h"

#include "config.h"

#ifdef NL_MSQUIC_AVAILABLE
#include <msquic.h>

#ifdef NL_OS_WINDOWS
#pragma warning(push)
#pragma warning(disable : 6553) // Annotation does not apply to value type.
#include <wincrypt.h>
#pragma warning(pop)
#endif

#define MsQuic m->Api

using namespace NLMISC;
using namespace NLNET;

class CQuicConnectionImpl
{
public:
	// The desired state change
	enum TDesiredStateChange
	{
		NoChange,
		Connect,
		Disconnect,
		Release,
	};

	CQuicConnectionImpl(CQuicConnection *self)
	    : Self(self)
	    , Api(NULL)
	    , Registration(NULL)
	    , Configuration(NULL)
	    , Connection(NULL)
	    , DesiredStateChange(NoChange)
	    , BufferReads(0)
	    , State(CQuicConnection::Disconnected)
	    , ReceiveEnable(0)
	{
	}

	CQuicConnection *Self;
	const QUIC_API_TABLE *Api;
	HQUIC Registration;
	HQUIC Configuration;
	HQUIC Connection;

	// This value is set by connect and disconnect,
	// and checked from the update call. (main thread only)
	TDesiredStateChange DesiredStateChange;
	CInetHost DesiredAddr;

	// This is the FIFO buffer for all datagrams that are received.
	// It is filled by the receive callback (any thread), and emptied by the receiveDatagram call (main thread only).
	CSynchronizedFIFO Buffer; // (any thread)
	CAtomicInt BufferWrites; // (any thread)
	int BufferReads; // If BufferReads == BufferWrites, then there's no more datagrams (main thread only)

	// Current state of the connection
	CQuicConnection::TState State; // Updated by the update call (main thread only)
	CAtomicFlag ConnectedFlag; // Set by the callback thread, checked by update (main)
	CAtomicFlag ShuttingDownFlag; // Set by the callback thread, checked by the update call (main thread), reset by update on complete disconnection
	CAtomicFlag ShutdownFlag; // Set by the callback thread, checked by the update call (main thread), reset by update on complete disconnection

	bool ReceiveEnable; // Set when datagrams may be received, otherwise they'll be ignored, this is to avoid stray messages after disconnecting (set and accessed by main thread only)
	CAtomicInt MaxSendLength; // Set by the callback thread, used to check if we can send

	static QUIC_STATUS
#ifdef _Function_class_
	    _Function_class_(QUIC_CONNECTION_CALLBACK)
#endif
	        connectionCallback(HQUIC connection, void *context, QUIC_CONNECTION_EVENT *ev);

	void init(); // Delayed init
	void update();
	void connect();
	void shutdown();
	void close();
	void release();

	/// Received datagram (from quic threads)
	void datagramReceived(const uint8 *buffer, uint32 length);
};

CQuicConnection::CQuicConnection()
#ifdef NL_CPP14
    : m(std::make_unique<CQuicConnectionImpl>(this))
#else
    : m(new CQuicConnectionImpl(this))
#endif
{
}

CQuicConnection::~CQuicConnection()
{
	disconnect(true);
	release();
}

void CQuicConnectionImpl::init()
{
	CQuicConnectionImpl *const m = this;

	if (MsQuic)
	{
		return;
	}

	// TestAndSet has acquire semantics, clear has release semantics, so we use clear to flag the events
	m->ConnectedFlag.testAndSet();
	m->ShuttingDownFlag.testAndSet();
	m->ShutdownFlag.testAndSet();

	// Open library
	QUIC_STATUS status = MsQuicOpenVersion(QUIC_API_VERSION_2, (const void **)&MsQuic);
	if (QUIC_FAILED(status))
	{
		nlwarning("MsQuicOpenVersion failed with status 0x%x", status);
		return;
	}

	// Registration, this creates the worker threads
	QUIC_REGISTRATION_CONFIG regConfig = { 0 };
	regConfig.AppName = "Ryzom Core (Client)";
	regConfig.ExecutionProfile = QUIC_EXECUTION_PROFILE_LOW_LATENCY;
	status = MsQuic->RegistrationOpen(&regConfig, &m->Registration);
	if (QUIC_FAILED(status))
	{
		nlwarning("MsQuic->RegistrationOpen failed with status 0x%x", status);
		release();
		return;
	}
}

void CQuicConnection::connect(const NLNET::CInetHost &addr)
{
	m->DesiredStateChange = CQuicConnectionImpl::Connect;
	m->DesiredAddr = addr;
	update();
}

void CQuicConnectionImpl::connect()
{
	CQuicConnectionImpl *const m = this;

	init();

	nlassert(m->State == CQuicConnection::Disconnected);
	nlassert(m->DesiredStateChange == Connect);

	NLNET::CInetHost addr = m->DesiredAddr;

	// Reset state
	m->DesiredStateChange = NoChange;
	m->ConnectedFlag.testAndSet();
	m->ShuttingDownFlag.testAndSet();
	m->ShutdownFlag.testAndSet();
	m->MaxSendLength = 0;
	m->BufferReads = m->BufferWrites;
	m->ReceiveEnable = true;

	// Clear FIFO
	{
		CFifoAccessor access(&m->Buffer);
		CBufFIFO *fifo = &access.value();
		fifo->clear();
	}

	if (!MsQuic)
	{
		nlwarning("QUIC API not available");
		return;
	}

	if (!addr.isValid())
	{
		return;
	}

	static const char *protocolName = "ryzomcore4";
	static const QUIC_BUFFER alpn = { sizeof(protocolName) - 1, (uint8_t *)protocolName };

	// Configuration, initialized in start, but destroyed on release only (may attempt more than once)
	QUIC_STATUS status = QUIC_STATUS_SUCCESS;
	if (!m->Configuration)
	{
		QUIC_SETTINGS settings = { 0 };
		settings.DatagramReceiveEnabled = TRUE;
		settings.IsSet.DatagramReceiveEnabled = TRUE;
		settings.MigrationEnabled = TRUE;
		settings.IsSet.MigrationEnabled = TRUE;
		settings.PeerBidiStreamCount = 0;
		settings.IsSet.PeerBidiStreamCount = TRUE;
		settings.PeerUnidiStreamCount = 0; // TODO: Configured from msg.xml
		settings.IsSet.PeerUnidiStreamCount = TRUE;
		// settings.SendBufferingEnabled = TRUE;
		// settings.IsSet.SendBufferingEnabled = TRUE;
		// settings.GreaseQuicBitEnabled = TRUE;
		// settings.IsSet.GreaseQuicBitEnabled = TRUE;
		// settings.MinimumMtu = m_MsgSize + size of QUIC header; // Probably violates QUIC protocol if we do this, also no need
		// settings.IsSet.MinimumMtu = TRUE;
		status = MsQuic->ConfigurationOpen(m->Registration, &alpn, 1, &settings, sizeof(settings), NULL, &m->Configuration);
		if (QUIC_FAILED(status))
		{
			close();
			nlwarning("MsQuic->ConfigurationOpen failed with status 0x%x", status);
			return;
		}

		// Load credentials for client, client doesn't need a certificate
		QUIC_CREDENTIAL_CONFIG credConfig;
		memset(&credConfig, 0, sizeof(credConfig));
		credConfig.Flags = QUIC_CREDENTIAL_FLAG_CLIENT;
		credConfig.Type = QUIC_CREDENTIAL_TYPE_NONE;
		status = MsQuic->ConfigurationLoadCredential(m->Configuration, &credConfig);
		if (QUIC_FAILED(status))
		{
			close();
			MsQuic->ConfigurationClose(m->Configuration);
			m->Configuration = nullptr;
			nlwarning("MsQuic->ConfigurationLoadCredential failed with status 0x%x", status);
			return;
		}
	}

	// Connect to the specified address
	status = MsQuic->ConnectionOpen(m->Registration, CQuicConnectionImpl::connectionCallback, (void *)this, &m->Connection);
	if (QUIC_FAILED(status))
	{
		close();
		nlwarning("MsQuic->ConnectionOpen failed with status 0x%x", status);
		return;
	}

	// Start the connection (NOTE: The hostname lookup is a bit redundant here, but it's okay I suppose...)
	status = MsQuic->ConnectionStart(m->Connection, m->Configuration, QUIC_ADDRESS_FAMILY_UNSPEC, nlUtf8ToMbcs(addr.hostname()), addr.port());
	if (QUIC_FAILED(status))
	{
		close();
		nlwarning("MsQuic->ConnectionStart to %s failed with status 0x%x", addr.toStringLong().c_str(), status);
		return;
	}

	// Check
	m->State = CQuicConnection::Connecting;
	update();
}

void CQuicConnection::disconnect(bool blocking)
{
	m->DesiredStateChange = CQuicConnectionImpl::Disconnect;
	m->ReceiveEnable = false;
	if (MsQuic && blocking)
	{
		try
		{
			while (m->State != CQuicConnection::Disconnected)
			{
				nlSleep(1);
				update();
			}
			return;
		}
		catch (const std::exception &e)
		{
			m->close(); // Not very safe safety fallback
			nlwarning("Exception while waiting for connection shutdown: %s", e.what());
		}
	}
}

void CQuicConnectionImpl::shutdown()
{
	CQuicConnectionImpl *const m = this;
	if (m->Connection)
	{
		MsQuic->ConnectionShutdown(m->Connection, QUIC_CONNECTION_SHUTDOWN_FLAG_NONE, 0);
	}
	update();
}

void CQuicConnectionImpl::close()
{
	CQuicConnectionImpl *const m = this;
	if (m->Connection)
	{
		MsQuic->ConnectionClose(m->Connection);
		m->Connection = NULL;
	}
}

// All the state updates and switches are done here
void CQuicConnectionImpl::update()
{
	CQuicConnectionImpl *const m = this;

	// Asynchronous release
	if (!m->Connection
	    && m->DesiredStateChange == CQuicConnectionImpl::Release)
	{
		nlassert(m->State == CQuicConnection::Disconnected);
		m->release();
		m->DesiredStateChange = CQuicConnectionImpl::NoChange;
		update();
		return;
	}

	// Early exit
	if (m->State == CQuicConnection::Disconnected
	    && m->DesiredStateChange != CQuicConnectionImpl::Connect)
	{
		nlassert(!m->Connection);
		return;
	}

	// Simple case, initial state, user wants to connect and we're disconnected
	if (m->DesiredStateChange == Connect
	    && m->State == CQuicConnection::Disconnected)
	{
		nlassert(!m->Connection);
		connect();
		return;
	}

	// Also simple case
	if (m->DesiredStateChange != NoChange // Disconnect or Connect (reconnecting)
	    && m->State == CQuicConnection::Connected)
	{
		m->State = CQuicConnection::Disconnecting;
		shutdown();
		return;
	}

	// Connected!
	if (!m->ConnectedFlag.testAndSet())
	{
		if (m->State == CQuicConnection::Connecting)
		{
			m->State = CQuicConnection::Connected;
			// We're good!
		}
		else
		{
			nlwarning("Connection is connected, but state is %d", m->State);
		}
		update();
		return;
	}

	// Shutting down, this is just a limbo state
	if (!m->ShuttingDownFlag.testAndSet())
	{
		if (m->State == CQuicConnection::Connected
		    || m->State == CQuicConnection::Connecting)
		{
			m->State = CQuicConnection::Disconnecting;
		}
		update();
		return;
	}

	// Shutdown complete, now close!
	if (!m->ShutdownFlag.testAndSet())
	{
		nlassert(m->State != CQuicConnection::Disconnected);
		nlassert(m->Connection);
		close();
		m->State = CQuicConnection::Disconnected;
		update();
		return;
	}
}

void CQuicConnection::update()
{
	m->update();
}

void CQuicConnectionImpl::release()
{
	CQuicConnectionImpl *const m = this;

	// Clear FIFO
	{
		CFifoAccessor access(&m->Buffer);
		CBufFIFO *fifo = &access.value();
		fifo->clear();
	}

	// Close configuration
	if (m->Configuration)
	{
		MsQuic->ConfigurationClose(m->Configuration);
		m->Configuration = NULL;
	}

	// Close registration
	if (m->Registration)
	{
		MsQuic->RegistrationClose(m->Registration);
		m->Registration = NULL;
	}

	// Close library
	if (MsQuic)
	{
		MsQuicClose(MsQuic);
		MsQuic = NULL;
	}
}

void CQuicConnection::release()
{
	m->DesiredStateChange = CQuicConnectionImpl::Release;
	m->ReceiveEnable = false;
	update();
}

CQuicConnection::TState CQuicConnection::state() const
{
	return m->State;
}

uint32 CQuicConnection::maxSendLength() const
{
	return m->MaxSendLength;
}

QUIC_STATUS
#ifdef _Function_class_
_Function_class_(QUIC_CONNECTION_CALLBACK)
#endif
    CQuicConnectionImpl::connectionCallback(HQUIC connection, void *context, QUIC_CONNECTION_EVENT *ev)
{
	CQuicConnectionImpl *const m = (CQuicConnectionImpl *)context;
	QUIC_STATUS status = QUIC_STATUS_NOT_SUPPORTED;
	switch (ev->Type)
	{
	case QUIC_CONNECTION_EVENT_CONNECTED: {
		m->ConnectedFlag.clear();
		nlinfo("Connected");
		nlassert(CStringView((const char *)ev->CONNECTED.NegotiatedAlpn, ev->CONNECTED.NegotiatedAlpnLength) == "ryzomcore4");
		status = QUIC_STATUS_SUCCESS;
		break;
	}
	case QUIC_CONNECTION_EVENT_SHUTDOWN_INITIATED_BY_TRANSPORT: {
		m->ShuttingDownFlag.clear();
		m->MaxSendLength = 0;
		status = QUIC_STATUS_SUCCESS;
		break;
	}
	case QUIC_CONNECTION_EVENT_SHUTDOWN_INITIATED_BY_PEER: {
		m->ShuttingDownFlag.clear();
		m->MaxSendLength = 0;
		status = QUIC_STATUS_SUCCESS;
		break;
	}
	case QUIC_CONNECTION_EVENT_SHUTDOWN_COMPLETE: {
		m->ShutdownFlag.clear();
		m->MaxSendLength = 0;
		status = QUIC_STATUS_SUCCESS;
		break;
	}
	case QUIC_CONNECTION_EVENT_DATAGRAM_RECEIVED:
		nlinfo("Datagram received");
		// YES PLEASE
		m->datagramReceived(ev->DATAGRAM_RECEIVED.Buffer->Buffer, ev->DATAGRAM_RECEIVED.Buffer->Length);
		status = QUIC_STATUS_SUCCESS;
		break;
	case QUIC_CONNECTION_EVENT_DATAGRAM_STATE_CHANGED:
		nlinfo("Datagram state changed");
		m->MaxSendLength = ev->DATAGRAM_STATE_CHANGED.SendEnabled ? ev->DATAGRAM_STATE_CHANGED.MaxSendLength : 0;
		status = QUIC_STATUS_SUCCESS;
		break;
	case QUIC_CONNECTION_EVENT_LOCAL_ADDRESS_CHANGED:
	case QUIC_CONNECTION_EVENT_PEER_ADDRESS_CHANGED:
	case QUIC_CONNECTION_EVENT_IDEAL_PROCESSOR_CHANGED:
	case QUIC_CONNECTION_EVENT_DATAGRAM_SEND_STATE_CHANGED:
	case QUIC_CONNECTION_EVENT_RESUMED:
	case QUIC_CONNECTION_EVENT_PEER_CERTIFICATE_RECEIVED:
	case QUIC_CONNECTION_EVENT_STREAMS_AVAILABLE: // TODO: Match with msg.xml
		// Don't care
		status = QUIC_STATUS_SUCCESS;
		break;
	case QUIC_CONNECTION_EVENT_PEER_STREAM_STARTED:
	case QUIC_CONNECTION_EVENT_PEER_NEEDS_STREAMS:
	case QUIC_CONNECTION_EVENT_RESUMPTION_TICKET_RECEIVED:
		// Not supported
		break;
	}
	return status;
}

bool CQuicConnection::sendDatagram(const uint8 *buffer, uint32 size)
{
	if (m->Connection && size <= m->MaxSendLength)
	{
		QUIC_BUFFER buf;
		buf.Buffer = (uint8 *)buffer;
		buf.Length = size;
		QUIC_STATUS status = MsQuic->DatagramSend(m->Connection, &buf, 1, QUIC_SEND_FLAG_NONE, this);
		if (QUIC_FAILED(status))
		{
			nlwarning("DatagramSend failed with %d", status);
			return false;
		}
		return true;
	}
	return false;
}

bool CQuicConnection::datagramAvailable() const
{
	// CFifoAccessor access(&m->Buffer);
	// return !access.value().empty();
	return m->ReceiveEnable && m->BufferWrites != m->BufferReads;
}

bool CQuicConnection::receiveDatagram(NLMISC::CBitMemStream &msgin)
{
	if (!m->ReceiveEnable)
		return false;

	int writes = m->BufferWrites;
	int reads = m->BufferReads;
	if (writes != reads)
	{
		CFifoAccessor access(&m->Buffer); // This block is mutex'd now
		CBufFIFO *fifo = &access.value();
		if (fifo->empty())
		{
			m->BufferReads = writes; // Update value (not a real counter)
			return false;
		}
		uint8 *buffer;
		uint32 size;
		fifo->front(buffer, size);
		msgin.clear();
		memcpy(msgin.bufferToFill(size), buffer, size);
		fifo->pop();
		if (fifo->empty())
		{
			m->BufferReads = writes; // Update value (not a real counter)
		}
		return true;
	}
	return false;
}

void CQuicConnectionImpl::datagramReceived(const uint8 *buffer, uint32 length)
{
	CQuicConnectionImpl *const m = this;
	CFifoAccessor access(&m->Buffer); // This block is mutex'd now
	CBufFIFO *fifo = &access.value();
	fifo->push(buffer, length);
	++m->BufferWrites;
}

#else

using namespace NLMISC;
using namespace NLNET;

class CQuicConnectionImpl
{
public:
	int Dummy;
};

CQuicConnection::CQuicConnection()
#ifdef NL_CPP14
    : m(std::make_unique<CQuicConnectionImpl>())
#else
    : m(new CQuicConnectionImpl())
#endif
{
}

CQuicConnection::~CQuicConnection()
{
}

void CQuicConnection::connect(const NLNET::CInetHost &addr)
{
}

void CQuicConnection::disconnect(bool blocking)
{
}

void CQuicConnection::release()
{
}

CQuicConnection::TState CQuicConnection::state() const
{
	return Disconnected;
}

void CQuicConnection::sendDatagram(const uint8 *buffer, uint32 size)
{
}

bool CQuicConnection::datagramAvailable()
{
	return false;
}

bool CQuicConnection::receiveDatagram(NLMISC::CBitMemStream &msgin)
{
	return false;
}

#endif

/* end of file */
