# SoC Image Processing Application with SystemC UVM

## Overview
ImageSysC-SoC is a comprehensive full-stack image processing System-on-Chip (SoC) project that leverages SystemC and UVM for high-level hardware modeling and verification. The project utilizes the Zynq processor, AXI bus, and GPIO to build the SoC, and incorporates Petalinux OS along with OpenCV libraries for real-time image processing. This project demonstrates the power of hardware/software co-design in camera systems requiring efficient image processing capabilities.

![image](https://github.com/cameronnthomas/Ultra-UVM/assets/173108159/d693af8e-0f9f-4154-94c5-2b1c0a593faf)

![image](https://github.com/cameronnthomas/Ultra-UVM/assets/173108159/a69079a0-5316-488b-9f7e-08dbb350f63f)


## Features
- **SoC Design**: Implements a System-on-Chip using the Zynq processor with AXI bus and GPIO.
- **Embedded OS**: Builds and boots Petalinux OS, providing a robust environment for embedded applications.
- **SystemC and UVM**: Utilizes SystemC for high-level hardware modeling and UVM for comprehensive verification.
- **Real-Time Image Processing**: Applies OpenCV libraries to process images in real-time, demonstrating practical applications in camera systems.
- **Modular Design**: Allows for easy replacement of processing modules, enhancing flexibility and scalability.

## Hardware Configuration
- **Zynq Processor**: Central processing unit of the SoC.
- **AXI Bus**: High-performance bus protocol for interconnect between processor and peripherals.
- **GPIO**: General-purpose input/output for external device interfacing.

## Software Configuration
### SystemC and UVM
- **SystemC**: Provides a high-level modeling language for designing hardware.
- **UVM**: Utilizes verification components like sequences, drivers, monitors, and scoreboards to ensure the SoC functions correctly.

### Petalinux OS
- **Embedded Linux SDK**: Custom Linux distribution tailored for FPGA-based SoC designs.
- **Bootable Image**: Configured to boot the Zynq processor with a created system image.

### OpenCV Libraries
- **Image Processing**: Implements image processing applications using OpenCV, including convolution filters and other transformations.
- **Memory Modules**: Initial image frames are stored in memory, processed, and then stored in output memory.

## How It Works
1. **Image Processing Module**: Reads image frames pixel-by-pixel, applies convolution filters, and stores the processed images.
2. **UVM Verification**: Ensures correct functionality through a framework that includes a sequencer, driver, interface, DUT, scoreboard, and monitor.
3. **Real-Time Constraints**: Maintains efficient image processing by leveraging the capabilities of the AXI bus and GPIO.
4. **Modular Approach**: Facilitates easy integration and replacement of different processing modules without impacting the overall design.

## Conclusion
ImageSysC-SoC showcases the integration of high-level hardware modeling and verification with real-time image processing. By combining SystemC, UVM, and OpenCV, this project provides a practical example of full-stack SoC design, emphasizing flexibility, scalability, and efficiency.

## Future Enhancements
- **Advanced Image Processing**: Implement more complex image processing algorithms.
- **Enhanced Verification**: Expand UVM testbenches for more comprehensive verification.
- **Cross-Platform Support**: Explore porting the project to other platforms and environments.

Feel free to contribute and enhance the capabilities of ImageSysC-SoC!
