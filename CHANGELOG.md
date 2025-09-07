# Eclipse Simulator Changelog

## [2.1.0] - 2025-09-07

### Performance Optimizations
- **Compiler Optimizations**
  - Added `-O3` maximum optimization level
  - Enabled `-march=native` for CPU-specific optimizations (Apple M4 Max)
  - Added `-ffast-math` for aggressive floating-point optimizations
  - Implemented `-flto` for link-time optimization
  
- **Parallel Processing** 
  - Implemented Grand Central Dispatch (GCD) for macOS-native threading
  - Created parallel versions of `calculate_eclipse_map_2d` and `calculate_eclipse_map_3d`
  - Added `--parallel` command-line flag to enable parallel processing
  - Automatic thread count detection (75% of available cores)
  - Achieved ~11.5% performance improvement on standard workloads

- **Algorithm Optimizations**
  - Pre-calculated loop invariants outside inner loops
  - Implemented tile-based processing for better cache locality
  - Optimized memory access patterns to reduce cache misses
  - Reduced redundant calculations in shadow fraction computations

### Visualization Improvements

#### Phase 1: Resolution and Basic Rendering
- **Resolution Increases**
  - Increased 2D map resolution from 1920x960 to 3840x1920 (4K)
  - Increased 3D globe resolution from 1024x1024 to 2048x2048 (2K)
  - Increased Earth pixel radius from 512 to 820 pixels

- **Command-line Options**
  - Added `--resolution-2d` flag for custom 2D map resolution
  - Added `--resolution-3d` flag for custom 3D globe resolution
  - Added `--earth-radius` flag to set Earth pixel radius
  - Added `--antialias` flag to enable/disable antialiasing
  - Added `--supersampling` flag with configurable factor (2x, 4x)
  - Added `--parallel` flag for parallel processing

- **Antialiasing and Supersampling**
  - Implemented CAIRO_ANTIALIAS_BEST for smooth edges
  - Added 2x/4x supersampling option for ultra-high quality renders
  - Improved text rendering with subpixel antialiasing

#### Phase 2: Advanced Shadow Rendering
- **Gradient-based Shadow Rendering**
  - Implemented smooth gradient transitions between umbra and penumbra
  - Added distinct shadow zones (umbra at 95%+, penumbra 5%-95%)
  - Created atmospheric scattering simulation for realistic edge effects

- **Shadow Map Enhancements**
  - Added floating-point precision shadow calculations
  - Implemented multi-sample shadow testing for smoother edges
  - Created adaptive shadow sampling based on eclipse magnitude

- **Color and Lighting Improvements**
  - Enhanced twilight zone rendering with color temperature adjustments
  - Added atmospheric refraction effects at shadow boundaries
  - Implemented realistic Earth limb darkening

### Build System
- Successfully adapted for macOS compilation
- Added Homebrew library paths for Apple Silicon
- Removed OpenMP dependencies (not supported on macOS)
- Integrated new parallel processing modules

### Documentation
- Created VISUALIZATION_IMPROVEMENTS.md with detailed enhancement plan
- Added PERFORMANCE_RESULTS.md with benchmark results
- Created OPTIMIZATION_ROADMAP.md for future improvements
- Added benchmark.sh script for performance testing

## [2.0.0] - Previous Release
- Switch to use of DE430 ephemeris
- Fix possible array overflow
- Add borders to diagrams of countries
- Fix warnings in Ubuntu 20.04
- Updated copyright and licensing information to GPL V3