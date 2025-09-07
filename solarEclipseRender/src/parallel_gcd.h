// parallel_gcd.h
// Parallel processing using Grand Central Dispatch for macOS

#ifndef PARALLEL_GCD_H
#define PARALLEL_GCD_H 1

#include <dispatch/dispatch.h>
#include <stdlib.h>

// Structure to pass data to parallel workers
typedef struct {
    void *data;
    int start_row;
    int end_row;
    int width;
    int thread_id;
} parallel_work_t;

// Function pointer type for parallel work
typedef void (*parallel_func_t)(parallel_work_t *work);

/**
 * parallel_for_rows - Execute a function in parallel across rows
 * 
 * @param start_row - First row to process
 * @param end_row - Last row to process (exclusive)
 * @param width - Width of each row
 * @param data - Data to pass to worker function
 * @param func - Function to execute for each chunk
 * @param num_threads - Number of threads to use (0 = auto)
 */
void parallel_for_rows(int start_row, int end_row, int width, 
                      void *data, parallel_func_t func, int num_threads);

/**
 * parallel_for_tiles - Execute a function in parallel across tiles
 * Improves cache locality by processing square tiles
 * 
 * @param width - Total width
 * @param height - Total height
 * @param tile_size - Size of each tile (e.g., 64x64)
 * @param data - Data to pass to worker function
 * @param func - Function to execute for each tile
 */
void parallel_for_tiles(int width, int height, int tile_size,
                       void *data, parallel_func_t func);

/**
 * get_optimal_threads - Get optimal number of threads for current system
 */
int get_optimal_threads(void);

#endif