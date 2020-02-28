#include "stdafx.h"
#include "TradeWindow.h"

#include <WinSock2.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <GWCA\Constants\Constants.h>
#include <GWCA\GameContainers\Array.h>

#include <GWCA\Managers\UIMgr.h>
#include <GWCA\Managers\MapMgr.h>
#include <GWCA\Managers\ChatMgr.h>
#include <GWCA\Managers\GameThreadMgr.h>

#include <Modules\Resources.h>

#include "logger.h"
#include "GuiUtils.h"
#include "GWToolbox.h"

// Every connection cost 30 seconds.
// You have 2 tries.
// After that, you can try every 30 seconds.
static const uint32_t COST_PER_CONNECTION_MS 	 = 30 * 1000;
static const uint32_t COST_PER_CONNECTION_MAX_MS = 60 * 1000;

using easywsclient::WebSocket;
using nlohmann::json;
using json_vec = std::vector<json>;

static const char ws_host[] = "wss://kamadan.gwtoolbox.com";
static const char https_host[] = "https://kamadan.gwtoolbox.com";

void TradeWindow::Initialize() {
	ToolboxWindow::Initialize();
	Resources::Instance().LoadTextureAsync(&button_texture, Resources::GetPath(L"img/icons", L"trade.png"));

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		printf("WSAStartup Failed.\n");
		return;
	}

	messages = CircularBuffer<Message>(100);

	should_stop = false;
	worker = std::thread([this]() {
		while (!should_stop) {
			if (thread_jobs.empty()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			} else {
				thread_jobs.front()();
				thread_jobs.pop();
			}
		}
	});
}

void TradeWindow::Terminate() {
	should_stop = true;
	if (worker.joinable())
		worker.join();

	if (ws_window) {
		DeleteWebSocket(ws_window);
		ws_window = nullptr;
	}

	WSACleanup();
	ToolboxWindow::Terminate();
}

TradeWindow::~TradeWindow() {
	Terminate();
}

bool TradeWindow::GetInKamadanAE1() {
	using namespace GW::Constants;
	switch (GW::Map::GetMapID()) {
	case MapID::Kamadan_Jewel_of_Istan_outpost:
	case MapID::Kamadan_Jewel_of_Istan_Halloween_outpost:
	case MapID::Kamadan_Jewel_of_Istan_Wintersday_outpost:
	case MapID::Kamadan_Jewel_of_Istan_Canthan_New_Year_outpost:
		return GW::Map::GetDistrict() == 1 && GW::Map::GetRegion() == GW::Constants::Region::America;
	default:
		return false;
	}
}

void TradeWindow::Update(float delta) {
	if (ws_window && ws_window->getReadyState() == WebSocket::CLOSED) {
		delete ws_window;
		ws_window = nullptr;
	}
	bool maintain_socket = visible || print_game_chat;
	if (maintain_socket && !ws_window) {
		AsyncWindowConnect();
	}
	if (!maintain_socket && ws_window) {
		ws_window->close();
	}
	fetch();
}

bool TradeWindow::parse_json_message(json* js, Message* msg) {
	try {
		msg->name = js->at("s").get<std::string>();
		msg->message = js->at("m").get<std::string>();
		msg->timestamp = js->at("t").get<uint64_t>() / 1000; // Messy?
	}
	catch (...) {
		Log::Log("ERROR: Failed to parse incoming trade message in TradeWindow::parse_json_message\n");
		return false;
	}
	return true;
}

void TradeWindow::fetch() {
	if (!ws_window || ws_window->getReadyState() != WebSocket::OPEN) 
		return;
	
	ws_window->poll();
	ws_window->dispatch([this](const std::string& data) {
		json res;
		try {
			res = json::parse(data.c_str());
		}
		catch (...) {
			Log::Log("ERROR: Failed to parse res JSON from response in ws_window->dispatch\n");
			return;
		}
		if (res.find("query") == res.end()) {
			// It's a new message
			TradeWindow::Message msg;
			if (parse_json_message(&res, &msg))
				messages.add(msg);

			// Check alerts
			// do not display trade chat while in kamadan AE district 1
			bool print_message = print_game_chat && !GetInKamadanAE1();

			if (print_message && filter_alerts) {
				print_message = false; // filtered unless allowed by words
				for (auto& word : alert_words) {
					auto found = std::search(msg.message.begin(), msg.message.end(), word.begin(), word.end(), [](char c1, char c2) -> bool {
						return tolower(c1) == c2;
						});
					if (found != msg.message.end()) {
						print_message = true;
						break; // don't need to check other words
					}
				}
			}

			if (print_message) {
				char buffer[512];
				snprintf(buffer, sizeof(buffer), "<a=1>%s</a>: <c=#f96677><quote>%s", msg.name.c_str(), msg.message.c_str());
				GW::Chat::WriteChat(GW::Chat::CHANNEL_TRADE, buffer);
			}
		} else {
			search_pending = false;
			json_vec results;
			try {
				if(res["num_results"].get<uint32_t>() > 0)
					results = res["results"].get<json_vec>();
			}
			catch (...) {
				Log::Log("ERROR: Failed to parse search results in TradeWindow::fetch\n");
				return;
			}
			messages.clear();
			for (int i = results.size() - 1; i >= 0; i--) {
				TradeWindow::Message msg;
				if (parse_json_message(&results[i], &msg))
					messages.add(msg);
			}
		}
	});
}

void TradeWindow::search(std::string query) {
	if (!ws_window || ws_window->getReadyState() != WebSocket::OPEN)
		return;

	// for now we won't allow to enqueue more than 1 search, it shouldn't change anything because how fast we get the answers
	if (search_pending)
		return;

	search_pending = true;

	/*
	 * The protocol is the following:
	 *  - From a connected web socket, we send a Json formated packet with the format
	 *  {
	 *   "query":  str($query$),
	 *   "offset": int($page$),
	 *   "sugest": int(1 or 2)
	 *  }
	 */

	json request;
	request["query"] = query;
	ws_window->send(request.dump());
}

void TradeWindow::Draw(IDirect3DDevice9* device) {


	// start the trade_searcher if its not active
	// if (!trade_searcher->is_active() && !trade_searcher->is_timed_out()) trade_searcher->search("");
	ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(700, 400), ImGuiSetCond_FirstUseEver);
	if (ImGui::Begin(Name(), GetVisiblePtr(), GetWinFlags())) {
		/* Search bar header */
		static char search_buffer[256];
		ImGui::PushItemWidth((ImGui::GetWindowContentRegionWidth() - 80.0f - 80.0f - 80.0f - ImGui::GetStyle().ItemInnerSpacing.x * 6));
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
		if (search_pending) {
			flags |= ImGuiInputTextFlags_ReadOnly;
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		}
		bool do_search = false;
		do_search |= ImGui::InputText("", search_buffer, 256, flags);
		ImGui::SameLine();
		do_search |= ImGui::Button(search_pending ? "Searching" : "Search", ImVec2(80.0f, 0));
		if (search_pending) {
			ImGui::PopStyleColor();
		}
		else if(do_search) {
			search(search_buffer);
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear", ImVec2(80.0f, 0))) {
			GuiUtils::StrCopy(search_buffer, "", 256);
			search("");
		}
		ImGui::SameLine();
		if (ImGui::Button("Alerts", ImVec2(80.0f, 0))) {
			show_alert_window = !show_alert_window;
		}

		/* Main trade chat area */
		ImGui::BeginChild("trade_scroll", ImVec2(0, -20.0f - ImGui::GetStyle().ItemInnerSpacing.y));
		/* Connection checks */
		if (!ws_window && !ws_window_connecting) {
			char buf[255];
			snprintf(buf, 255, "The connection to %s has timed out.", ws_host);
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(buf).x) / 2);
			ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2);
			ImGui::Text(buf);
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Click to reconnect").x) / 2);
			if (ImGui::Button("Click to reconnect")) {
				AsyncWindowConnect(true);
			}
		} else if (ws_window_connecting || (ws_window && ws_window->getReadyState() == WebSocket::CONNECTING)) {
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Connecting...").x)/2);
			ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2);
			ImGui::Text("Connecting...");
		} else {
			/* Display trade messages */
			bool show_time = ImGui::GetWindowWidth() > 600.0f;

			char timetext[128];
			time_t now = time(nullptr);

			const float innerspacing = ImGui::GetStyle().ItemInnerSpacing.x;
			const float time_width = show_time ? 100.0f : 0.0f;
			const float playername_left = time_width + innerspacing; // player button left align
			const float playernamewidth = 160.0f;
			const float message_left = playername_left + playernamewidth + innerspacing;

			for (int i = messages.size() - 1; i >= 0; i--) {
				Message &msg = messages[i];
				ImGui::PushID(i);

				// ==== time elapsed column ====
				if (show_time) {
					// negative numbers have came from this before, it is probably just server client desync
					int time_since_message = (int)now - msg.timestamp;

					ImGui::PushFont(GuiUtils::GetFont(GuiUtils::FontSize::f16));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.7f, .7f, .7f, 1.0f));

					// decide if days, hours, minutes, seconds...
					if ((int)(time_since_message / (60 * 60 * 24))) {
						int days = (int)(time_since_message / (60 * 60 * 24));
						_snprintf(timetext, 128, "%d %s ago", days, days > 1 ? "days" : "day");
					} else if ((int)(time_since_message / (60 * 60))) {
						int hours = (int)(time_since_message / (60 * 60));
						_snprintf(timetext, 128, "%d %s ago", hours, hours > 1 ? "hours" : "hour");
					} else if ((int)(time_since_message / (60))) {
						int minutes = (int)(time_since_message / 60);
						_snprintf(timetext, 128, "%d %s ago", minutes, minutes > 1 ? "minutes" : "minute");
					} else {
						_snprintf(timetext, 128, "%d %s ago", time_since_message, time_since_message > 1 ? "seconds" : "second");
					}
					ImGui::SetCursorPosX(playername_left - innerspacing - ImGui::CalcTextSize(timetext).x);
					ImGui::Text(timetext);
					ImGui::PopStyleColor();
					ImGui::PopFont();
				}

				// ==== Sender name column ====
				if (show_time) {
					ImGui::SameLine(playername_left);
				}
				if (ImGui::Button(msg.name.c_str(), ImVec2(playernamewidth, 0))) {
					// open whisper to player
					GW::GameThread::Enqueue([&msg]() {
						std::wstring name_ws = GuiUtils::ToWstr(msg.name);
						GW::UI::SendUIMessage(GW::UI::kOpenWhisper, (wchar_t*)name_ws.data(), nullptr);
					});
				}

				// ==== Message column ====
				ImGui::SameLine(message_left);
				ImGui::TextWrapped("%s", msg.message.c_str());
				ImGui::PopID();
			}
		}
		ImGui::EndChild();

		/* Link to website footer */
		static char buf[128];
		if (!buf[0]) {
			snprintf(buf, 128, "Powered by %s", https_host);
		}
		if (ImGui::Button(buf, ImVec2(ImGui::GetWindowContentRegionWidth(), 20.0f))){ 
			CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
			ShellExecuteA(NULL, "open", https_host, NULL, NULL, SW_SHOWNORMAL);
		}

		/* Alerts window */
		if (show_alert_window) {
			ImGui::SetNextWindowSize(ImVec2(250, 220), ImGuiCond_FirstUseEver);
			if (ImGui::Begin("Trade Alerts", &show_alert_window)) {
				DrawAlertsWindowContent(true);
			}
			ImGui::End();
		}
	}
	ImGui::End();
}

void TradeWindow::DrawAlertsWindowContent(bool ownwindow) {
	ImGui::Text("Alerts");
	ImGui::Checkbox("Send Kamadan ad1 trade chat to your trade chat", &print_game_chat);
	ImGui::Checkbox("Only show messages containing:", &filter_alerts);
	ImGui::TextDisabled("(Each line is a separate keyword. Not case sensitive.)");
	if (ImGui::InputTextMultiline("##alertfilter", alert_buf, ALERT_BUF_SIZE, 
		ImVec2(-1.0f, ownwindow ? -1.0f : 0.0f))) {

		ParseBuffer(alert_buf, alert_words);
		alertfile_dirty = true;
	}
}

void TradeWindow::DrawSettingInternal() {
	DrawAlertsWindowContent(false);
}

void TradeWindow::LoadSettings(CSimpleIni* ini) {
	ToolboxWindow::LoadSettings(ini);
	print_game_chat = ini->GetBoolValue(Name(), VAR_NAME(print_game_chat), false);
	filter_alerts   = ini->GetBoolValue(Name(), VAR_NAME(filter_alerts), false);

	std::ifstream alert_file;
	alert_file.open(Resources::GetPath(L"AlertKeywords.txt"));
	if (alert_file.is_open()) {
		alert_file.get(alert_buf, ALERT_BUF_SIZE, '\0');
		alert_file.close();
		ParseBuffer(alert_buf, alert_words);
	}
	alert_file.close();
}


void TradeWindow::SaveSettings(CSimpleIni* ini) {
	ToolboxWindow::SaveSettings(ini);

	ini->SetBoolValue(Name(), VAR_NAME(print_game_chat), print_game_chat);
	ini->SetBoolValue(Name(), VAR_NAME(filter_alerts), filter_alerts);

	if (alertfile_dirty) {
		std::ofstream bycontent_file;
		bycontent_file.open(Resources::GetPath(L"AlertKeywords.txt"));
		if (bycontent_file.is_open()) {
			bycontent_file.write(alert_buf, strlen(alert_buf));
			bycontent_file.close();
			alertfile_dirty = false;
		}
	}
}

void TradeWindow::ParseBuffer(const char *text, std::vector<std::string>& words) {
	words.clear();
	std::istringstream stream(text);
	std::string word;
	while (std::getline(stream, word)) {
		for (size_t i = 0; i < word.length(); i++)
			word[i] = tolower(word[i]);
		words.push_back(word);
	}
}

void TradeWindow::ParseBuffer(std::fstream stream, std::vector<std::string>& words) {
	words.clear();
	std::string word;
	while (std::getline(stream, word)) {
		for (size_t i = 0; i < word.length(); i++)
			word[i] = tolower(word[i]);
		words.push_back(word);
	}
}

void TradeWindow::AsyncWindowConnect(bool force) {
	if (ws_window) return;
	if (ws_window_connecting) return;
	if (!force && !window_rate_limiter.AddTime(COST_PER_CONNECTION_MS, COST_PER_CONNECTION_MAX_MS))
		return;
	ws_window_connecting = true;
	thread_jobs.push([this]() {
		if (!(ws_window = WebSocket::from_url(ws_host))) {
			printf("Couldn't connect to the host '%s'", ws_host);
		}
		ws_window_connecting = false;
		if (messages.size() == 0)
			search(""); // Initial draw, gets latest N messages
	});
}

void TradeWindow::DeleteWebSocket(easywsclient::WebSocket *ws) {
	if (!ws) return;
	if (ws->getReadyState() == easywsclient::WebSocket::OPEN)
		ws->close();
	while (ws->getReadyState() != easywsclient::WebSocket::CLOSED)
		ws->poll();
	delete ws;
}
