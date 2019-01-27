#include "GameSettings.h"

#include <ctime>
#include <ShellApi.h>

#include <GWCA\Managers\GameThreadMgr.h>
#include <GWCA\Managers\MapMgr.h>
#include <GWCA\Managers\ChatMgr.h>
#include <GWCA\Managers\PartyMgr.h>
#include <GWCA\Managers\ItemMgr.h>
#include <GWCA\Managers\AgentMgr.h>
#include <GWCA\Managers\StoCMgr.h>
#include <GWCA\Managers\FriendListMgr.h>
#include <GWCA\Managers\Render.h>
#include <GWCA\Managers\CameraMgr.h>
#include <GWCA\Managers\UIMgr.h>

#include <GWCA\Context\GameContext.h>

#include <logger.h>
#include "GuiUtils.h"
#include <GWToolbox.h>
#include <Timer.h>
#include <Color.h>

namespace {
#if 0 // @Deprecated
	void ChatEventCallback(DWORD id, DWORD type, wchar_t* info, void* unk) {
		if (type == 0x29 && GameSettings::Instance().select_with_chat_doubleclick) {
			static wchar_t last_name[64] = L"";
			static clock_t timer = TIMER_INIT();
			if (TIMER_DIFF(timer) < 500 && wcscmp(last_name, info) == 0) {
				GW::PlayerArray players = GW::Agents::GetPlayerArray();
				if (players.valid()) {
					for (unsigned i = 0; i < players.size(); ++i) {
						GW::Player& player = players[i];
						wchar_t* name = player.Name;
						if (player.AgentID > 0
							&& name != nullptr
							&& wcscmp(info, name) == 0) {
							GW::Agents::ChangeTarget(players[i].AgentID);
						}
					}
				}
			} else {
				timer = TIMER_INIT();
				wcscpy_s(last_name, info);
			}
		}
	}
#endif

	void SendChatCallback(GW::Chat::Channel chan, wchar_t msg[120]) {
		if (!GameSettings::Instance().auto_url || !msg) return;
		size_t len = wcslen(msg);
		size_t max_len = 120;

		if (chan == GW::Chat::CHANNEL_WHISPER) {
			// msg == "Whisper Target Name,msg"
			size_t i;
			for (i = 0; i < len; i++)
				if (msg[i] == ',')
					break;

			if (i < len) {
				msg += i + 1;
				len -= i + 1;
				max_len -= i + 1;
			}
		}

		if (wcsncmp(msg, L"http://", 7) && wcsncmp(msg, L"https://", 8)) return;

		if (len + 5 < max_len) {
			for (int i = len; i > 0; i--)
				msg[i] = msg[i - 1];
			msg[0] = '[';
			msg[len + 1] = ';';
			msg[len + 2] = 'x';
			msg[len + 3] = 'x';
			msg[len + 4] = ']';
			msg[len + 5] = 0;
		}
	}

	void FlashWindow() {
		FLASHWINFO flashInfo = { 0 };
		flashInfo.cbSize = sizeof(FLASHWINFO);
		flashInfo.hwnd = GW::MemoryMgr::GetGWWindowHandle();
		flashInfo.dwFlags = FLASHW_TIMER | FLASHW_TRAY | FLASHW_TIMERNOFG;
		flashInfo.uCount = 0;
		flashInfo.dwTimeout = 0;
		FlashWindowEx(&flashInfo);
	}

	void PrintTime(wchar_t *buffer, size_t n, DWORD time_sec) {
		DWORD secs = time_sec % 60;
		DWORD minutes = (time_sec / 60) % 60;
		DWORD hours = time_sec / 3600;
		DWORD time = 0;
		const wchar_t *time_unit = L"";
		if (hours != 0) {
			time_unit = L"hour";
			time = hours;
		} else if (minutes != 0) {
			time_unit = L"minute";
			time = minutes;
		} else {
			time_unit = L"second";
			time = secs;
		}
		if (time > 1) {
			swprintf(buffer, n, L"%lu %ss", time, time_unit);
		} else {
			swprintf(buffer, n, L"%lu %s", time, time_unit);
		}
	}

	const wchar_t *GetPlayerName(void) {
		GW::Agent *player = GW::Agents::GetPlayer();
		if (!player) return L"";
		DWORD playerNumber = player->PlayerNumber;
		return GW::Agents::GetPlayerNameByLoginNumber(playerNumber);
	}

	void WhisperCallback(const wchar_t from[20], const wchar_t msg[140]) {
		GameSettings&  game_setting = GameSettings::Instance();
		if (game_setting.flash_window_on_pm) FlashWindow();
		DWORD status = GW::FriendListMgr::GetMyStatus();
		if (status == GW::FriendStatus_Away && !game_setting.afk_message.empty()) {
			wchar_t buffer[120];
			DWORD diff_time = (clock() - game_setting.afk_message_time) / CLOCKS_PER_SEC;
			wchar_t time_buffer[128];
			PrintTime(time_buffer, 128, diff_time);
			swprintf(buffer, 120, L"Automatic message: \"%s\" (%s ago)", game_setting.afk_message.c_str(), time_buffer);
			// Avoid infinite recursion
			if (wcsncmp(from, ::GetPlayerName(), 20))
				GW::Chat::SendChat(from, buffer);
		}
	}

	int move_materials_to_storage(GW::Item *item) {
		assert(item && item->Quantity);
		assert(item->GetIsMaterial());

		int slot = GW::Items::GetMaterialSlot(item);
		if (slot < 0 || (int)GW::Constants::N_MATS <= slot) return 0;
		int availaible = 250;
		GW::Item *b_item = GW::Items::GetItemBySlot(GW::Constants::Bag::Material_Storage, slot + 1);
		if (b_item) availaible = 250 - b_item->Quantity;
		int will_move = std::min((int)item->Quantity, availaible);
		if (will_move) GW::Items::MoveItem(item, GW::Constants::Bag::Material_Storage, slot, will_move);
		return will_move;
	}

	// From bag_first to bag_last (included) i.e. [bag_first, bag_last]
	// Returns the amount moved
	int complete_existing_stack(GW::Item *item, int bag_first, int bag_last, int remaining) {
		if (!item->GetIsStackable() || remaining == 0) return 0;
		int remaining_start = remaining;
		for (int bag_i = bag_first; bag_i <= bag_last; bag_i++) {
			GW::Bag *bag = GW::Items::GetBag(bag_i);
			if (!bag) continue;
			size_t slot = bag->find2(item);
			while (slot != GW::Bag::npos) {
				GW::Item *b_item = bag->Items[slot];
				// b_item can be null in the case of birthday present for instance.
				if (b_item != nullptr) {
					int availaible = 250 - b_item->Quantity;
					int will_move = std::min(availaible, remaining);
					if (will_move > 0) {
						GW::Items::MoveItem(item, b_item, will_move);
						remaining -= will_move;
					}
					if (remaining == 0)
						return remaining_start;
				}
				slot = bag->find2(item, slot + 1);
			}
		}
		return remaining_start - remaining;
	}

	void move_to_first_empty_slot(GW::Item *item, int bag_first, int bag_last) {
		for (int bag_i = bag_first; bag_i <= bag_last; bag_i++) {
			GW::Bag *bag = GW::Items::GetBag(bag_i);
			if (!bag) continue;
			size_t slot = bag->find1(0);
			// The reason why we test if the slot has no item is because birthday present have ModelId == 0
			while (slot != GW::Bag::npos) {
				if (bag->Items[slot] == nullptr) {
					GW::Items::MoveItem(item, bag, slot);
					return;
				}
				slot = bag->find1(0, slot + 1);
			}
		}
	}

	void move_item_to_storage_page(GW::Item *item, int page) {
		assert(item && item->Quantity);
		// 9 being the material storage
		if (page < 0 || 9 < page) return;

		if (page == 9) {
			if (!item->GetIsMaterial()) return;
			move_materials_to_storage(item);
			return;
		}

		assert(0 <= page && page < 9);
		const int storage1 = (int)GW::Constants::Bag::Storage_1;
		const int bag_index = storage1 + page;
		assert(GW::Items::GetBag(bag_index));

		int remaining = item->Quantity;

		// For materials, we always try to move what we can into the material page
		if (item->GetIsMaterial()) {
			int moved = move_materials_to_storage(item);
			remaining -= moved;
		}

		// if the item is stackable we try to complete stack that already exist in the current storage page
		int moved = complete_existing_stack(item, bag_index, bag_index, remaining);
		remaining -= moved;

		// if there is still item, we find the first empty slot and move everything there
		if (remaining) {
			move_to_first_empty_slot(item, bag_index, bag_index);
		}
	}

	void move_item_to_storage(GW::Item *item) {
		assert(item && item->Quantity);

		GW::Bag **bags = GW::Items::GetBagArray();
		if (!bags) return;
		int remaining = item->Quantity;

		// We try to move to the material storage
		if (item->GetIsMaterial()) {
			int moved = move_materials_to_storage(item);
			remaining -= moved;
		}

		const int storage1 = (int)GW::Constants::Bag::Storage_1;
		const int storage9 = (int)GW::Constants::Bag::Storage_9;

		// If item is stackable, try to complete similar stack
		if (remaining == 0) return;
		int moved = complete_existing_stack(item, storage1, storage9, remaining);
		remaining -= moved;

		// We find the first empty slot and put the remaining there
		if (remaining) {
			move_to_first_empty_slot(item, storage1, storage9);
		}
	}

	void move_item_to_inventory(GW::Item *item) {
		assert(item && item->Quantity);

		const int backpack = (int)GW::Constants::Bag::Backpack;
		const int bag2 = (int)GW::Constants::Bag::Bag_2;

		int total = item->Quantity;
		int remaining = total;

		// If item is stackable, try to complete similar stack
		int moved = complete_existing_stack(item, backpack, bag2, remaining);
		remaining -= moved;

		// If we didn't move any item (i.e. there was no stack to complete), move the stack to first empty slot
		if (remaining == total) {
			move_to_first_empty_slot(item, backpack, bag2);
		}
	}

	// This whole section is commented because packets are not up to date after the update. 
	// Should still work if you match the right packets.

#ifdef APRIL_FOOLS
	namespace AF {
		using namespace GW::Packet::StoC;
		void CreateXunlaiAgentFromGameThread(void) {
			{
				NpcGeneralStats packet;
				packet.header = NpcGeneralStats::STATIC_HEADER;
				packet.npc_id = 221;
				packet.file_id = 0x0001c601;
				packet.data1 = 0;
				packet.scale = 0x64000000;
				packet.data2 = 0;
				packet.flags = 0x0000020c;
				packet.profession = 3;
				packet.level = 15;
				wcsncpy(packet.name, L"\x8101\x1f4e\xd020\x87c8\x35a8\x0000", 8);

				GW::StoC::EmulatePacket((GW::Packet::StoC::PacketBase *)&packet);
			}

			{
				NPCModelFile packet;
				packet.header = NPCModelFile::STATIC_HEADER;
				packet.npc_id = 221;
				packet.count = 1;
				packet.data[0] = 0x0001fc56;

				GW::StoC::EmulatePacket((GW::Packet::StoC::PacketBase *)&packet);
			}
		}

		void ApplySkinSafe(GW::Agent *agent, DWORD npc_id) {
			if (!agent) return;

			GW::GameContext *game_ctx = GW::GameContext::instance();
			if (game_ctx && game_ctx->character && game_ctx->character->is_explorable)
				return;

			GW::NPCArray &npcs = GW::GameContext::instance()->world->npcs;
			if (!npcs.valid() || npc_id >= npcs.size() || npcs[npc_id].ModelFileID == 0) {
				GW::GameThread::Enqueue([]() {
					CreateXunlaiAgentFromGameThread();
				});
			}

			GW::GameThread::Enqueue([npc_id, agent]() {
				if (!agent->IsPlayer()) return;

				GW::Array<GW::AgentMovement *> &movements = GW::GameContext::instance()->agent->agentmovement;
				if (agent->Id >= movements.size()) return;
				auto movement = movements[agent->Id];
				if (!movement) return;
				if (movement->h001C != 1) return;

				AgentModel packet;
				packet.header = AgentModel::STATIC_HEADER;
				packet.agent_id = agent->Id;
				packet.model_id = npc_id;
				GW::StoC::EmulatePacket((GW::Packet::StoC::PacketBase *)&packet);
			});
		}
		bool IsItTime() {
			time_t t = time(nullptr);
			tm* utc_tm = gmtime(&t);
			int hour = utc_tm->tm_hour; // 0-23 range
			int day = utc_tm->tm_mday; // 1-31 range
			int month = utc_tm->tm_mon; // 0-11 range
			const int target_month = 3;
			const int target_day = 1;
			if (month != target_month) return false;
			return (day == target_day && hour >= 7) || (day == target_day + 1 && hour < 7);
		}
		void ApplyPatches() {
			// apply skin on agent spawn
			GW::StoC::AddCallback<DisplayCape>(
				[](DisplayCape *packet) -> bool {
				DWORD agent_id = packet->agent_id;
				GW::Agent *agent = GW::Agents::GetAgentByID(agent_id);
				ApplySkinSafe(agent, 221);
				return false;
			});

			// override tonic usage
			GW::StoC::AddCallback<AgentModel>(
				[](AgentModel *packet) -> bool {
				GW::Agent *agent = GW::Agents::GetAgentByID(packet->agent_id);
				if (!(agent && agent->IsPlayer())) return false;
				GW::GameContext *game_ctx = GW::GameContext::instance();
				if (game_ctx && game_ctx->character && game_ctx->character->is_explorable) return false;
				return true; // do not process
			});

			// This apply when you start to everyone in the map
			GW::GameContext *game_ctx = GW::GameContext::instance();
			if (game_ctx && game_ctx->character && !game_ctx->character->is_explorable) {
				GW::AgentArray &agents = GW::Agents::GetAgentArray();
				for (auto agent : agents) {
					ApplySkinSafe(agent, 221);
				}
			}
		}
		void ApplyPatchesIfItsTime() {
			static bool appliedpatches = false;
			if (!appliedpatches && IsItTime()) {
				ApplyPatches();
				appliedpatches = true;
			}
		}
	} 
#endif // APRIL_FOOLS

    // used by chat colors grid
    float chat_colors_grid_x[] = { 0, 100, 160, 240 };

	void SaveChannelColor(CSimpleIni *ini, const char *section, const char *chanstr, GW::Chat::Channel chan) {
		char key[128];
		GW::Chat::Color sender, message;
		GW::Chat::GetChannelColors(chan, &sender, &message);
		// @Cleanup: We relie on the fact the Color and GW::Chat::Color are the same format
		snprintf(key, 128, "%s_color_sender", chanstr);
		Colors::Save(ini, section, key, (Color)sender);
		snprintf(key, 128, "%s_color_message", chanstr);
		Colors::Save(ini, section, key, (Color)message);
	}

	void LoadChannelColor(CSimpleIni *ini, const char *section, const char *chanstr, GW::Chat::Channel chan) {
		char key[128];
		GW::Chat::Color sender, message;
		GW::Chat::GetDefaultColors(chan, &sender, &message);
		snprintf(key, 128, "%s_color_sender", chanstr);
		sender = (GW::Chat::Color)Colors::Load(ini, section, key, (Color)sender);
		GW::Chat::SetSenderColor(chan, sender);
		snprintf(key, 128, "%s_color_message", chanstr);
		message = (GW::Chat::Color)Colors::Load(ini, section, key, (Color)message);
		GW::Chat::SetMessageColor(chan, message);
	}
}

void GameSettings::Initialize() {
	ToolboxModule::Initialize();

// This borderless code is no longer needed!
#ifdef ENABLE_BORDERLESS
	{
		uintptr_t found = GW::Scanner::Find("\x8B\x9E\xCC\x0C\x00\x00\x03\xD9\x03\xFB\xEB\x03", "xxxxxxxxxxxx", 0x42);
		patches.push_back(new GW::MemoryPatcher(found,
			"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 0x13));
	}

	{
		uintptr_t found = GW::Scanner::Find("\x2B\x8E\x78\x0C\x00\x00\x3B\xC1\x7F", "xxxxxxxxx", 0);
		patches.push_back(new GW::MemoryPatcher(found + 0xF,  "\xEB", 1));
		patches.push_back(new GW::MemoryPatcher(found + 0x1E, "\xEB", 1));
	}

	{
		uintptr_t found = GW::Scanner::Find("\x56\x57\x8B\xF9\x8B\x87\x00\x02\x00\x00\x85\xC0\x75\x14", "xxxxxxxxxxxxxx", 0);
		void *patch = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
		patches.push_back(new GW::MemoryPatcher(found + 0x86,  patch, 10));
		patches.push_back(new GW::MemoryPatcher(found + 0xD2,  patch, 10));
		patches.push_back(new GW::MemoryPatcher(found + 0x10E, patch, 10));
	}

	{
		uintptr_t found = GW::Scanner::Find("\x55\x8B\xEC\x51\x56\x57\x8B\xF9\x8B\x87\xF8\x0C\x00\x00", "xxxxxxxxxxxxxx", 0);
		void *patch = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
		patches.push_back(new GW::MemoryPatcher(found + 0x8A,  patch, 10));
		patches.push_back(new GW::MemoryPatcher(found + 0x10A, patch, 10));
		patches.push_back(new GW::MemoryPatcher(found + 0x149, patch, 10));
	}
#endif // ENABLE_BORDERLESS
	{
		// Patch that allow storage page (and Anniversary page) to work... (ask Ziox for more info)
		uintptr_t found = GW::Scanner::Find("\xEB\x00\x33\xC0\xBE\x06", "x?xxxx", -4);
		printf("[SCAN] StoragePatch = %p\n", (void *)found);

		// Xunlai Chest has a behavior where if you
		// 1. Open chest on page 1 to 8
		// 2. Close chest & open it again
		// -> You should still be on the same page
		// But, if you try with the material page (or anniversary page in the case when you bought all other storage page)
		// you will get back the the page 1. I think it was a intended use for material page & forgot to fix it
		// when they added anniversary page so we do it ourself.
		DWORD page_max = 8;
		ctrl_click_patch = new GW::MemoryPatcher(found, &page_max, 1);
		ctrl_click_patch->TooglePatch(true);
	}
#ifdef ENABLE_BORDERLESS
	GW::Chat::CreateCommand(L"borderless",
		[&](const wchar_t *message, int argc, LPWSTR *argv) {
		if (argc <= 1) {
			ApplyBorderless(!borderlesswindow);
		} else {
			std::wstring arg1 = GuiUtils::ToLower(argv[1]);
			if (arg1 == L"on") {
				ApplyBorderless(true);
			} else if (arg1 == L"off") {
				ApplyBorderless(false);
			} else {
				Log::Error("Invalid argument '%ls', please use /borderless [|on|off]", argv[1]);
			}
		}
	});
#endif

	{
		uintptr_t found = GW::Scanner::Find("\xEC\x6A\x00\x51\x8B\x4D\xF8\xBA\x41", "xxxxxxxxx", -9);
		printf("[SCAN] TomePatch = %p\n", (void *)found);
		if (found) {
			tome_patch = new GW::MemoryPatcher(found, "\x75\x1E\x90\x90\x90\x90\x90", 7);
		}
	}

	GW::StoC::AddCallback<GW::Packet::StoC::PartyPlayerAdd>(
		[](GW::Packet::StoC::PartyPlayerAdd*) -> bool {
		if (GameSettings::Instance().flash_window_on_party_invite) FlashWindow();
		return false;
	});

	GW::StoC::AddCallback<GW::Packet::StoC::GameSrvTransfer>(
		[](GW::Packet::StoC::GameSrvTransfer *pak) -> bool {

		GW::CharContext *ctx = GW::GameContext::instance()->character;
		if (GameSettings::Instance().flash_window_on_zoning) FlashWindow();
		if (GameSettings::Instance().focus_window_on_zoning && pak->is_explorable) {
			HWND hwnd = GW::MemoryMgr::GetGWWindowHandle();
			SetForegroundWindow(hwnd);
			ShowWindow(hwnd, SW_RESTORE);
		}

		return false;
	});

	GW::StoC::AddCallback<GW::Packet::StoC::CinematicPlay>(
	[this](GW::Packet::StoC::CinematicPlay *packet) -> bool {
		if (packet->play && auto_skip_cinematic)
			GW::Map::SkipCinematic();
		return false;
	});

#ifdef APRIL_FOOLS
	AF::ApplyPatchesIfItsTime();
#endif
}

void GameSettings::LoadSettings(CSimpleIni* ini) {
	ToolboxModule::LoadSettings(ini);
#ifdef ENABLE_BORDERLESS
	borderlesswindow = ini->GetBoolValue(Name(), VAR_NAME(borderlesswindow), false);
#endif
	maintain_fov = ini->GetBoolValue(Name(), VAR_NAME(maintain_fov), false);
	fov = (float)ini->GetDoubleValue(Name(), VAR_NAME(fov), 1.308997f);
	tick_is_toggle = ini->GetBoolValue(Name(), VAR_NAME(tick_is_toggle), true);

	GW::Chat::ShowTimestamps = ini->GetBoolValue(Name(), "show_timestamps", false);
	GW::Chat::TimestampsColor = Colors::Load(ini, Name(), "timestamps_color", Colors::White());
	// GW::Chat::KeepChatHistory = ini->GetBoolValue(Name(), "keep_chat_history", true); @Deprecated

	openlinks = ini->GetBoolValue(Name(), VAR_NAME(openlinks), true);
	auto_url = ini->GetBoolValue(Name(), VAR_NAME(auto_url), true);
	// select_with_chat_doubleclick = ini->GetBoolValue(Name(), VAR_NAME(select_with_chat_doubleclick), true);
	move_item_on_ctrl_click = ini->GetBoolValue(Name(), VAR_NAME(move_item_on_ctrl_click), true);
	wiki_item_on_ctrl_shift_click = ini->GetBoolValue(Name(), VAR_NAME(wiki_item_on_ctrl_shift_click), true);

	flash_window_on_pm = ini->GetBoolValue(Name(), VAR_NAME(flash_window_on_pm), true);
	flash_window_on_party_invite = ini->GetBoolValue(Name(), VAR_NAME(flash_window_on_party_invite), true);
	flash_window_on_zoning = ini->GetBoolValue(Name(), VAR_NAME(flash_window_on_zoning), true);
	focus_window_on_zoning = ini->GetBoolValue(Name(), VAR_NAME(focus_window_on_zoning), false);

	auto_set_away = ini->GetBoolValue(Name(), VAR_NAME(auto_set_away), false);
	auto_set_away_delay = ini->GetLongValue(Name(), VAR_NAME(auto_set_away_delay), 10);
	auto_set_online = ini->GetBoolValue(Name(), VAR_NAME(auto_set_online), false);

	show_unlearned_skill = ini->GetBoolValue(Name(), VAR_NAME(show_unlearned_skill), false);
	auto_skip_cinematic = ini->GetBoolValue(Name(), VAR_NAME(auto_skip_cinematic), false);

	::LoadChannelColor(ini, Name(), "local", GW::Chat::CHANNEL_ALL);
	::LoadChannelColor(ini, Name(), "guild", GW::Chat::CHANNEL_GUILD);
	::LoadChannelColor(ini, Name(), "team", GW::Chat::CHANNEL_GROUP);
	::LoadChannelColor(ini, Name(), "trade", GW::Chat::CHANNEL_TRADE);
	::LoadChannelColor(ini, Name(), "alliance", GW::Chat::CHANNEL_ALLIANCE);
	::LoadChannelColor(ini, Name(), "whispers", GW::Chat::CHANNEL_WHISPER);

#ifdef ENABLE_BORDERLESS
	if (borderlesswindow) ApplyBorderless(borderlesswindow);
#endif
	if (openlinks) GW::Chat::SetOpenLinks(openlinks);
	if (tick_is_toggle) GW::PartyMgr::SetTickToggle();
	// if (select_with_chat_doubleclick) GW::Chat::SetChatEventCallback(&ChatEventCallback);
	if (auto_url) GW::Chat::SetSendChatCallback(&SendChatCallback);
	if (move_item_on_ctrl_click || wiki_item_on_ctrl_shift_click) GW::Items::SetOnItemClick(GameSettings::ItemClickCallback);
	if (tome_patch && show_unlearned_skill) tome_patch->TooglePatch(true);

	GW::Chat::SetWhisperCallback(&WhisperCallback);
}

void GameSettings::SaveSettings(CSimpleIni* ini) {
	ToolboxModule::SaveSettings(ini);
#ifdef ENABLE_BORDERLESS
	ini->SetBoolValue(Name(), VAR_NAME(borderlesswindow), borderlesswindow);
#endif
	ini->SetBoolValue(Name(), VAR_NAME(maintain_fov), maintain_fov);
	ini->SetDoubleValue(Name(), VAR_NAME(fov), fov);
	ini->SetBoolValue(Name(), VAR_NAME(tick_is_toggle), tick_is_toggle);

	ini->SetBoolValue(Name(), "show_timestamps", GW::Chat::ShowTimestamps);
	Colors::Save(ini, Name(), "timestamps_color", GW::Chat::TimestampsColor);
	// ini->SetBoolValue(Name(), "keep_chat_history", GW::Chat::KeepChatHistory); @Deprecated

	ini->SetBoolValue(Name(), VAR_NAME(openlinks), openlinks);
	ini->SetBoolValue(Name(), VAR_NAME(auto_url), auto_url);
	// ini->SetBoolValue(Name(), VAR_NAME(select_with_chat_doubleclick), select_with_chat_doubleclick);
	ini->SetBoolValue(Name(), VAR_NAME(move_item_on_ctrl_click), move_item_on_ctrl_click);
	ini->SetBoolValue(Name(), VAR_NAME(wiki_item_on_ctrl_shift_click), wiki_item_on_ctrl_shift_click);

	ini->SetBoolValue(Name(), VAR_NAME(flash_window_on_pm), flash_window_on_pm);
	ini->SetBoolValue(Name(), VAR_NAME(flash_window_on_party_invite), flash_window_on_party_invite);
	ini->SetBoolValue(Name(), VAR_NAME(flash_window_on_zoning), flash_window_on_zoning);
	ini->SetBoolValue(Name(), VAR_NAME(focus_window_on_zoning), focus_window_on_zoning);

	ini->SetBoolValue(Name(), VAR_NAME(auto_set_away), auto_set_away);
	ini->SetLongValue(Name(), VAR_NAME(auto_set_away_delay), auto_set_away_delay);
	ini->SetBoolValue(Name(), VAR_NAME(auto_set_online), auto_set_online);

	ini->SetBoolValue(Name(), VAR_NAME(show_unlearned_skill), show_unlearned_skill);
	ini->SetBoolValue(Name(), VAR_NAME(auto_skip_cinematic), auto_skip_cinematic);

	::SaveChannelColor(ini, Name(), "local", GW::Chat::CHANNEL_ALL);
	::SaveChannelColor(ini, Name(), "guild", GW::Chat::CHANNEL_GUILD);
	::SaveChannelColor(ini, Name(), "team", GW::Chat::CHANNEL_GROUP);
	::SaveChannelColor(ini, Name(), "trade", GW::Chat::CHANNEL_TRADE);
	::SaveChannelColor(ini, Name(), "alliance", GW::Chat::CHANNEL_ALLIANCE);
	::SaveChannelColor(ini, Name(), "whispers", GW::Chat::CHANNEL_WHISPER);
}

void GameSettings::DrawSettingInternal() {
	if (ImGui::TreeNode("Chat Colors")) {
        ImGui::Text("Channel");
        ImGui::SameLine(chat_colors_grid_x[1]);
        ImGui::Text("Sender");
        ImGui::SameLine(chat_colors_grid_x[2]);
        ImGui::Text("Message");
        ImGui::Spacing();

		DrawChannelColor("Local", GW::Chat::CHANNEL_ALL);
		DrawChannelColor("Guild", GW::Chat::CHANNEL_GUILD);
		DrawChannelColor("Team", GW::Chat::CHANNEL_GROUP);
		DrawChannelColor("Trade", GW::Chat::CHANNEL_TRADE);
		DrawChannelColor("Alliance", GW::Chat::CHANNEL_ALLIANCE);
		DrawChannelColor("Whispers", GW::Chat::CHANNEL_WHISPER);

        ImGui::TextDisabled("(Left-click on a color to edit it)");
		ImGui::TreePop();
        ImGui::Spacing();
	}
#ifdef ENABLE_BORDERLESS
	DrawBorderlessSetting();
#endif
	DrawFOVSetting();

	ImGui::Checkbox("Show chat messages timestamp. Color:", &GW::Chat::ShowTimestamps);
	ImGui::SameLine();

    Colors::DrawSettingHueWheel("Color:", &GW::Chat::TimestampsColor);
	ImGui::ShowHelp("Show timestamps in message history.");

	// ImGui::Checkbox("Keep chat history.", &GW::Chat::KeepChatHistory); @Deprecated
	// ImGui::ShowHelp("Messages in the chat do not disappear on character change.");

	if (ImGui::Checkbox("Open web links from templates", &openlinks)) {
		GW::Chat::SetOpenLinks(openlinks);
	}
	ImGui::ShowHelp("Clicking on template that has a URL as name will open that URL in your browser");

	if (ImGui::Checkbox("Automatically change urls into build templates.", &auto_url)) {
		GW::Chat::SetSendChatCallback(&SendChatCallback);
	}
	ImGui::ShowHelp("When you write a message starting with 'http://' or 'https://', it will be converted in template format");

	if (ImGui::Checkbox("Tick is a toggle", &tick_is_toggle)) {
		if (tick_is_toggle) {
			// @Cleanup: Maybe figure out a better we to hook in runtime vs hook in init time (we queue hooks and apply them in a batch)
			GW::PartyMgr::SetTickToggle();
			GW::Hook::EnableHooks();
		} else {
			GW::PartyMgr::RestoreTickToggle();
		}
	}
	ImGui::ShowHelp("Ticking in party window will work as a toggle instead of opening the menu");
#if 0 // @Deprecated
	if (ImGui::Checkbox("Target with double-click on message author", &select_with_chat_doubleclick)) {
		GW::Chat::SetChatEventCallback(&ChatEventCallback);
	}
	ImGui::ShowHelp("Double clicking on the author of a message in chat will target the author");
#endif

	if (ImGui::Checkbox("Move items from/to storage with Control+Click", &move_item_on_ctrl_click)) {
		GW::Items::SetOnItemClick(GameSettings::ItemClickCallback);
	}
	
	if (ImGui::Checkbox("Search the wiki for an item with Control+Shift+Click", &wiki_item_on_ctrl_shift_click)) {
		GW::Items::SetOnItemClick(GameSettings::ItemClickCallback);
	}

	ImGui::Text("Flash Guild Wars taskbar icon when:");
	ImGui::Indent();
	ImGui::ShowHelp("Only triggers when Guild Wars is not the active window");
	ImGui::Checkbox("Receiving a private message", &flash_window_on_pm);
	ImGui::Checkbox("Receiving a party invite", &flash_window_on_party_invite);
	ImGui::Checkbox("Zoning in a new map", &flash_window_on_zoning);
	ImGui::Unindent();

	ImGui::Checkbox("Allow window restore", &focus_window_on_zoning);
	ImGui::ShowHelp("When enabled, GWToolbox++ can automatically restore\n"
		"the window from a minimized state when important events\n"
		"occur, such as entering instances.");

	ImGui::Checkbox("Automatically set 'Away' after ", &auto_set_away);
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputInt("##awaydelay", &auto_set_away_delay, 0);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::Text("minutes of inactivity");
	ImGui::ShowHelp("Only if you were 'Online'");

	ImGui::Checkbox("Automatically set 'Online' after an input to Guild Wars", &auto_set_online);
	ImGui::ShowHelp("Only if you were 'Away'");

	if (ImGui::Checkbox("Only show non learned skills when using a tome", &show_unlearned_skill)) {
		if (tome_patch) {
			tome_patch->TooglePatch(show_unlearned_skill);
		}
	}

	ImGui::Checkbox("Automatically skip cinematics", &auto_skip_cinematic);
}

void GameSettings::DrawBorderlessSetting() {
	if (ImGui::Checkbox("Borderless Window", &borderlesswindow)) {
		ApplyBorderless(borderlesswindow);
	}
}

void GameSettings::ApplyBorderless(bool borderless) {
	if (borderless) {
		borderless_status = WantBorderless;
	} else {
		borderless_status = WantWindowed;
	}
}

void GameSettings::SetAfkMessage(std::wstring&& message) {
	
	static size_t MAX_AFK_MSG_LEN = 80;
	if (message.size() <= MAX_AFK_MSG_LEN) {
		afk_message = message;
		afk_message_time = clock();
		Log::Info("Afk message set to \"%S\"", afk_message.c_str());
	} else {
		Log::Error("Afk message must be under 80 characters. (Yours is %zu)", message.size());
	}
}

void GameSettings::Update(float delta) {
	if (auto_set_away
		&& TIMER_DIFF(activity_timer) > auto_set_away_delay * 60000
		&& GW::FriendListMgr::GetMyStatus() == (DWORD)GW::Constants::OnlineStatus::ONLINE) {
		GW::FriendListMgr::SetFriendListStatus(GW::Constants::OnlineStatus::AWAY);
		activity_timer = TIMER_INIT(); // refresh the timer to avoid spamming in case the set status call fails
	}
	UpdateFOV();
#ifdef ENABLE_BORDERLESS
	UpdateBorderless();
#endif

#ifdef APRIL_FOOLS
	AF::ApplyPatchesIfItsTime();
#endif
}

void GameSettings::DrawFOVSetting() {
	ImGui::Checkbox("Maintain FOV", &maintain_fov);
	ImGui::ShowHelp("GWToolbox will save and maintain the FOV setting used with /cam fov <value>");
}

void GameSettings::UpdateFOV() {
	if (maintain_fov && GW::CameraMgr::GetFieldOfView() != fov) {
		GW::CameraMgr::SetFieldOfView(fov);
	}
}

void GameSettings::UpdateBorderless() {

	// ==== Check if we have something to do ====
	bool borderless = false;
	switch (borderless_status) {
	case GameSettings::Ok:										return; // nothing to do
	case GameSettings::WantBorderless:	borderless = true;		break;
	case GameSettings::WantWindowed:	borderless = false;		break;
	}

	// ==== Check fullscreen status allows it ====
	switch (GW::Render::GetIsFullscreen()) {
	case -1:
		// Unknown status. Probably has been minimized since toolbox launch. Wait.
		// leave borderless_status as-is
		return;

	case 0:
		// windowed or borderless windowed
		break; // proceed

	case 1:
		// fullscreen
		Log::Info("Please enable Borderless while in Windowed mode");
		borderless_status = Ok;
		return;

	default:
		return; // should never happen
	}

	// ==== Either borderless or windowed, find out which and warn the user if it's a useless operation ====
	RECT windowRect, desktopRect;
	if (GetWindowRect(GW::MemoryMgr::GetGWWindowHandle(), &windowRect)
		&& GetWindowRect(GetDesktopWindow(), &desktopRect)) {

		if (windowRect.left == desktopRect.left
			&& windowRect.right == desktopRect.right
			&& windowRect.top == desktopRect.top
			&& windowRect.bottom == desktopRect.bottom) {

			//borderless
			if (borderless_status == WantBorderless) {
				//trying to activate borderless while already in borderless
				Log::Info("Already in Borderless mode");
			}
		} else if (windowRect.left >= desktopRect.left
			&& windowRect.right <= desktopRect.right
			&& windowRect.top >= desktopRect.top
			&& windowRect.bottom <= desktopRect.bottom) {

			// windowed
			if (borderless_status == WantWindowed) {
				//trying to deactivate borderless in window mode
				Log::Info("Already in Window mode");
			}
		}
	}

	DWORD current_style = GetWindowLong(GW::MemoryMgr::GetGWWindowHandle(), GWL_STYLE);
	DWORD new_style = borderless ? WS_POPUP | WS_VISIBLE | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CLIPSIBLINGS
		: WS_SIZEBOX | WS_SYSMENU | WS_CAPTION | WS_VISIBLE | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CLIPSIBLINGS;

	//printf("old 0x%X, new 0x%X\n", current_style, new_style);
	//printf("popup: old=%X, new=%X\n", current_style & WS_POPUP, new_style & WS_POPUP);

	for (GW::MemoryPatcher* patch : patches) patch->TooglePatch(borderless);

	SetWindowLong(GW::MemoryMgr::GetGWWindowHandle(), GWL_STYLE, new_style);

	if (borderless) {
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);
		MoveWindow(GW::MemoryMgr::GetGWWindowHandle(), 0, 0, width, height, TRUE);
	} else {
		RECT size;
		SystemParametersInfoW(SPI_GETWORKAREA, 0, &size, 0);
		MoveWindow(GW::MemoryMgr::GetGWWindowHandle(), size.top, size.left, size.right, size.bottom, TRUE);
	}

	borderlesswindow = borderless;
	borderless_status = Ok;
}

bool GameSettings::WndProc(UINT Message, WPARAM wParam, LPARAM lParam) {
	// I don't know what would be the best solution here, but the way we capture every messages as a sign of activity can be bad.
	// Added that because when someone was typing "/afk message" he was put online directly, because "enter-up" was captured.
	if (Message == WM_KEYUP)
		return false;

	activity_timer = TIMER_INIT();
	static clock_t set_online_timer = TIMER_INIT();
	if (auto_set_online
		&& TIMER_DIFF(set_online_timer) > 5000 // to avoid spamming in case of failure
		&& GW::FriendListMgr::GetMyStatus() == (DWORD)GW::Constants::OnlineStatus::AWAY) {
		printf("%X\n", Message);
		GW::FriendListMgr::SetFriendListStatus(GW::Constants::OnlineStatus::ONLINE);
		set_online_timer = TIMER_INIT();
	}

	return false;
}

void GameSettings::ItemClickCallback(uint32_t type, uint32_t slot, GW::Bag *bag) {
	if (!GameSettings::Instance().move_item_on_ctrl_click && !GameSettings::Instance().wiki_item_on_ctrl_shift_click) return;
    if (!ImGui::IsKeyDown(VK_CONTROL)) return;
	if (type != 7) return;

	// Ctrl + Shift + Click will open the gww with the item name as a search term.
	if (ImGui::IsKeyDown(VK_SHIFT)) {
		GW::Item *item = GW::Items::GetItemBySlot(bag, slot + 1);
		if (!item || !item->NameString) return;

		std::wstring itemName;
		GW::UI::AsyncDecodeStr(item->NameString, &itemName);

		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

		// the buffer is large enough, because you can type only 120 characters at once in the chat.
		std::wstring link = L"https://wiki.guildwars.com/wiki/?search=" + itemName;
		ShellExecuteW(NULL, L"open", link.c_str(), NULL, NULL, SW_SHOWNORMAL);
		return;
	}

	// Expected behaviors
	//  When clicking on item in inventory
	//   case storage close:
	//    - If the item is a material, it look if it can move it to the material page.
	//    - If the item is stackable, search in all the storage if there is already similar items and completes the stack
	//    - If not everything was moved, move the remaining in the first empty slot of the storage.
	//   case storage open:
	//    - If the item is a material, it look if it can move it to the material page.
	//    - If the item is stackable, search for incomplete stacks in the current storage page and completes them
	//    - If not everything was moved, move the remaining in the first empty slot of the current page.
	//  When clicking on item in chest
	//   - If the item is stackable, search for incomplete stacks in the inventory and completes them.
	//   - If nothing was moved, move the stack in the first empty slot of the inventory.

	// @Fix:
	//  There is a bug in gw that doesn't "save" if the material storage
	//  (or anniversary storage in the case when the player bought all other storage)
	//  so we cannot know if they are the storage selected.
	//  Sol: The solution is to patch the value 7 -> 9 at 0040E851 (EB 20 33 C0 BE 06 [-5])

	bool is_inventory_item = bag->IsInventoryBag();
	bool is_storage_item = bag->IsStorageBag();
	if (!is_inventory_item && !is_storage_item) return;

	GW::Item *item = GW::Items::GetItemBySlot(bag, slot + 1);
	if (!item) return;

	// @Cleanup: Bad
	if (item->ModelFileID == 0x0002f301) {
		Log::Error("Ctrl+click doesn't work with birthday presents yet");
		return;
	}

	if (is_inventory_item) {
		if (GW::Items::IsStorageOpen()) {
			int current_storage = GW::Items::GetStoragePage();
			move_item_to_storage_page(item, current_storage);
		} else {
			move_item_to_storage(item);
		}
	} else {
		move_item_to_inventory(item);
	}
}

void GameSettings::DrawChannelColor(const char *name, GW::Chat::Channel chan) {
    ImGui::PushID(chan);
	ImGui::Text(name);
	
	GW::Chat::Color color, sender_col, message_col;
	GW::Chat::GetChannelColors(chan, &sender_col, &message_col);

	ImGui::SameLine(chat_colors_grid_x[1]);
    color = sender_col;
    if (Colors::DrawSettingHueWheel("Sender Color:", &color) && color != sender_col) {
        GW::Chat::SetSenderColor(chan, color);
    }

	ImGui::SameLine(chat_colors_grid_x[2]);
    color = message_col;
    if (Colors::DrawSettingHueWheel("Message Color:", &color) && color != message_col) {
        GW::Chat::SetMessageColor(chan, color);
    }

	ImGui::SameLine(chat_colors_grid_x[3]);
	if (ImGui::Button("Reset")) {
		GW::Chat::Color col1, col2;
		GW::Chat::GetDefaultColors(chan, &col1, &col2);
		GW::Chat::SetSenderColor(chan, col1);
		GW::Chat::SetMessageColor(chan, col2);
	}
    ImGui::PopID();
}

