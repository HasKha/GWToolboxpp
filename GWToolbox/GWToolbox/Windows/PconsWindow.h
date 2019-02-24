#pragma once

#include <list>
#include <vector>
#include <Defines.h>

#include <GWCA\Constants\Constants.h>
#include <GWCA\GameEntities\Position.h>
#include <GWCA\GameEntities\Agent.h>

#include "Timer.h"
#include "Pcons.h"
#include "ToolboxWindow.h"

class PconsWindow : public ToolboxWindow {
	PconsWindow() {};
	~PconsWindow() {};
public:
	static PconsWindow& Instance() {
		static PconsWindow instance;
		return instance;
	}

	const char* Name() const override { return "Pcons"; }

	void Initialize() override;

	bool SetEnabled(bool b);
	bool GetEnabled();

	inline void ToggleEnable() { SetEnabled(!enabled); }

	void Update(float delta) override;

	bool DrawTabButton(IDirect3DDevice9* device, bool show_icon, bool show_text) override;
	void Draw(IDirect3DDevice9* pDevice) override;

	void LoadSettings(CSimpleIni* ini) override;
	void SaveSettings(CSimpleIni* ini) override;
	void DrawSettingInternal() override;

private:
	std::vector<Pcon*> pcons;
	PconAlcohol* pcon_alcohol = nullptr;
	GW::Constants::InstanceType current_map_type;
	clock_t scan_inventory_timer;
	bool enabled = false;

	// Interface Settings
	bool tick_with_pcons = false;
	int items_per_row = 3;
	bool show_enable_button = true;
	bool show_auto_refill_pcons_tickbox = true;
	bool show_auto_disable_pcons_tickbox = false;
	GW::Agent* player;

	// Pcon Settings
	// todo: tonic pop?
	// todo: morale / dp removal
	// todo: replenish character pcons from chest?

	void MapChanged(); // Called via Update() when map id changes
	// Elite area auto disable
	void CheckBossRangeAutoDisable();	// Trigger Elite area auto disable if applicable
	void CheckObjectivesCompleteAutoDisable();

	GW::Constants::MapID map_id;
	GW::Constants::InstanceType instance_type;
	bool elite_area_disable_triggered = false;	// Already triggered in this run?
	clock_t elite_area_check_timer;

	
	// Map of which objectives to check per map_id
	std::vector<DWORD> objectives_complete = {};
	bool disable_cons_on_objective_completion = false;
	std::map<GW::Constants::MapID, std::vector<DWORD>> objectives_to_complete_by_map_id = {
		{GW::Constants::MapID::The_Fissure_of_Woe,{ 309,310,311,312,313,314,315,316,317,318,319 }}, // Can be done in any order - check them all.
		{GW::Constants::MapID::The_Deep, { 421 }},
		{GW::Constants::MapID::Urgozs_Warren, { 357 }},
		{GW::Constants::MapID::The_Underworld,{ 157 }} // Only need to check for Nightman Cometh for Underworld.
	};
	std::vector<DWORD> current_objectives_to_check = {};
	char* disable_cons_on_objective_completion_hint = "Disable cons when final objective(s) completed";
	// Map of which locations to turn off near by map_id e.g. Kanaxai, Urgoz
	bool disable_cons_in_final_room = false;
	std::map<GW::Constants::MapID, GW::Vector2f> final_room_location_by_map_id = {
		{GW::Constants::MapID::The_Deep, GW::Vector2f(30428.0f, -5842.0f)},		// Rough location of Kanaxai
		{GW::Constants::MapID::Urgozs_Warren, GW::Vector2f(-2800.0f, 14316.0f)} // Front entrance of Urgoz's room
	};
	char* disable_cons_in_final_room_hint = "Disable cons when reaching the final room in Urgoz and Deep";
	GW::Vector2f current_final_room_location = NULL;
	
};
