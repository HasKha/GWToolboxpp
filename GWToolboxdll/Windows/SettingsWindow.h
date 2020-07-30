#pragma once

#include <ToolboxWindow.h>

class SettingsWindow : public ToolboxWindow {
    SettingsWindow() {
        show_menubutton = true;
    };
    ~SettingsWindow() {};
public:
    static SettingsWindow& Instance() {
        static SettingsWindow instance;
        return instance;
    }

    const char* Name() const override { return "Settings"; }

    void Initialize() override;

    void LoadSettings(CSimpleIni* ini) override;
    void SaveSettings(CSimpleIni* ini) override;

    // Draw user interface. Will be called every frame if the element is visible
    void Draw(IDirect3DDevice9* pDevice) override;

    bool DrawSettingsSection(const char* section) {
        const auto callbacks = ToolboxModule::GetSettingsCallbacks();
        auto settings_section = callbacks->find(section);
        if (settings_section == callbacks->end())
            return false;
        if (drawn_settings.find(section) != drawn_settings.end())
            return true; // Already drawn
        drawn_settings[section] = true;
        const bool &is_showing = ImGui::CollapsingHeader(section, ImGuiTreeNodeFlags_AllowItemOverlap);
        if(is_showing) ImGui::PushID(section);
        size_t i = 0;
        for (auto &entry : settings_section->second) {
            if (i && is_showing) ImGui::Separator();
            entry.second(&settings_section->first, is_showing);
            i++;
        }
        if (is_showing) ImGui::PopID();
        return true;
    }

    size_t sep_modules = 0;
    size_t sep_windows = 0;
    size_t sep_widgets = 0;
private:
    std::map<std::string, bool> drawn_settings{};
    bool hide_when_entering_explorable = false;
};
