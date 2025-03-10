///|/ Copyright (c) Prusa Research 2016 - 2022 Lukáš Hejl @hejllukas, David Kocík @kocikdav, Oleksandra Iushchenko @YuSanka, Vojtěch Bubník @bubnikv, Enrico Turri @enricoturri1966, Lukáš Matěna @lukasmatena, Vojtěch Král @vojtechkral, Tomáš Mészáros @tamasmeszaros
///|/ Copyright (c) Slic3r 2015 Alessandro Ranellucci @alranel
///|/
///|/ PrusaSlicer is released under the terms of the AGPLv3 or higher
///|/
#ifndef slic3r_GUI_hpp_
#define slic3r_GUI_hpp_

namespace boost { class any; }
namespace boost::filesystem { class path; }

#include <wx/string.h>

#include "libslic3r/Config.hpp"
#include "libslic3r/Preset.hpp"

class wxWindow;
class wxMenuBar;
class wxComboCtrl;
class wxFileDialog;
class wxArrayString;
class wxTopLevelWindow;

namespace Slic3r { 

class AppConfig;
class DynamicPrintConfig;
class Print;

namespace GUI {

void disable_screensaver();
void enable_screensaver();
bool debugged();
void break_to_debugger();

// Platform specific Ctrl+/Alt+ (Windows, Linux) vs. ⌘/⌥ (OSX) prefixes 
extern const std::string& shortkey_ctrl_prefix();
extern const std::string& shortkey_alt_prefix();

extern AppConfig* get_app_config();

extern void add_menus(wxMenuBar *menu, int event_preferences_changed, int event_language_change);

// If monospaced_font is true, the error message is displayed using html <code><pre></pre></code> tags,
// so that the code formatting will be preserved. This is useful for reporting errors from the placeholder parser.
void show_error(wxWindow* parent, const wxString& message, bool monospaced_font = false);
void show_error(wxWindow* parent, const char* message, bool monospaced_font = false);
inline void show_error(wxWindow* parent, const std::string& message, bool monospaced_font = false) { show_error(parent, message.c_str(), monospaced_font); }
void show_info(wxWindow* parent, const wxString& message, const wxString& title = wxString());
void show_info(wxWindow* parent, const char* message, const char* title = nullptr);
inline void show_info(wxWindow* parent, const std::string& message,const std::string& title = std::string()) { show_info(parent, message.c_str(), title.c_str()); }
void warning_catcher(wxWindow* parent, const wxString& message);
void show_substitutions_info(const PresetsConfigSubstitutions& presets_config_substitutions);
void show_substitutions_info(const ConfigSubstitutions& config_substitutions, const std::string& filename);

// Creates a wxCheckListBoxComboPopup inside the given wxComboCtrl, filled with the given text and items.
// Items data must be separated by '|', and contain the item name to be shown followed by its initial value (0 for false, 1 for true).
// For example "Item1|0|Item2|1|Item3|0", and so on.
void create_combochecklist(wxComboCtrl* comboCtrl, const std::string& text, const std::string& items);

// Returns the current state of the items listed in the wxCheckListBoxComboPopup contained in the given wxComboCtrl,
// encoded inside an unsigned int.
unsigned int combochecklist_get_flags(wxComboCtrl* comboCtrl);

// Sets the current state of the items listed in the wxCheckListBoxComboPopup contained in the given wxComboCtrl,
// with the flags encoded in the given unsigned int.
void combochecklist_set_flags(wxComboCtrl* comboCtrl, unsigned int flags);

// wxString conversions:

// wxString from std::string in UTF8
wxString	from_u8(const std::string &str);
// std::string in UTF8 from wxString
std::string	into_u8(const wxString &str);
// wxString from boost path
wxString	from_path(const boost::filesystem::path &path);
// boost path from wxString
boost::filesystem::path	into_path(const wxString &str);

// Display an About dialog
extern void about();
// Ask the destop to open the datadir using the default file explorer.
extern void desktop_open_datadir_folder();
// Ask the destop to open the directory specified by path using the default file explorer.
void desktop_open_folder(const boost::filesystem::path& path);

#ifdef __linux__
// Calling wxExecute on Linux with proper handling of AppImage's env vars.
// argv example: { "xdg-open", path.c_str(), nullptr }
void desktop_execute(const char* argv[]);
void desktop_execute_get_result(wxString command, wxArrayString& output);
#endif // __linux__

#ifdef _WIN32
// Call CreateProcessW to start external proccess on path
// returns true on success
// path should contain path to the process
// cmd_opt can be empty or contain command line options. Example: L"/silent"
// error_msg will contain error message if create_process return false
bool create_process(const boost::filesystem::path& path, const std::wstring& cmd_opt, std::string& error_msg);
#endif //_WIN32

} // namespace GUI
} // namespace Slic3r

#endif
