///|/ Copyright (c) Prusa Research 2017 - 2023 Vojtěch Bubník @bubnikv, Lukáš Matěna @lukasmatena
///|/ Copyright (c) 2017 Joseph Lenox @lordofhyphens
///|/ Copyright (c) Slic3r 2014 - 2015 Alessandro Ranellucci @alranel
///|/
///|/ ported from lib/Slic3r/Extruder.pm:
///|/ Copyright (c) Slic3r 2011 - 2014 Alessandro Ranellucci @alranel
///|/
///|/ PrusaSlicer is released under the terms of the AGPLv3 or higher
///|/
#include "Extruder.hpp"
#include "GCode/GCodeWriter.hpp"
#include "PrintConfig.hpp"

namespace Slic3r {

Tool::Tool(uint16_t id, GCodeConfig &config) :
    m_id(id),
    m_config(&config),
    m_formatter(config.gcode_precision_xyz, config.gcode_precision_e)
{
    //reset();
}

Extruder::Extruder(uint16_t id, GCodeConfig &config) :
    Tool(id, config)
{
    // set extra_toolchange to be init for when it will be new current extruder
    m_restart_extra_toolchange = retract_restart_extra_toolchange();

    // cache values that are going to be called often
    m_e_per_mm3 = this->extrusion_multiplier();
    if (! m_config->use_volumetric_e)
        m_e_per_mm3 /= this->filament_crossection();
}

Mill::Mill(uint16_t mill_id, GCodeConfig &config) :
    Tool(mill_id, config)
{
    m_mill_id = mill_id;
    m_id = mill_id + (uint16_t)config.retract_length.size();
}

std::pair<double, double> Tool::extrude(double dE)
{
    assert(! std::isnan(dE));
    assert(dE < std::numeric_limits<int32_t>::max());
    assert(dE > -std::numeric_limits<int32_t>::max());
    // in case of relative E distances we always reset to 0 before any output
    if (m_config->use_relative_e_distances)
        m_E = 0.;
    // Quantize extruder delta to G-code resolution.
    dE = m_formatter.quantize_e(dE);
    m_E          += dE;
    m_absolute_E += dE;
    if (dE < 0.)
        m_retracted -= dE;
    return std::make_pair(dE, m_E);
}

/* This method makes sure the extruder is retracted by the specified amount
   of filament and returns the amount of filament retracted.
   If the extruder is already retracted by the same or a greater amount, 
   this method is a no-op.
   The restart_extra argument sets the extra length to be used for
   unretraction. If we're actually performing a retraction, any restart_extra
   value supplied will overwrite the previous one if any. */
std::pair<double, double> Tool::retract(double length, std::optional<double> restart_extra, std::optional<double> restart_extra_toolchange)
{
    assert(!std::isnan(length));
    assert(!restart_extra || (!std::isnan(*restart_extra) && *restart_extra >= 0)); //not nan, it's optional now
    assert(!restart_extra_toolchange || (!std::isnan(*restart_extra_toolchange) && *restart_extra_toolchange >= 0)); //not nan, it's optional now
    assert(length < std::numeric_limits<int32_t>::max());
    //assert(length > 0); // it's possible, as you may want to "reset the extruder but do not use restart_extra yet.
    assert(!restart_extra || *restart_extra < std::numeric_limits<int32_t>::max());
    assert(!restart_extra || *restart_extra > -std::numeric_limits<int32_t>::max());
    assert(!restart_extra_toolchange || *restart_extra_toolchange < std::numeric_limits<int32_t>::max());
    assert(!restart_extra_toolchange || *restart_extra_toolchange > -std::numeric_limits<int32_t>::max());
    // in case of relative E distances we always reset to 0 before any output
    if (m_config->use_relative_e_distances)
        m_E = 0.;
    // Quantize extruder delta to G-code resolution.
    double to_retract = this->retract_to_go(length);
    if (to_retract > 0.) {
        m_E             -= to_retract;
        m_absolute_E    -= to_retract;
        m_retracted     += to_retract;
        if(restart_extra)
            m_restart_extra = *restart_extra;
    }
    if (restart_extra_toolchange)
        m_restart_extra_toolchange = *restart_extra_toolchange;
    return std::make_pair(to_retract, m_E);
}
double Tool::retract_to_go(double length) const { return std::max(0., m_formatter.quantize_e(length - m_retracted)); }

std::pair<double, double> Tool::unretract()
{
    auto [dE, emitE] = this->extrude(m_retracted + m_restart_extra + m_restart_extra_toolchange);
    m_retracted     = 0.;
    m_restart_extra = 0.;
    if(m_restart_extra_toolchange != 0)
        m_restart_extra_toolchange = 0.;
    return std::make_pair(dE, emitE);
}

bool Tool::need_unretract() {
    return m_retracted + m_restart_extra + m_restart_extra_toolchange != 0;
}

// Called after a M600 or somethgin like that, so you don't have to unretract, but the absolute position won't change.
void Tool::reset_retract() {
    m_retracted = 0.;
    m_restart_extra = 0.;
    if (m_restart_extra_toolchange != 0)
        m_restart_extra_toolchange = 0.;
}
// Setting the retract state from the script.
// Sets current retraction value & restart extra filament amount if retracted > 0.
void Tool::set_retracted(double retracted, double restart_extra)
{
    if (retracted < - EPSILON)
        throw Slic3r::RuntimeError("Custom G-code reports negative z_retracted.");
    if (restart_extra < - EPSILON)
        throw Slic3r::RuntimeError("Custom G-code reports negative z_restart_extra.");

    if (retracted > EPSILON) {
        m_retracted     = retracted;
        m_restart_extra = restart_extra < EPSILON ? 0 : restart_extra;
    } else {
        m_retracted     = 0;
        m_restart_extra = 0;
    }
}

// Used filament volume in mm^3.
double Tool::extruded_volume() const
{
    return m_config->use_volumetric_e ? 
        m_absolute_E + m_retracted :
        this->used_filament() * this->filament_crossection();
}

// Used filament length in mm.
double Tool::used_filament() const
{
    return m_config->use_volumetric_e ?
        this->extruded_volume() / this->filament_crossection() :
        m_absolute_E + m_retracted;
}

double Tool::filament_diameter() const
{
    return 0;
}

double Tool::filament_density() const
{
    return 0;
}

double Tool::filament_cost() const
{
    return 0;
}

double Tool::extrusion_multiplier() const
{
    return 0;
}

// Return a "retract_before_wipe" percentage as a factor clamped to <0, 1>
double Tool::retract_before_wipe() const
{
    return 0;
}

double Tool::retract_length() const
{
    return 0;
}

double Tool::retract_lift() const
{
    return 0;
}

int Tool::retract_speed() const
{
    return 0;
}

int Tool::deretract_speed() const
{
    return 0;
}

double Tool::retract_restart_extra() const
{
    return 0;
}

double Tool::retract_length_toolchange() const
{
    return 0;
}

double Tool::retract_restart_extra_toolchange() const
{
    return 0;
}

Vec2d Tool::xy_offset() const { return Vec2d{0., 0.}; }

int16_t Tool::temp_offset() const
{
    return 0;
}

int8_t Tool::fan_offset() const
{
    return 0;
}

double Extruder::filament_diameter() const
{
    return m_config->filament_diameter.get_at(m_id);
}

double Extruder::filament_density() const
{
    return m_config->filament_density.get_at(m_id);
}

double Extruder::filament_cost() const
{
    return m_config->filament_cost.get_at(m_id);
}

double Extruder::extrusion_multiplier() const
{
    return m_config->extrusion_multiplier.get_at(m_id);
}

// Return a "retract_before_wipe" percentage as a factor clamped to <0, 1>
double Extruder::retract_before_wipe() const
{
    return std::min(1., std::max(0., m_config->retract_before_wipe.get_abs_value(m_id,1)));
}

double Extruder::retract_length() const
{
    assert(m_config->retract_length.is_enabled());
    assert(m_config->retract_length.get_at(m_id) < std::numeric_limits<int32_t>::max());
    assert(m_config->retract_length.get_at(m_id) > -std::numeric_limits<int32_t>::max());
    assert(m_config->retract_length.size() > m_id);
    return m_config->retract_length.get_at(m_id);
}

double Extruder::retract_lift() const
{
    assert(m_config->retract_lift.is_enabled());
    assert(m_config->retract_lift.get_at(m_id) < std::numeric_limits<int32_t>::max());
    assert(m_config->retract_lift.get_at(m_id) > -std::numeric_limits<int32_t>::max());
    assert(m_config->retract_lift.size() > m_id);
    return m_config->retract_lift.get_at(m_id);
}

int Extruder::retract_speed() const
{
    return int(floor(m_config->retract_speed.get_at(m_id)+0.5));
}

int Extruder::deretract_speed() const
{
    int speed = int(floor(m_config->deretract_speed.get_at(m_id)+0.5));
    return (speed > 0) ? speed : this->retract_speed();
}

double Extruder::retract_restart_extra() const
{
    assert(m_config->retract_restart_extra.is_enabled());
    assert(m_config->retract_restart_extra.get_at(m_id) < std::numeric_limits<int32_t>::max());
    assert(m_config->retract_restart_extra.get_at(m_id) > -std::numeric_limits<int32_t>::max());
    assert(m_config->retract_restart_extra.size() > m_id);
    return m_config->retract_restart_extra.get_at(m_id);
}

double Extruder::retract_length_toolchange() const
{
    return m_config->retract_length_toolchange.get_at(m_id);
}

double Extruder::retract_restart_extra_toolchange() const
{
    assert(m_config->retract_restart_extra_toolchange.is_enabled());
    assert(m_config->retract_restart_extra_toolchange.get_at(m_id) < std::numeric_limits<int32_t>::max());
    assert(m_config->retract_restart_extra_toolchange.get_at(m_id) > -std::numeric_limits<int32_t>::max());
    assert(m_config->retract_restart_extra_toolchange.size() > m_id);
    return m_config->retract_restart_extra_toolchange.get_at(m_id);
}

Vec2d Extruder::xy_offset() const { return m_config->extruder_offset.get_at(m_id); }

int16_t Extruder::temp_offset() const
{
    return int16_t(m_config->extruder_temperature_offset.get_at(m_id));
}

int8_t Extruder::fan_offset() const
{
    return int8_t(m_config->extruder_fan_offset.get_at(m_id));
}

double Mill::retract_lift() const {
    return m_config->milling_z_lift.get_at(m_mill_id);
}

}
