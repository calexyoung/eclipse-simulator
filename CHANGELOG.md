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

- **Additional Performance Improvements**
  - Replaced all `gsl_pow_2()` calls with direct multiplication
  - Switched to libjpeg-turbo for 2-6x faster JPEG I/O
  - Integrated Apple Accelerate Framework for BLAS/vector operations
  - Created math_accelerate.h for vectorized math functions

### Visualization Improvements

#### Phase 2 - Shadow and Lighting Enhancements
- **Gradient-Based Shadow Edges**: Implemented smooth shadow transitions using cubic and quadratic easing functions
- **Penumbra/Umbra Distinction**: 
  - Umbra zone (>95% coverage): Maximum darkness with subtle gradients
  - Heavy penumbra (80-95%): Rapid darkness increase with cubic easing
  - Medium penumbra (50-80%): Moderate darkness with quadratic easing  
  - Light penumbra (0.1-50%): Gentle shadow with smooth step function
- **Variable Shadow Darkness**: Shadow intensity varies realistically based on eclipse magnitude
- **Atmospheric Scattering Effects**:
  - Bluish tint in shadows simulating atmospheric scattering
  - Reddish sunset-like effects at shadow boundaries
  - Color temperature adjustments for realistic appearance

#### Phase 1 - Core Quality Improvements
- **Resolution Enhancements**
  - 2D Map Resolution: Increased from 1600x800 to 3840x1920 (4K)
  - 3D Globe Resolution: Increased from 800x800 to 2048x2048 (2K)
  - Earth Pixel Radius: Increased from 320 to 820 pixels
  - Binary Map Angular Resolution: Increased from 4 to 8

- **Command-line Options**
  - Added `--resolution-2d-x` and `--resolution-2d-y` for custom 2D map resolution
  - Added `--resolution-3d` for custom 3D globe resolution
  - Added `--earth-radius` to set Earth pixel radius
  - Added `--parallel` flag for parallel processing

- **Antialiasing and Rendering Quality**
  - Implemented CAIRO_ANTIALIAS_BEST for smooth edges
  - Improved text rendering with subpixel antialiasing
  - Better edge smoothing on eclipse boundaries

- **Earth Texture Improvements**
  - Day Texture: Upscaled from 4096x2048 to 8192x4096
  - Night Texture: Upscaled from 2048x1024 to 4096x2048
  - Applied sharpness, saturation, and contrast enhancements

### Build System
- Successfully adapted for macOS compilation
- Added Homebrew library paths for Apple Silicon
- Removed OpenMP dependencies (not supported on macOS)
- Integrated new parallel processing modules
- Fixed all Makefiles for macOS compatibility

### Documentation
- Created VISUALIZATION_IMPROVEMENTS.md with detailed enhancement plan
- Added PERFORMANCE_RESULTS.md with benchmark results
- Created OPTIMIZATION_ROADMAP.md for future improvements
- Added benchmark.sh script for performance testing
- Updated .gitignore to exclude macOS files and ephemeris-compute-de430

## [2.0.0] - Previous Release
- Switch to use of DE430 ephemeris
- Fix possible array overflow
- Add borders to diagrams of countries
- Fix warnings in Ubuntu 20.04
- Updated copyright and licensing information to GPL V3