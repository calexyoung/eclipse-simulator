#!/bin/bash

# Eclipse Simulator Performance Benchmark
# Tests sequential vs parallel performance

echo "Eclipse Simulator Performance Benchmark"
echo "======================================="
echo ""

# Test parameters - using a short time span for quick testing
JD_MIN=2460763.811
JD_MAX=2460763.812
OUTPUT_DIR="/tmp/eclipse_benchmark"

# Create output directories
mkdir -p "$OUTPUT_DIR/sequential"
mkdir -p "$OUTPUT_DIR/parallel"

echo "Test Configuration:"
echo "  Julian Date Range: $JD_MIN to $JD_MAX"
echo "  Output Directory: $OUTPUT_DIR"
echo ""

# Get system info
echo "System Information:"
sysctl -n machdep.cpu.brand_string
echo "  CPU Cores: $(sysctl -n hw.ncpu)"
echo "  Physical Cores: $(sysctl -n hw.physicalcpu)"
echo ""

# Test 1: Sequential processing (baseline)
echo "Running Sequential Test..."
START_TIME=$(date +%s)
./solarEclipseRender/bin/eclipseRender.bin \
    --jd_min $JD_MIN \
    --jd_max $JD_MAX \
    --output "$OUTPUT_DIR/sequential" \
    --title "Sequential Test" 2>/dev/null
END_TIME=$(date +%s)
SEQ_TIME=$((END_TIME - START_TIME))
echo "  Sequential Time: ${SEQ_TIME}s"

# Test 2: Parallel processing
echo "Running Parallel Test..."
START_TIME=$(date +%s)
./solarEclipseRender/bin/eclipseRender.bin \
    --jd_min $JD_MIN \
    --jd_max $JD_MAX \
    --output "$OUTPUT_DIR/parallel" \
    --title "Parallel Test" \
    --parallel 2>/dev/null
END_TIME=$(date +%s)
PAR_TIME=$((END_TIME - START_TIME))
echo "  Parallel Time: ${PAR_TIME}s"

# Calculate speedup
if [ $PAR_TIME -gt 0 ]; then
    SPEEDUP=$(echo "scale=2; $SEQ_TIME / $PAR_TIME" | bc)
    IMPROVEMENT=$(echo "scale=1; ($SEQ_TIME - $PAR_TIME) * 100 / $SEQ_TIME" | bc)
    echo ""
    echo "Performance Results:"
    echo "===================="
    echo "  Sequential Time: ${SEQ_TIME}s"
    echo "  Parallel Time: ${PAR_TIME}s"
    echo "  Speedup: ${SPEEDUP}x"
    echo "  Performance Improvement: ${IMPROVEMENT}%"
else
    echo "Error: Parallel test completed too quickly to measure"
fi

echo ""
echo "Benchmark complete!"