#ifndef EXTRUSION_PAINTING_TAB_HPP
#define EXTRUSION_PAINTING_TAB_HPP

#include <wx/wx.h>

namespace Slic3r {

class ExtrusionPaintingTab : public wxPanel {
public:
    ExtrusionPaintingTab(wxWindow* parent);

private:
    void on_preview_button_click(wxCommandEvent& event);
};

} // namespace Slic3r

#endif // EXTRUSION_PAINTING_TAB_HPP
