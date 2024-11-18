# Velocity Painting Plugin for SuperSlicer

## Overview

The Velocity Painting plugin for SuperSlicer allows you to dynamically adjust the print speed based on the intensity of a grayscale image. This plugin supports various projection modes and processes G-code post-slicing to apply speed changes.

## Features

1. **Velocity Painting**:
   - Adjust print speed dynamically based on grayscale image intensity.
   - Support projection modes: `projectX`, `projectY`, `projectZ`, `cylinderZ`, `spherical`.
   - Process G-code post-slicing to apply speed changes.

2. **Extrusion Painting**:
   - Modify extrusion rates to create patterns (e.g., over/under extrusion).
   - Smooth transitions (anti-aliasing) for gradients.

3. **User Interface (GUI)**:
   - Add a SuperSlicer tab for Velocity/Extrusion Painting.
   - Input fields: projection mode, center coordinates, image dimensions, Z-offset, speeds, and extrusion multiplier.
   - Allow preview of effects on models.

4. **Command-Line Interface (CLI)**:
   - Implement CLI tool for advanced users:
     ```
     python velocity_painting.py <projection_mode> <params> <image_file> <source_gcode> <output_gcode>
     ```

5. **G-code Processing**:
   - Adapt OrcaSlicer logic for SuperSlicer compatibility.
   - Ensure valid G-code for single/multi-wall prints.

## Installation

1. Clone the repository:
   ```
   git clone https://github.com/supermerill/SuperSlicer.git
   ```

2. Navigate to the plugin directory:
   ```
   cd SuperSlicer/plugins/velocity_painting
   ```

3. Install the required dependencies:
   ```
   pip install -r requirements.txt
   ```

## Usage

### GUI

1. Open SuperSlicer and navigate to the Velocity/Extrusion Painting tab.
2. Select the desired projection mode.
3. Enter the center coordinates, image dimensions, Z-offset, speeds, and extrusion multiplier.
4. Click the "Preview" button to see the effects on the model.
5. Click "Apply" to process the G-code with the specified settings.

### CLI

1. Open a terminal and navigate to the plugin directory:
   ```
   cd SuperSlicer/plugins/velocity_painting
   ```

2. Run the CLI tool with the desired parameters:
   ```
   python velocity_painting.py <projection_mode> <params> <image_file> <source_gcode> <output_gcode>
   ```

## Examples

### Example 1: Basic Usage

```
python velocity_painting.py projectX "center=50,50;dimensions=100,100;z_offset=0;min_speed=1000;max_speed=3000" image.png input.gcode output.gcode
```

### Example 2: Advanced Usage

```
python velocity_painting.py spherical "center=50,50;dimensions=100,100;z_offset=0;min_speed=800;max_speed=2500" pattern.png input.gcode output.gcode
```

## Contributing

1. Fork the repository.
2. Create a new branch:
   ```
   git checkout -b feature/your-feature-name
   ```

3. Make your changes and commit them:
   ```
   git commit -m "Add your commit message"
   ```

4. Push to the branch:
   ```
   git push origin feature/your-feature-name
   ```

5. Open a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

This plugin is refactored from an existing OrcaSlicer plugin. Special thanks to the OrcaSlicer team for their contributions.
