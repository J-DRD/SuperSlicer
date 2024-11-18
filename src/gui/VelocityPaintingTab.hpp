#ifndef VELOCITY_PAINTING_TAB_HPP
#define VELOCITY_PAINTING_TAB_HPP

#include <wx/wx.h>

namespace Slic3r {

class VelocityPaintingTab : public wxPanel {
public:
    VelocityPaintingTab(wxWindow* parent);

private:
    void on_preview_button_click(wxCommandEvent& event);
};

} // namespace Slic3r

#endif // VELOCITY_PAINTING_TAB_HPP
