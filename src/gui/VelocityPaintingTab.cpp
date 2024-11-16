#include "VelocityPaintingTab.hpp"
#include "VelocityPainting.hpp"
#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>

namespace Slic3r {

VelocityPaintingTab::VelocityPaintingTab(wxWindow* parent) : wxPanel(parent) {
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    // Projection mode
    wxBoxSizer* projection_mode_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* projection_mode_label = new wxStaticText(this, wxID_ANY, wxT("Projection Mode:"));
    projection_mode_sizer->Add(projection_mode_label, 0, wxALL, 5);
    wxArrayString projection_modes;
    projection_modes.Add(wxT("projectX"));
    projection_modes.Add(wxT("projectY"));
    projection_modes.Add(wxT("projectZ"));
    projection_modes.Add(wxT("cylinderZ"));
    projection_modes.Add(wxT("spherical"));
    wxChoice* projection_mode_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, projection_modes);
    projection_mode_sizer->Add(projection_mode_choice, 1, wxALL, 5);
    main_sizer->Add(projection_mode_sizer, 0, wxEXPAND, 5);

    // Center coordinates
    wxBoxSizer* center_coords_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* center_coords_label = new wxStaticText(this, wxID_ANY, wxT("Center Coordinates:"));
    center_coords_sizer->Add(center_coords_label, 0, wxALL, 5);
    wxTextCtrl* center_coords_x = new wxTextCtrl(this, wxID_ANY);
    center_coords_sizer->Add(center_coords_x, 1, wxALL, 5);
    wxTextCtrl* center_coords_y = new wxTextCtrl(this, wxID_ANY);
    center_coords_sizer->Add(center_coords_y, 1, wxALL, 5);
    main_sizer->Add(center_coords_sizer, 0, wxEXPAND, 5);

    // Image dimensions
    wxBoxSizer* image_dims_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* image_dims_label = new wxStaticText(this, wxID_ANY, wxT("Image Dimensions:"));
    image_dims_sizer->Add(image_dims_label, 0, wxALL, 5);
    wxTextCtrl* image_width = new wxTextCtrl(this, wxID_ANY);
    image_dims_sizer->Add(image_width, 1, wxALL, 5);
    wxTextCtrl* image_height = new wxTextCtrl(this, wxID_ANY);
    image_dims_sizer->Add(image_height, 1, wxALL, 5);
    main_sizer->Add(image_dims_sizer, 0, wxEXPAND, 5);

    // Z-offset
    wxBoxSizer* z_offset_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* z_offset_label = new wxStaticText(this, wxID_ANY, wxT("Z-offset:"));
    z_offset_sizer->Add(z_offset_label, 0, wxALL, 5);
    wxTextCtrl* z_offset = new wxTextCtrl(this, wxID_ANY);
    z_offset_sizer->Add(z_offset, 1, wxALL, 5);
    main_sizer->Add(z_offset_sizer, 0, wxEXPAND, 5);

    // Speeds
    wxBoxSizer* speeds_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* min_speed_label = new wxStaticText(this, wxID_ANY, wxT("Min Speed:"));
    speeds_sizer->Add(min_speed_label, 0, wxALL, 5);
    wxTextCtrl* min_speed = new wxTextCtrl(this, wxID_ANY);
    speeds_sizer->Add(min_speed, 1, wxALL, 5);
    wxStaticText* max_speed_label = new wxStaticText(this, wxID_ANY, wxT("Max Speed:"));
    speeds_sizer->Add(max_speed_label, 0, wxALL, 5);
    wxTextCtrl* max_speed = new wxTextCtrl(this, wxID_ANY);
    speeds_sizer->Add(max_speed, 1, wxALL, 5);
    main_sizer->Add(speeds_sizer, 0, wxEXPAND, 5);

    // Extrusion multiplier
    wxBoxSizer* extrusion_multiplier_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* extrusion_multiplier_label = new wxStaticText(this, wxID_ANY, wxT("Extrusion Multiplier:"));
    extrusion_multiplier_sizer->Add(extrusion_multiplier_label, 0, wxALL, 5);
    wxTextCtrl* extrusion_multiplier = new wxTextCtrl(this, wxID_ANY);
    extrusion_multiplier_sizer->Add(extrusion_multiplier, 1, wxALL, 5);
    main_sizer->Add(extrusion_multiplier_sizer, 0, wxEXPAND, 5);

    // Preview button
    wxButton* preview_button = new wxButton(this, wxID_ANY, wxT("Preview"));
    main_sizer->Add(preview_button, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

    SetSizer(main_sizer);
    Layout();
}

void VelocityPaintingTab::on_preview_button_click(wxCommandEvent& event) {
    // Implement the logic to preview the effects on models
}

} // namespace Slic3r
