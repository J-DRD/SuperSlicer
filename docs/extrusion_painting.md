# Extrusion Painting Plugin for SuperSlicer

The Extrusion Painting plugin for SuperSlicer allows you to modify extrusion rates to create patterns and smooth transitions for gradients. This document provides an overview of the plugin's features and usage instructions.

## Features

1. **Modify Extrusion Rates**: Adjust extrusion rates based on grayscale image intensity to create patterns such as over/under extrusion.
2. **Smooth Transitions**: Implement anti-aliasing for smooth transitions in gradients.
3. **Projection Modes**: Support for various projection modes including `projectX`, `projectY`, `projectZ`, `cylinderZ`, and `spherical`.
4. **G-code Processing**: Process G-code post-slicing to apply extrusion rate changes.

## User Interface (GUI)

The Extrusion Painting plugin adds a new tab in SuperSlicer's interface with the following input fields:

- **Projection Mode**: Select the projection mode from the dropdown list.
- **Center Coordinates**: Input the X and Y coordinates for the center of the projection.
- **Image Dimensions**: Specify the width and height of the image.
- **Z-offset**: Set the Z-offset value.
- **Extrusion Multiplier**: Adjust the extrusion multiplier value.

### Preview

The plugin allows you to preview the effects of the extrusion painting on your model before applying the changes.

## Command-Line Interface (CLI)

For advanced users, the plugin provides a CLI tool for extrusion painting. The CLI tool can be used with the following parameters:

```
python extrusion_painting.py <projection_mode> <params> <image_file> <source_gcode> <output_gcode>
```

- **projection_mode**: The projection mode to use (e.g., `projectX`, `projectY`, `projectZ`, `cylinderZ`, `spherical`).
- **params**: Additional parameters for the projection mode.
- **image_file**: The path to the grayscale image file.
- **source_gcode**: The path to the source G-code file.
- **output_gcode**: The path to the output G-code file.

## G-code Processing

The plugin adapts the logic from OrcaSlicer to ensure compatibility with SuperSlicer. It processes the G-code post-slicing to apply the extrusion rate changes based on the grayscale image intensity.

## Installation

To install the Extrusion Painting plugin for SuperSlicer, follow these steps:

1. Download the plugin files from the repository.
2. Copy the plugin files to the appropriate directory in your SuperSlicer installation.
3. Restart SuperSlicer to load the plugin.

## Usage

1. Open SuperSlicer and load your 3D model.
2. Navigate to the Extrusion Painting tab.
3. Configure the input fields as needed.
4. Click the "Preview" button to see the effects on your model.
5. Once satisfied, proceed with slicing and exporting the G-code.

For CLI usage, run the `extrusion_painting.py` script with the appropriate parameters as described above.

## Support

If you encounter any issues or have questions about the Extrusion Painting plugin, please refer to the documentation or contact the support team.

## License

This plugin is released under the MIT License. See the LICENSE file for more details.
