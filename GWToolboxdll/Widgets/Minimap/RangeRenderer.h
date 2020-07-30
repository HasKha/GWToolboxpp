#pragma once

#include <Color.h>
#include <Widgets/Minimap/VBuffer.h>

class RangeRenderer : public VBuffer
{
private:
    static constexpr size_t num_circles = 5;
    static constexpr size_t circle_points = 96;
    static constexpr size_t circle_triangles = circle_points - 2;

public:
    void Render(IDirect3DDevice9 *device) override;
    void SetDrawCenter(bool b) { draw_center_ = b; }

    void DrawSettings();
    void LoadSettings(CSimpleIni *ini, const char *section);
    void SaveSettings(CSimpleIni *ini, const char *section) const;

private:
    void CreateCircle(D3DVertex *vertices, float radius, DWORD color) const;
    void Initialize(IDirect3DDevice9 *device) override;

    bool HaveHos();

    bool checkforhos_ = true;
    bool havehos_ = false;

    bool draw_center_ = false;

    float line_thickness = 1.f;

    Color color_range_hos = 0;
    Color color_range_aggro = 0;
    Color color_range_cast = 0;
    Color color_range_spirit = 0;
    Color color_range_compass = 0;
};
