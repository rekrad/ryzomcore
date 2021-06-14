// Ryzom - MMORPG Framework <http://dev.ryzom.com/projects/ryzom/>
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

#include "stdpch.h"
//#include "spawn_zones_back_compat.h"

// game_share
#include "game_share/utils.h"
// egs
#include "zone_manager.h"


using namespace NLMISC;
using namespace NLNET;
using namespace std;


namespace BACK_COMPAT
{

// !!! DO NOT EDIT THIS TABLE !!!
static const string OldSpawnZones[] =
{
	"spawn_global_fyros_matis",
	"spawn_global_tryker_matis",
	"spawn_global_bagne_matis",
	"spawn_kami_place_yrkanis",
	"spawn_karavan_place_yrkanis",
	"spawn_karavan_place_natae",
	"spawn_karavan_place_davae",
	"spawn_karavan_place_avalae",
	"spawn_kami_groveofconfusion",
	"spawn_karavan_groveofconfusion",
	"spawn_kami_fleetinggarden",
	"spawn_karavan_fleetinggarden",
	"spawn_karavan_upperbog",
	"spawn_kami_region_hiddensource",
	"spawn_karavan_region_hereticshovel",
	"spawn_kami_region_hereticshovel",
	"spawn_karavan_region_hiddensource",
	"spawn_kami_knollofdissent",
	"spawn_karavan_knollofdissent",
	"spawn_outpost_matis_01",
	"spawn_outpost_matis_02",
	"spawn_outpost_matis_03",
	"spawn_outpost_matis_04",
	"spawn_outpost_matis_05",
	"spawn_outpost_matis_06",
	"spawn_outpost_matis_07",
	"spawn_outpost_matis_08",
	"spawn_outpost_matis_09",
	"spawn_outpost_matis_10",
	"spawn_outpost_matis_11",
	"spawn_outpost_matis_12",
	"spawn_outpost_matis_13",
	"spawn_outpost_matis_14",
	"spawn_outpost_matis_15",
	"spawn_outpost_matis_16",
	"spawn_outpost_matis_17",
	"spawn_outpost_matis_18",
	"spawn_outpost_matis_19",
	"spawn_outpost_matis_20",
	"spawn_outpost_matis_21",
	"spawn_outpost_matis_22",
	"spawn_outpost_matis_23",
	"spawn_outpost_matis_24",
	"spawn_outpost_matis_25",
	"spawn_outpost_matis_26",
	"spawn_outpost_matis_27",
	"spawn_outpost_matis_28",
	"spawn_outpost_matis_29",
	"spawn_outpost_matis_30",
	"spawn_outpost_matis_31",
	"spawn_outpost_matis_32",
	"spawn_local_ava_bar_int2_bar_ext1",
	"spawn_local_ava_bar_ext_bar_int2",
	"spawn_local_dav_bar_int2_bar_ext1",
	"spawn_local_dav_bar_ext_bar_int2",
	"spawn_local_nat_bar_ext2",
	"spawn_local_nat_bar_ext_bar_int2",
	"spawn_local_yrk_serre_npc2_serre2",
	"spawn_local_yrk_serre_npc1_serre1",
	"spawn_local_yrk_serre2_serre_npc2",
	"spawn_local_yrk_serre1_serre_npc1",
	"spawn_local_yrk_bar_int_ext1",
	"spawn_local_yrk_bar_int1",
	"spawn_local_yrk_mairie_ss_rdc",
	"spawn_local_yrk_mairie_rdc_ss",
	"spawn_newbie_place_stalli",
	"start_newb_welcomer_stalli_1",
	"start_newb_welcomer_stalli_2",
	"start_newb_welcomer_stalli_3",
	"start_newb_welcomer_stalli_4",
	"start_newb_welcomer_stalli_5",
	"spawn_newbie_place_borea",
	"start_newb_welcomer_borea_1",
	"start_newb_welcomer_borea_2",
	"start_newb_welcomer_borea_3",
	"start_newb_welcomer_borea_4",
	"start_newb_welcomer_borea_5",
	"spawn_newbie_place_nistia",
	"start_newb_welcomer_nistia_1",
	"start_newb_welcomer_nistia_2",
	"start_newb_welcomer_nistia_3",
	"start_newb_welcomer_nistia_4",
	"start_newb_welcomer_nistia_5",
	"spawn_newbie_place_rosilio",
	"start_newb_welcomer_rosilio_1",
	"start_newb_welcomer_rosilio_2",
	"start_newb_welcomer_rosilio_3",
	"start_newb_welcomer_rosilio_4",
	"start_newb_welcomer_rosilio_5",
	"spawn_newbie_place_miani",
	"start_newb_welcomer_miani_1",
	"start_newb_welcomer_miani_2",
	"start_newb_welcomer_miani_3",
	"start_newb_welcomer_miani_4",
	"start_newb_welcomer_miani_5",
	"spawn_MA_Yrk_exit_secours",
	"spawn_inst_MA_pjroom",
	"spawn_inst_MA_guildhall",
	"spawn_inst_MA_guildhall_fight",
	"spawn_inst_MA_guildhall_magic",
	"spawn_inst_MA_guildhall_harvest",
	"spawn_inst_MA_guildhall_craft",
	"spawn_inst_MA_council",
	"spawn_inst_MA_trainer",
	"spawn_MA_Yrk_007",
	"spawn_MA_Yrk_008",
	"spawn_MA_Yrk_009",
	"spawn_MA_Yrk_010",
	"spawn_MA_Yrk_011",
	"spawn_MA_Yrk_012",
	"spawn_MA_Yrk_013",
	"spawn_MA_Yrk_014",
	"spawn_MA_Yrk_015",
	"spawn_MA_Yrk_016",
	"spawn_MA_Yrk_017",
	"spawn_MA_Yrk_018",
	"spawn_stalli_68",
	"spawn_stalli_69",
	"spawn_stalli_70",
	"spawn_rosilio_71",
	"spawn_rosilio_72",
	"spawn_rosilio_73",
	"spawn_borea_74",
	"spawn_borea_75",
	"spawn_borea_76",
	"spawn_miani_77",
	"spawn_nistia_80",
	"spawn_natae_imm1_029",
	"spawn_natae_imm1_030",
	"spawn_natae_imm1_031",
	"spawn_natae_imm2_032",
	"spawn_natae_imm2_033",
	"spawn_natae_imm2_034",
	"spawn_davae_imm1_041",
	"spawn_davae_imm1_042",
	"spawn_davae_imm1_043",
	"spawn_davae_imm2_044",
	"spawn_davae_imm2_045",
	"spawn_davae_imm2_046",
	"spawn_avalae_imm1_053",
	"spawn_avalae_imm1_054",
	"spawn_avalae_imm1_055",
	"spawn_avalae_imm2_056",
	"spawn_avalae_imm2_057",
	"spawn_avalae_imm2_058",
	"spawn_global_route_gouffre_zorai",
	"spawn_kami_place_zora",
	"spawn_karavan_place_zora",
	"spawn_kami_place_hoi_cho",
	"spawn_kami_place_jen_lai",
	"spawn_kami_place_min_cho",
	"spawn_global_terre_zorai",
	"spawn_global_sources_zorai",
	"spawn_kami_maidengrove",
	"spawn_kami_havenofpurity",
	"spawn_kami_region_groveofumbra",
	"spawn_karavan_region_groveofumbra",
	"spawn_karavan_region_groveofumbra",
	"spawn_kami_region_thevoid",
	"spawn_karavan_region_thevoid",
	"spawn_kami_region_knotofdementia",
	"spawn_karavan_region_knotofdementia",
	"spawn_outpost_zorai_01",
	"spawn_outpost_zorai_02",
	"spawn_outpost_zorai_03",
	"spawn_outpost_zorai_04",
	"spawn_outpost_zorai_05",
	"spawn_outpost_zorai_06",
	"spawn_outpost_zorai_07",
	"spawn_outpost_zorai_08",
	"spawn_outpost_zorai_09",
	"spawn_outpost_zorai_10",
	"spawn_outpost_zorai_11",
	"spawn_outpost_zorai_12",
	"spawn_outpost_zorai_13",
	"spawn_outpost_zorai_14",
	"spawn_outpost_zorai_15",
	"spawn_outpost_zorai_16",
	"spawn_outpost_zorai_17",
	"spawn_outpost_zorai_18",
	"spawn_outpost_zorai_19",
	"spawn_outpost_zorai_20",
	"spawn_outpost_zorai_21",
	"spawn_outpost_zorai_22",
	"spawn_outpost_zorai_23",
	"spawn_outpost_zorai_24",
	"spawn_outpost_zorai_25",
	"spawn_outpost_zorai_26",
	"spawn_outpost_zorai_27",
	"spawn_outpost_zorai_28",
	"spawn_outpost_zorai_29",
	"spawn_outpost_zorai_30",
	"spawn_zora_mairie_ss",
	"spawn_zora_mairie_rdc",
	"spawn_newbie_place_qai_lo",
	"start_newb_welcomer_qai_lo_1",
	"start_newb_welcomer_qai_lo_2",
	"start_newb_welcomer_qai_lo_3",
	"start_newb_welcomer_qai_lo_4",
	"start_newb_welcomer_qai_lo_5",
	"spawn_newbie_place_sheng_wo",
	"start_newb_welcomer_sheng_wo_1",
	"start_newb_welcomer_sheng_wo_2",
	"start_newb_welcomer_sheng_wo_3",
	"start_newb_welcomer_sheng_wo_4",
	"start_newb_welcomer_sheng_wo_5",
	"spawn_newbie_place_men_xing",
	"start_newb_welcomer_men_xing_1",
	"start_newb_welcomer_men_xing_2",
	"start_newb_welcomer_men_xing_3",
	"start_newb_welcomer_men_xing_4",
	"start_newb_welcomer_men_xing_5",
	"spawn_newbie_place_koi_zun",
	"start_newb_welcomer_koi_zun_1",
	"start_newb_welcomer_koi_zun_2",
	"start_newb_welcomer_koi_zun_3",
	"start_newb_welcomer_koi_zun_4",
	"start_newb_welcomer_koi_zun_5",
	"spawn_newbie_place_yin_piang",
	"start_newb_welcomer_yin_piang_1",
	"start_newb_welcomer_yin_piang_2",
	"start_newb_welcomer_yin_piang_3",
	"start_newb_welcomer_yin_piang_4",
	"start_newb_welcomer_yin_piang_5",
	"spawn_inst_ZO_pjroom",
	"spawn_inst_ZO_guildhall",
	"spawn_inst_ZO_guildhall_ss",
	"spawn_inst_ZO_council",
	"spawn_inst_ZO_trainer",
	"spawn_ZO_Zor_103",
	"spawn_ZO_Zor_104",
	"spawn_ZO_Zor_105",
	"spawn_ZO_Zor_106",
	"spawn_ZO_Zor_107",
	"spawn_ZO_Zor_108",
	"spawn_ZO_Zor_109",
	"spawn_ZO_Zor_110",
	"spawn_ZO_Zor_111",
	"spawn_ZO_Zor_112",
	"spawn_ZO_Zor_113",
	"spawn_ZO_Zor_114",
	"spawn_QaiLo_148",
	"spawn_MenXing_151",
	"spawn_ShengWo_154",
	"spawn_YinPiang_157",
	"spawn_KoiZun_160",
	"spawn_jenlai_imm1_133",
	"spawn_jenlai_imm1_134",
	"spawn_jenlai_imm1_135",
	"spawn_mincho_imm1_125",
	"spawn_mincho_imm1_126",
	"spawn_mincho_imm1_127",
	"spawn_hoicho_imm1_120",
	"spawn_hoicho_imm1_121",
	"spawn_hoicho_imm1_122",
	"spawn_kami_place_pyr",
	"spawn_karavan_place_pyr",
	"spawn_kami_place_dyron",
	"spawn_global_sources_fyros",
	"spawn_kami_place_thesos",
	"spawn_global_matis_fyros",
	"spawn_global_route_gouffre_fyros",
	"pvp_challenge_fyros_spawn_2",
	"pvp_challenge_fyros_spawn_1",
	"spawn_kami_oflovaksoasis",
	"spawn_kami_frahartowers",
	"spawn_karavan_frahartowers",
	"spawn_kami_outlawcanyon",
	"spawn_karavan_outlawcanyon",
	"spawn_kami_sawdustmines",
	"spawn_karavan_sawdustmines",
	"spawn_karavan_region_thescorchedcorridor",
	"spawn_kami_region_thescorchedcorridor",
	"spawn_karavan_place_thesos",
	"spawn_karavan_place_dyron",
	"spawn_outpost_fyros_01",
	"spawn_outpost_fyros_02",
	"spawn_outpost_fyros_03",
	"spawn_outpost_fyros_04",
	"spawn_outpost_fyros_05",
	"spawn_outpost_fyros_06",
	"spawn_outpost_fyros_07",
	"spawn_outpost_fyros_08",
	"spawn_outpost_fyros_09",
	"spawn_outpost_fyros_10",
	"spawn_outpost_fyros_11",
	"spawn_outpost_fyros_12",
	"spawn_outpost_fyros_13",
	"spawn_outpost_fyros_14",
	"spawn_outpost_fyros_15",
	"spawn_outpost_fyros_16",
	"spawn_outpost_fyros_17",
	"spawn_outpost_fyros_18",
	"spawn_outpost_fyros_19",
	"spawn_outpost_fyros_20",
	"spawn_outpost_fyros_21",
	"spawn_outpost_fyros_22",
	"spawn_outpost_fyros_23",
	"spawn_outpost_fyros_24",
	"spawn_outpost_fyros_25",
	"spawn_outpost_fyros_26",
	"spawn_outpost_fyros_27",
	"spawn_outpost_fyros_28",
	"spawn_local_oasis_1st_rdc",
	"spawn_local_oasis_rdc_1st",
	"spawn_local_fortress_1st_rdc",
	"spawn_local_fortress_rdc_1st",
	"spawn_local_war_ss_rdc",
	"spawn_local_war_rdc_ss",
	"spawn_local_sherif_arm_1st_rdc",
	"spawn_local_sherif_arm_rdc_1st",
	"spawn_local_sherif_1st_rdc",
	"spawn_local_sherif_rdc_1st",
	"spawn_newbie_place_aegus",
	"start_newb_welcomer_aegus_1",
	"start_newb_welcomer_aegus_2",
	"start_newb_welcomer_aegus_3",
	"start_newb_welcomer_aegus_4",
	"start_newb_welcomer_aegus_5",
	"spawn_newbie_place_kaemon",
	"start_newb_welcomer_kaemon_1",
	"start_newb_welcomer_kaemon_2",
	"start_newb_welcomer_kaemon_3",
	"start_newb_welcomer_kaemon_4",
	"start_newb_welcomer_kaemon_5",
	"spawn_newbie_place_sekovix",
	"start_newb_welcomer_sekovix_1",
	"start_newb_welcomer_sekovix_2",
	"start_newb_welcomer_sekovix_3",
	"start_newb_welcomer_sekovix_4",
	"start_newb_welcomer_sekovix_5",
	"spawn_newbie_place_phyxon",
	"start_newb_welcomer_phyxon_1",
	"start_newb_welcomer_phyxon_2",
	"start_newb_welcomer_phyxon_3",
	"start_newb_welcomer_phyxon_4",
	"start_newb_welcomer_phyxon_5",
	"spawn_newbie_place_galemus",
	"start_newb_welcomer_galemus_1",
	"start_newb_welcomer_galemus_2",
	"start_newb_welcomer_galemus_3",
	"start_newb_welcomer_galemus_4",
	"start_newb_welcomer_galemus_5",
	"spawn_FY_pyr_fontaine",
	"spawn_inst_FY_pjroom",
	"spawn_inst_FY_guildhall",
	"spawn_inst_FY_guildhall_fight",
	"spawn_inst_FY_guildhall_magic",
	"spawn_inst_FY_guildhall_harvest",
	"spawn_inst_FY_guildhall_craft",
	"spawn_inst_FY_conseil",
	"spawn_inst_FY_trainer",
	"spawn_FY_pyr_317",
	"spawn_FY_pyr_318",
	"spawn_FY_pyr_319",
	"spawn_FY_pyr_320",
	"spawn_FY_pyr_321",
	"spawn_FY_pyr_322",
	"spawn_FY_pyr_323",
	"spawn_FY_pyr_324",
	"spawn_FY_pyr_325",
	"spawn_FY_pyr_326",
	"spawn_FY_pyr_327",
	"spawn_FY_pyr_328",
	"spawn_aegus",
	"spawn_kaemon",
	"spawn_sekovix",
	"spawn_phyxon",
	"spawn_galemus",
	"spawn_global_matis_tryker",
	"spawn_global_route_gouffre_tryker",
	"spawn_karavan_place_avendale",
	"spawn_karavan_place_crystabell",
	"spawn_kami_place_fairhaven",
	"spawn_karavan_place_fairhaven",
	"spawn_karavan_place_windermeer",
	"spawn_local_lagoonsofloria_windsofmuse",
	"spawn_local_windsofmuse_lagoonsofloria",
	"spawn_local_lagoonsofloria_libertylake",
	"spawn_local_libertylake_lagoonsofloria",
	"spawn_local_windsofmuse_dewdrops",
	"spawn_local_dewdrops_windsofmuse",
	"spawn_local_windsofmuse_dewdrops_2",
	"spawn_local_dewdrops_windsofmuse_2",
	"spawn_local_bountybeaches_windsofmuse",
	"spawn_local_windsofmuse_bountybeaches",
	"spawn_local_bountybeaches_libertylake",
	"spawn_local_libertylake_bountybeaches",
	"spawn_local_bountybeaches_thefount",
	"spawn_local_thefount_bountybeaches",
	"spawn_local_thefount_restingwater",
	"spawn_local_restingwater_thefount",
	"spawn_local_restingwater_enchantedisle",
	"spawn_local_enchantedisle_restingwater",
	"spawn_local_enchantedisle_libertylake",
	"spawn_local_libertylake_enchantedisle",
	"spawn_kami_region_bountybeaches",
	"spawn_karavan_region_bountybeaches",
	"spawn_karavan_region_lagoonsofloria",
	"spawn_kami_region_lagoonsofloria",
	"spawn_karavan_dewdrops",
	"spawn_kami_restingwater",
	"spawn_karavan_restingwater",
	"spawn_kami_windsofmuse",
	"spawn_karavan_windsofmuse",
	"spawn_karavan_thefount",
	"spawn_kami_enchantedisle",
	"spawn_karavan_enchantedisle",
	"spawn_outpost_tryker_02",
	"spawn_outpost_tryker_03",
	"spawn_outpost_tryker_04",
	"spawn_outpost_tryker_05",
	"spawn_outpost_tryker_06",
	"spawn_outpost_tryker_07",
	"spawn_outpost_tryker_08",
	"spawn_outpost_tryker_09",
	"spawn_outpost_tryker_10",
	"spawn_outpost_tryker_11",
	"spawn_outpost_tryker_12",
	"spawn_outpost_tryker_13",
	"spawn_outpost_tryker_14",
	"spawn_outpost_tryker_15",
	"spawn_outpost_tryker_16",
	"spawn_outpost_tryker_17",
	"spawn_outpost_tryker_18",
	"spawn_outpost_tryker_19",
	"spawn_outpost_tryker_20",
	"spawn_outpost_tryker_21",
	"spawn_outpost_tryker_22",
	"spawn_outpost_tryker_23",
	"spawn_outpost_tryker_24",
	"spawn_outpost_tryker_25",
	"spawn_outpost_tryker_26",
	"spawn_outpost_tryker_27",
	"spawn_outpost_tryker_28",
	"spawn_outpost_tryker_29",
	"spawn_outpost_tryker_30",
	"spawn_outpost_tryker_31",
	"spawn_outpost_tryker_32",
	"spawn_FH_mairie_rdc",
	"spawn_FH_mairie_1st",
	"spawn_FH_mairie_2nd",
	"spawn_newbie_place_aubermouth",
	"start_newb_welcomer_aubermouth_1",
	"start_newb_welcomer_aubermouth_2",
	"start_newb_welcomer_aubermouth_3",
	"start_newb_welcomer_aubermouth_4",
	"start_newb_welcomer_aubermouth_5",
	"spawn_newbie_place_barkdell",
	"start_newb_welcomer_barkdell_1",
	"start_newb_welcomer_barkdell_2",
	"start_newb_welcomer_barkdell_3",
	"start_newb_welcomer_barkdell_4",
	"start_newb_welcomer_barkdell_5",
	"spawn_newbie_place_hobwelly",
	"start_newb_welcomer_hobwelly_1",
	"start_newb_welcomer_hobwelly_2",
	"start_newb_welcomer_hobwelly_3",
	"start_newb_welcomer_hobwelly_4",
	"start_newb_welcomer_hobwelly_5",
	"spawn_newbie_place_waverton",
	"start_newb_welcomer_waverton_1",
	"start_newb_welcomer_waverton_2",
	"start_newb_welcomer_waverton_3",
	"start_newb_welcomer_waverton_4",
	"start_newb_welcomer_waverton_5",
	"spawn_newbie_place_dingleton",
	"start_newb_welcomer_dingleton_1",
	"start_newb_welcomer_dingleton_2",
	"start_newb_welcomer_dingleton_3",
	"start_newb_welcomer_dingleton_4",
	"start_newb_welcomer_dingleton_5",
	"spawn_inst_TR_pjroom",
	"spawn_inst_TR_guildhall",
	"spawn_inst_TR_guildhall_2",
	"spawn_inst_TR_guildhall_ss_fight",
	"spawn_inst_TR_guildhall_ss_magic",
	"spawn_inst_TR_guildhall_ss_craft",
	"spawn_inst_TR_conseil",
	"spawn_inst_TR_trainer",
	"spawn_TR_fair_204",
	"spawn_TR_fair_205",
	"spawn_TR_fair_217",
	"spawn_TR_fair_219",
	"spawn_TR_fair_223",
	"spawn_TR_fair_225",
	"spawn_TR_fair_229",
	"spawn_TR_fair_231",
	"spawn_auber_1st",
	"spawn_auber_rdc",
	"spawn_waver_1st",
	"spawn_waver_rdc",
	"spawn_bark_rdc",
	"spawn_bark_1st",
	"spawn_ding_rdc",
	"spawn_ding_1st",
	"spawn_hobw_rdc",
	"spawn_hobw_1st",
	"spawn_windermeer_task_289",
	"spawn_windermeer_task_290",
	"spawn_windermeer_task_291",
	"spawn_windermeer_task_292",
	"spawn_windermeer_task_terrace_1st",
	"spawn_windermeer_rite_275",
	"spawn_windermeer_rite_terrace_1st",
	"spawn_avendale_task_264",
	"spawn_avendale_task_terrace_1st",
	"spawn_avendale_rite_terrace_1st",
	"spawn_avendale_rite_269",
	"spawn_crystabell_task_249",
	"spawn_crystabell_task_terrace_1st",
	"spawn_crystabell_rite_243",
	"spawn_crystabell_rite_terrace_1st",
	"spawn_global_zorai_terre",
	"spawn_global_nexus_terre",
	"spawn_kami_forbidden_depths",
	"spawn_karavan_forbidden_depths",
	"spawn_kami_region_the_land_of_continuity",
	"spawn_karavan_region_the_land_of_continuity",
	"spawn_kami_region_the_sunken_city",
	"spawn_outpost_prime_root_20",
	"spawn_outpost_prime_root_21",
	"spawn_outpost_prime_root_22",
	"spawn_outpost_prime_root_23",
	"spawn_outpost_prime_root_24",
	"spawn_outpost_prime_root_25",
	"spawn_outpost_prime_root_26",
	"spawn_outpost_prime_root_27",
	"spawn_outpost_prime_root_28",
	"spawn_outpost_prime_root_29",
	"spawn_outpost_prime_root_30",
	"spawn_global_fyros_sources",
	"spawn_global_zorai_sources",
	"spawn_kami_region_the_under_spring_fyros",
	"spawn_karavan_region_the_under_spring_fyros",
	"spawn_outpost_prime_root_17",
	"spawn_outpost_prime_root_18",
	"spawn_outpost_prime_root_19",
	"spawn_global_fyros_route_gouffre",
	"spawn_global_nexus_route_gouffre",
	"spawn_global_zorai_route_gouffre",
	"spawn_global_tryker_route_gouffre",
	"spawn_kami_region_the_trench_of_trials_zorai",
	"spawn_karavan_region_the_trench_of_trials_zorai",
	"spawn_kami_region_the_elusive_forest",
	"spawn_karavan_region_the_elusive_forest",
	"spawn_kami_region_the_windy_gate",
	"spawn_karavan_region_the_windy_gate",
	"spawn_karavan_gate_of_obscurity",
	"spawn_outpost_prime_root_04",
	"spawn_outpost_prime_root_05",
	"spawn_outpost_prime_root_06",
	"spawn_outpost_prime_root_07",
	"spawn_outpost_prime_root_08",
	"spawn_outpost_prime_root_09",
	"spawn_outpost_prime_root_10",
	"spawn_outpost_prime_root_11",
	"spawn_outpost_prime_root_12",
	"spawn_outpost_prime_root_13",
	"spawn_outpost_prime_root_14",
	"spawn_outpost_prime_root_15",
	"spawn_outpost_prime_root_16",
	"spawn_global_bagne_nexus",
	"spawn_global_route_gouffre_nexus",
	"spawn_global_terre_nexus",
	"spawn_kami_region_nexus_terre",
	"spawn_karavan_region_nexus_terre",
	"spawn_outpost_nexus_01",
	"spawn_outpost_nexus_02",
	"spawn_outpost_nexus_03",
	"spawn_outpost_nexus_04",
	"spawn_outpost_nexus_05",
	"spawn_outpost_nexus_06",
	"spawn_outpost_nexus_07",
	"spawn_global_matis_bagne",
	"spawn_global_nexus_bagne",
	"spawn_karavan_region_the_abyss_of_ichor_matis",
	"spawn_kami_region_the_abyss_of_ichor_matis",
	"spawn_outpost_prime_root_01",
	"spawn_outpost_prime_root_02",
	"spawn_outpost_prime_root_03"
};

static vector<uint16> OldToNewSpawnZoneId;

void initSpawnZonesCompat()
{
	static bool initialized = false;
	if (initialized)
		return;

	const uint nbOldSpawnZones = sizeof(OldSpawnZones) / sizeof(OldSpawnZones[0]);

	OldToNewSpawnZoneId.resize( nbOldSpawnZones );
	for (uint i = 0; i < nbOldSpawnZones; i++)
	{
		const uint16 newId = CZoneManager::getInstance().getTpSpawnZoneIdByName( OldSpawnZones[i] );
		OldToNewSpawnZoneId[i] = newId;

		if (newId == 0xffff)
			nlwarning("<SPAWN_ZONE> old spawn zone '%s' has not been found", OldSpawnZones[i].c_str());
	}

	initialized = true;
}

uint16 oldToNewSpawnZoneId(uint16 oldId)
{
	BOMB_IF( (oldId >= OldToNewSpawnZoneId.size()), "<oldToNewSpawnZoneId> invalid old spawn zone index", return 0xffff );

	return OldToNewSpawnZoneId[oldId];
}

} // namespace BACK_COMPAT

