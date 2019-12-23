#include "stdafx.h"
#include "ZaishenCycles.h"


using namespace ZaishenCycles;
using namespace GW::Constants::QuestID;

const std::vector<uint32_t> zaishen_missions = {
		ZaishenMission::Augury_Rock,
		ZaishenMission::Grand_Court_of_Sebelkeh,
		ZaishenMission::Ice_Caves_of_Sorrow,
		ZaishenMission::Raisu_Palace,
		ZaishenMission::Gate_of_Desolation,
		ZaishenMission::Thirsty_River,
		ZaishenMission::Blacktide_Den,
		ZaishenMission::Against_the_Charr,
		ZaishenMission::Abaddons_Mouth,
		ZaishenMission::Nundu_Bay,
		ZaishenMission::Divinity_Coast,
		ZaishenMission::Zen_Daijun,
		ZaishenMission::Pogahn_Passage,
		ZaishenMission::Tahnnakai_Temple,
		ZaishenMission::The_Great_Northern_Wall,
		ZaishenMission::Dasha_Vestibule,
		ZaishenMission::The_Wilds,
		ZaishenMission::Unwaking_Waters,
		ZaishenMission::Chahbek_Village,
		ZaishenMission::Aurora_Glade,
		ZaishenMission::A_Time_for_Heroes,
		ZaishenMission::Consulate_Docks,
		ZaishenMission::Ring_of_Fire,
		ZaishenMission::Nahpui_Quarter,
		ZaishenMission::The_Dragons_Lair,
		ZaishenMission::Dzagonur_Bastion,
		ZaishenMission::DAlessio_Seaboard,
		ZaishenMission::Assault_on_the_Stronghold,
		ZaishenMission::The_Eternal_Grove,
		ZaishenMission::Sanctum_Cay,
		ZaishenMission::Rilohn_Refuge,
		ZaishenMission::Warband_of_Brothers,
		ZaishenMission::Borlis_Pass,
		ZaishenMission::Imperial_Sanctum,
		ZaishenMission::Moddok_Crevice,
		ZaishenMission::Nolani_Academy,
		ZaishenMission::Destructions_Depths,
		ZaishenMission::Venta_Cemetery,
		ZaishenMission::Fort_Ranik,
		ZaishenMission::A_Gate_Too_Far,
		ZaishenMission::Minister_Chos_Estate,
		ZaishenMission::Thunderhead_Keep,
		ZaishenMission::Tihark_Orchard,
		ZaishenMission::Finding_the_Bloodstone,
		ZaishenMission::Dunes_of_Despair,
		ZaishenMission::Vizunah_Square,
		ZaishenMission::Jokanur_Diggings,
		ZaishenMission::Iron_Mines_of_Moladune,
		ZaishenMission::Kodonur_Crossroads,
		ZaishenMission::G_O_L_E_M,
		ZaishenMission::Arborstone,
		ZaishenMission::Gates_of_Kryta,
		ZaishenMission::Gate_of_Madness,
		ZaishenMission::The_Elusive_Golemancer,
		ZaishenMission::Riverside_Province,
		ZaishenMission::Boreas_Seabed,
		ZaishenMission::Ruins_of_Morah,
		ZaishenMission::Hells_Precipice,
		ZaishenMission::Ruins_of_Surmia,
		ZaishenMission::Curse_of_the_Nornbear,
		ZaishenMission::Sunjiang_District,
		ZaishenMission::Elona_Reach,
		ZaishenMission::Gate_of_Pain,
		ZaishenMission::Blood_Washes_Blood,
		ZaishenMission::Bloodstone_Fen,
		ZaishenMission::Jennurs_Horde,
		ZaishenMission::Gyala_Hatchery,
		ZaishenMission::Abaddons_Gate,
		ZaishenMission::The_Frost_Gate
};


const std::vector<uint32_t> zaishen_bounties = {
		0, //ZaishenBounty::Droajam_Mage_of_the_Sands,
		0, //ZaishenBounty::Royen_Beastkeeper,
		ZaishenBounty::Eldritch_Ettin,
		0, //ZaishenBounty::Vengeful_Aatxe,
		ZaishenBounty::Fronis_Irontoe,
		0, //ZaishenBounty::Urgoz,
		0, //ZaishenBounty::Fenrir,
		ZaishenBounty::Selvetarm,
		0, //ZaishenBounty::Mohby_Windbeak,
		0, //ZaishenBounty::Charged_Blackness,
		0, //ZaishenBounty::Rotscale,
		ZaishenBounty::Zoldark_the_Unholy,
		0, //ZaishenBounty::Korshek_the_Immolated,
		0, //ZaishenBounty::Myish_Lady_of_the_Lake,
		ZaishenBounty::Frostmaw_the_Kinslayer,
		0, //ZaishenBounty::Kunvie_Firewing,
		ZaishenBounty::Zhim_Monns,
		0, //ZaishenBounty::The_Greater_Darkness,
		ZaishenBounty::TPS_Regulator_Golem,
		ZaishenBounty::Plague_of_Destruction,
		0, //ZaishenBounty::The_Darknesses,
		0, //ZaishenBounty::Admiral_Kantoh,
		0, //ZaishenBounty::Borrguus_Blisterbark,
		ZaishenBounty::Forgewight,
		0, //ZaishenBounty::Baubao_Wavewrath,
		0, //ZaishenBounty::Joffs_the_Mitigator,
		ZaishenBounty::Rragar_Maneater,
		0, //ZaishenBounty::Chung_the_Attuned,
		0, //ZaishenBounty::Lord_Jadoth,
		0, //ZaishenBounty::Nulfastu_Earthbound,
		0, //ZaishenBounty::The_Iron_Forgeman,
		ZaishenBounty::Magmus,
		0, //ZaishenBounty::Mobrin_Lord_of_the_Marsh,
		0, //ZaishenBounty::Jarimiya_the_Unmerciful,
		ZaishenBounty::Duncan_the_Black,
		0, //ZaishenBounty::Quansong_Spiritspeak,
		0, //ZaishenBounty::The_Stygian_Underlords,
		0, //ZaishenBounty::Fozzy_Yeoryios,
		0, //ZaishenBounty::The_Black_Beast_of_Arrgh,
		ZaishenBounty::Arachni,
		0, //ZaishenBounty::The_Four_Horsemen,
		ZaishenBounty::Remnant_of_Antiquities,
		0, //ZaishenBounty::Arbor_Earthcall,
		ZaishenBounty::Prismatic_Ooze,
		0, //ZaishenBounty::Lord_Khobay,
		0, //ZaishenBounty::Jedeh_the_Mighty,
		0, //ZaishenBounty::Ssuns_Blessed_of_Dwayna,
		ZaishenBounty::Justiciar_Thommis,
		0, //ZaishenBounty::Harn_and_Maxine_Coldstone,
		0, //ZaishenBounty::Pywatt_the_Swift,
		ZaishenBounty::Fendi_Nin,
		0, //ZaishenBounty::Mungri_Magicbox,
		0, //ZaishenBounty::Priest_of_Menzies,
		ZaishenBounty::Ilsundur_Lord_of_Fire,
		0, //ZaishenBounty::Kepkhet_Marrowfeast,
		0, //ZaishenBounty::Commander_Wahli,
		0, //ZaishenBounty::Kanaxai,
		0, //ZaishenBounty::Khabuus,
		0, //ZaishenBounty::Molotov_Rocktail,
		0, //ZaishenBounty::The_Stygian_Lords,
		0, //ZaishenBounty::Dragon_Lich,
		ZaishenBounty::Havok_Soulwail,
		0, //ZaishenBounty::Ghial_the_Bone_Dancer,
		ZaishenBounty::Murakai_Lady_of_the_Night,
		ZaishenBounty::Rand_Stormweaver,
		0, //ZaishenBounty::Verata,
};


uint32_t ZaishenCycles::GetZiashenMission(std::chrono::system_clock::time_point time)
{
	auto cycle_start = system_clock::from_time_t(1299168000);
	auto delta = duration_cast<std::chrono::seconds>(time - cycle_start);
	size_t index = (delta.count() / 86400) % zaishen_missions.size();
	return zaishen_missions.at(index);
}


uint32_t ZaishenCycles::GetZiashenBounty(std::chrono::system_clock::time_point time)
{
	auto cycle_start = system_clock::from_time_t(1244736000);
	auto delta = duration_cast<std::chrono::seconds>(time - cycle_start);
	size_t index = (delta.count() / 86400) % zaishen_bounties.size();
	return zaishen_bounties.at(index);
}
