# Eclipse Simulator - Changelog

All notable changes and improvements to the Eclipse Simulator visualization quality are documented in this file.

## [Unreleased]

### Phase 1 - Core Quality Improvements (2025-09-07)

#### Resolution Enhancements
- **2D Map Resolution**: Increased from 1600x800 to 3840x1920 (4K)
- **3D Globe Resolution**: Increased from 800x800 to 2048x2048 (2K)
- **Earth Pixel Radius**: Increased from 320 to 820 pixels for better detail in 3D views
- **Binary Map Angular Resolution**: Increased from 4 to 8 for more accurate shadow calculations

#### Command-Line Options Added
- `--resolution-2d-x WIDTH`: Set custom horizontal resolution for 2D maps
- `--resolution-2d-y HEIGHT`: Set custom vertical resolution for 2D maps
- `--resolution-3d SIZE`: Set custom resolution for 3D globe (square)
- `--earth-radius PIXELS`: Set Earth radius in pixels for 3D view

#### Rendering Quality
- **Antialiasing**: Implemented `CAIRO_ANTIALIAS_BEST` across all Cairo rendering contexts
  - Smoother contour lines
  - Improved text rendering quality
  - Better edge smoothing on eclipse boundaries

#### Earth Texture Improvements
- **Day Texture**: Upscaled from 4096x2048 to 8192x4096
- **Night Texture**: Upscaled from 2048x1024 to 4096x2048
- **Image Enhancements Applied**:
  - Sharpness enhancement factor: 1.2
  - Color saturation boost: 1.1
  - Contrast improvement: 1.05
- **Texture Processing**: Added `enhance_textures.py` utility for texture enhancement
- **Quality Settings**: JPEG quality increased to 95 with optimization

#### Build System Updates
- Fixed macOS compilation by removing OpenMP dependencies
- Updated Makefiles to use Homebrew paths for dependencies
- Added proper linking for GSL, Cairo, and other libraries on macOS

#### Files Modified
- `solarEclipseRender/src/settings.c` - Updated default resolutions
- `solarEclipseRender/src/main.c` - Added command-line options
- `solarEclipseRender/src/render_2d.c` - Added antialiasing
- `solarEclipseRender/src/render_3d.c` - Added antialiasing
- `solarEclipseRender/Makefile` - macOS compatibility fixes
- `ephemeris-compute-de430/Makefile` - macOS compatibility fixes

#### Files Added
- `solarEclipseRender/enhance_textures.py` - Texture enhancement utility
- `VISUALIZATION_IMPROVEMENTS.md` - Comprehensive improvement plan
- `CHANGELOG.md` - This changelog

---

## Planned Improvements

### Phase 2 - Shadow and Lighting Enhancements
- [ ] Implement gradient-based shadow edges
- [ ] Add proper penumbra/umbra distinction
- [ ] Variable shadow darkness based on eclipse magnitude
- [ ] Atmospheric scattering effects at shadow edges

### Phase 3 - Atmospheric Effects
- [ ] Earth atmospheric glow effect
- [ ] Cloud layer overlay
- [ ] Terminator line shading
- [ ] Atmospheric refraction effects

### Phase 4 - Additional Visual Elements
- [ ] Improved eclipse magnitude labels
- [ ] Coordinate grid overlay
- [ ] Eclipse path centerline highlighting
- [ ] Duration annotations along path

### Phase 5 - Performance Optimizations
- [ ] Multi-threading for rendering
- [ ] Texture atlas implementation
- [ ] Memory pool allocation
- [ ] Streaming output for video generation

### Phase 6 - Output Format Enhancements
- [ ] 16-bit per channel PNG support
- [ ] TIFF output option
- [ ] GeoTIFF with embedded coordinates
- [ ] KML/KMZ export

---

## Testing

### Test Renders Completed
- **2025-09-07**: Successfully rendered March 29, 2025 partial solar eclipse
  - Output location: `/tmp/test_eclipse_improved/`
  - Frame count: 62 frames (2D and 3D)
  - Resolution tested: 1024x1024 for 3D globe
  - All antialiasing features confirmed working

---

## Notes

- Original Earth textures backed up as `earth_day_original.jpg` and `earth_night_original.jpg`
- All improvements maintain backward compatibility with existing command-line interface
- Performance impact of improvements is minimal due to optimized rendering pipeline