#ifndef EXTRUSION_PAINTING_HPP
#define EXTRUSION_PAINTING_HPP

#include <vector>
#include "GCodeLine.hpp"

namespace Slic3r {

class ExtrusionPainting {
public:
    void modify_extrusion_rates(std::vector<GCodeLine>& gcode_lines, const std::vector<std::vector<int>>& image_data, int projection_mode, double z_offset, double min_extrusion, double max_extrusion);
    double get_intensity(const GCodeLine& line, const std::vector<std::vector<int>>& image_data, int projection_mode, double z_offset);
    void smooth_transitions(std::vector<GCodeLine>& gcode_lines);
    void process_gcode(std::vector<GCodeLine>& gcode_lines, const std::vector<std::vector<int>>& image_data, int projection_mode, double z_offset, double min_extrusion, double max_extrusion);
};

} // namespace Slic3r

#endif // EXTRUSION_PAINTING_HPP
