#pragma once

#include <Defines.h>
#include <ToolboxModule.h>

class ToolboxUIElement : public ToolboxModule
{
public:
    virtual ~ToolboxUIElement()
    {
        //Terminate();
    }

    // Draw user interface. Will be called every frame if the element is visible
    virtual void Draw(IDirect3DDevice9 *) {};

    void Initialize() override;
    void Terminate() override;

    void LoadSettings(CSimpleIni *ini) override;

    void SaveSettings(CSimpleIni *ini) override;

    // returns true if clicked
    virtual bool DrawTabButton(IDirect3DDevice9 *device,
                               bool show_icon = true, bool show_text = true);

    virtual bool ToggleVisible() { return visible = !visible; }

    virtual bool IsWindow() const { return false; }
    virtual bool IsWidget() const { return false; }

    char *TypeName() const override { return "ui element"; }

    void RegisterSettingsContent() override;

    void DrawSizeAndPositionSettings();

    bool visible = false;
    bool lock_move = false;
    bool lock_size = false;
    bool show_menubutton = false;

    bool *GetVisiblePtr(bool force_show = false)
    {
        if (!has_closebutton || show_closebutton || force_show)
            return &visible;
        return nullptr;
    }

    bool show_closebutton = true;
protected:
    bool has_closebutton = false;
    bool is_resizable = true;
    bool is_movable = true;

    virtual void ShowVisibleRadio();
    IDirect3DTexture9 *button_texture = nullptr;
};
