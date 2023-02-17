// NeL - MMORPG Framework <http://dev.ryzom.com/projects/nel/>
// Copyright (C) 2010  Winch Gate Property Limited
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

#ifndef NL_INET_ADDRESS_H
#define NL_INET_ADDRESS_H

#include "nel/misc/types_nl.h"

#include "nel/net/ipv6_address.h"

#include <string>
#include <vector>


struct sockaddr_in;
struct sockaddr_in6;
struct in_addr;
struct in6_addr;


#ifdef NL_OS_WINDOWS
// automatically add the win socket library if you use nel network part
#pragma comment(lib, "ws2_32.lib")

// it seems that the default loop back address is not defined for ipv6
#ifndef IN6ADDR_LOOPBACK_INIT
#define IN6ADDR_LOOPBACK_INIT { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }
#endif

#endif

namespace NLMISC
{
	class IStream;
}


namespace NLNET
{

struct ESocket;


/**
 * Internet address (IP + port).
 * The structure sockaddr_in is internally in network byte order
 * \author Olivier Cado
 * \author Nevrax France
 * \date 2000
 */
class CInetAddress
{
public:
	typedef struct sockaddr_in TSockAddrIn;
	typedef struct sockaddr_in6 TSockAddrIn6;
	typedef struct sockaddr_storage TSockAddrStorage;

	/// Default Constructor. The address is set to INADDR_ANY
	CInetAddress();
	CInetAddress(bool any);

	/// Alternate constructor (calls setByName())
	CInetAddress( const std::string& hostName, uint16 port );

	/// Alternate constructor (calls setByName())
	/// example: CInetAddress("www.nevrax.com:80")
	CInetAddress( const std::string& hostNameAndPort );

	/// Copy constructor
	CInetAddress( const CInetAddress& other );

	/// Assignment operator
	CInetAddress& operator=( const CInetAddress& other );

	/// Comparison == operator
	friend bool operator==( const CInetAddress& a1, const CInetAddress& a2 );

	/// Comparison < operator
	friend bool operator<( const CInetAddress& a1, const CInetAddress& a2 );

	/// Destructor
	~CInetAddress();

	/// Resolves a name
	CInetAddress&		setByName( const std::string& hostname );

	/// Sets port
	void				setPort( uint16 port );

	/// Sets hostname and port (ex: www.nevrax.com:80)
	void				setNameAndPort( const std::string& hostNameAndPort );

	/** Sets internal IPv4 socket address directly (contents is copied).
	 * It also retrieves the host name if CInetAddress::RetrieveNames is true.
	 */
	void				setSockAddr( const sockaddr_in* saddr );

	/** Sets internal IPv6 socket address directly (contents is copied).
	 * It also retrieves the host name if CInetAddress::RetrieveNames is true.
	 */
	void				setSockAddr6( const sockaddr_in6* saddr6 );

	/// Returns if object (address and port) is valid
	bool				isValid() const;

	// Convert an IPv4 or the Any listener address and port to the sockaddr_in structure
	bool toSockAddrInet(TSockAddrIn *addr) const;

	// Convert an IPv6 address and port to the sockaddr_in6 structure
	bool toSockAddrInet6(TSockAddrIn6 *addr) const;

	/// Returns internal IP address
	inline const CIPv6Address &getAddress() const { return m_Address; }

private:
	/// Returns internal IP address (DEPRECATED)
	uint32				internalIPAddress() const;

public:
	/// Returns the internal network address (it s the network address for example 192.168.0.0 for a C class) (DEPRECATED) // FIXME: IPv6
	uint32				internalNetAddress () const;

	/// Returns readable IP address. (ex: "195.68.21.195")
	std::string			ipAddress() const;

	/// Returns readable IP address. (ex: "195.68.21.195") (previously host name) (DEPRECATED)
	std::string			hostName() const;

	/// Returns port
	uint16				port() const;

	/// Returns hostname and port as a string. (ex: "www.nevrax.org:80 (195.68.21.195)")
	std::string			asString() const;

	/// Returns IP address and port as a string. (ex: "195.68.21.195:80")
	std::string			asIPString() const;

	/// Serialize
	void serial( NLMISC::IStream& s );

	/// Returns true if this CInetAddress is 127.0.0.1
	bool is127001 () const;

	/// Returns true if this CInetAddress is a loop back address
	bool isLoopbackIPAddress () const;

	/// Creates a CInetAddress object with local host address, port=0
	static CInetAddress	localHost();

	/** Returns the list of the local host addresses (with port=0)
	 * (especially useful if the host is multihomed)
	 */
	static std::vector<CInetAddress> localAddresses();

	uint32 hash32() const;
	uint64 hash64() const;
	inline size_t hash() const
	{
		if (sizeof(size_t) == sizeof(uint64)) return hash64();
		else return hash32();
	}

protected:
	/// Constructor with IPv4 address, port=0
	CInetAddress( const in_addr *ip, const char *hostname = 0);

	/// Constructor with IPv6 address, port=0
	CInetAddress( const in6_addr *ip, const char *hostname = 0);

private:
	// Called in all constructors. Calls CBaseSocket::init().
	void				init();
	
	CIPv6Address m_Address;
	uint16 m_Port;

};

/// Take a internet dot string and convert it in an uint32 internal format for example "128.64.32.16" -> 0xF0804020
uint32 stringToInternalIPAddress (const std::string &addr);

/// Take an internal address and convert it to a internet dot string
std::string internalIPAddressToString (uint32 addr);

std::string vectorCInetAddressToString(const std::vector<CInetAddress> &addrs);

}

#endif // NL_INET_ADDRESS_H

/* End of inet_address.h */
