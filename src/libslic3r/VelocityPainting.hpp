#ifndef VELOCITY_PAINTING_HPP
#define VELOCITY_PAINTING_HPP

#include <vector>
#include "GCodeLine.hpp"

namespace Slic3r {

class VelocityPainting {
public:
    void adjust_print_speed(std::vector<GCodeLine>& gcode_lines, const std::vector<std::vector<int>>& image_data, int projection_mode, double z_offset, double min_speed, double max_speed);
    double get_intensity(const GCodeLine& line, const std::vector<std::vector<int>>& image_data, int projection_mode, double z_offset);
    void process_gcode(std::vector<GCodeLine>& gcode_lines, const std::vector<std::vector<int>>& image_data, int projection_mode, double z_offset, double min_speed, double max_speed);
};

} // namespace Slic3r

#endif // VELOCITY_PAINTING_HPP
