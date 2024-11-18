#include "VelocityPainting.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

namespace Slic3r {

void VelocityPainting::adjust_print_speed(std::vector<GCodeLine>& gcode_lines, const std::vector<std::vector<int>>& image_data, int projection_mode, double z_offset, double min_speed, double max_speed) {
    for (auto& line : gcode_lines) {
        double intensity = get_intensity(line, image_data, projection_mode, z_offset);
        double speed = min_speed + (max_speed - min_speed) * intensity;
        line.set_speed(speed);
    }
}

double VelocityPainting::get_intensity(const GCodeLine& line, const std::vector<std::vector<int>>& image_data, int projection_mode, double z_offset) {
    // Implement the logic to get the intensity based on the projection mode and image data
    // This is a placeholder implementation
    return 0.5;
}

void VelocityPainting::process_gcode(std::vector<GCodeLine>& gcode_lines, const std::vector<std::vector<int>>& image_data, int projection_mode, double z_offset, double min_speed, double max_speed) {
    adjust_print_speed(gcode_lines, image_data, projection_mode, z_offset, min_speed, max_speed);
}

} // namespace Slic3r
