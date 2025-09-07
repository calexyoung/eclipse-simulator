# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Eclipse Simulator is a command-line tool for producing animations and visualizations of solar eclipses. It generates diagrams, KML path files, and video animations based on the DE430 planetary ephemeris from NASA's Jet Propulsion Laboratory.

## Architecture

### Core Components

1. **Python Frontend** (`solarEclipses.py`): Main entry point that orchestrates eclipse simulations
   - Processes eclipse data from `solarEclipses.dat`
   - Uses Dask for parallel processing of multiple eclipse simulations
   - Generates output in `/output` directory

2. **C Renderer** (`solarEclipseRender/`): Core rendering engine written in C
   - Binary: `bin/eclipseRender.bin`
   - Computes eclipse visibility and generates visualizations
   - Uses Cairo for graphics, GSL for mathematical computations

3. **KML Generator** (`solarEclipses_makeKml.py`): Converts JSON output to KML format for Google Earth

4. **External Dependency**: `ephemeris-compute-de430` - Automatically downloaded from GitHub for planetary position calculations

## Build Commands

```bash
# Build the C renderer
cd solarEclipseRender
./prettymake  # or 'make' directly

# Clean build
make clean
make afresh  # clean + rebuild
```

## Running Simulations

```bash
# Simulate eclipses for specific years (default: 2020-2021)
./solarEclipses.py --year-min 2025 --year-max 2030

# The script will:
# 1. Download ephemeris-compute-de430 if not present (~500MB)
# 2. Compile the C renderer
# 3. Run simulations in parallel
# 4. Generate KML files
```

## Key Dependencies

### Python
- `dask` - Parallel processing
- Standard library only otherwise

### C Libraries (required for building)
- `cairo` - Graphics rendering
- `gsl`, `gslcblas` - Mathematical computations
- `libpng`, `libjpeg` - Image processing
- `mysql_config` - MySQL client libraries
- `zlib` - Compression

## Output Structure

Simulations create temporary files in `/tmp/eclipse_*` and final output in `./output/`:
- Eclipse visualization images
- JSON data files with eclipse parameters
- KML files for geographic visualization

## Time Handling

- All times are in UTC (enforced by the Python script)
- Julian Day numbers are used internally (Terrestrial Time)
- Eclipse data spans 1600-2200 AD (DE430 ephemeris limits)