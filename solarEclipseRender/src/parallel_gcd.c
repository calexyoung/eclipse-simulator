// parallel_gcd.c
// Implementation of parallel processing using Grand Central Dispatch

#include <stdio.h>
#include <unistd.h>
#include "parallel_gcd.h"

/**
 * get_optimal_threads - Get optimal number of threads for current system
 */
int get_optimal_threads(void) {
    long nproc = sysconf(_SC_NPROCESSORS_ONLN);
    if (nproc < 1) nproc = 1;
    // Use 75% of available cores to leave room for system tasks
    int optimal = (int)(nproc * 0.75);
    if (optimal < 1) optimal = 1;
    return optimal;
}

/**
 * parallel_for_rows - Execute a function in parallel across rows
 */
void parallel_for_rows(int start_row, int end_row, int width, 
                      void *data, parallel_func_t func, int num_threads) {
    
    int total_rows = end_row - start_row;
    
    // Auto-detect thread count if not specified
    if (num_threads <= 0) {
        num_threads = get_optimal_threads();
    }
    
    // Don't use more threads than rows
    if (num_threads > total_rows) {
        num_threads = total_rows;
    }
    
    // Calculate rows per thread
    int rows_per_thread = total_rows / num_threads;
    int remaining_rows = total_rows % num_threads;
    
    // Create a dispatch group to wait for all tasks
    dispatch_group_t group = dispatch_group_create();
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
    
    // Allocate work structures
    parallel_work_t *work_items = calloc(num_threads, sizeof(parallel_work_t));
    
    // Launch parallel tasks
    int current_row = start_row;
    for (int i = 0; i < num_threads; i++) {
        work_items[i].data = data;
        work_items[i].start_row = current_row;
        work_items[i].width = width;
        work_items[i].thread_id = i;
        
        // Distribute remaining rows among first threads
        int thread_rows = rows_per_thread;
        if (i < remaining_rows) {
            thread_rows++;
        }
        
        work_items[i].end_row = current_row + thread_rows;
        current_row = work_items[i].end_row;
        
        // Dispatch async task
        dispatch_group_async(group, queue, ^{
            func(&work_items[i]);
        });
    }
    
    // Wait for all tasks to complete
    dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
    
    // Clean up
    free(work_items);
}

/**
 * parallel_for_tiles - Execute a function in parallel across tiles
 */
void parallel_for_tiles(int width, int height, int tile_size,
                       void *data, parallel_func_t func) {
    
    int tiles_x = (width + tile_size - 1) / tile_size;
    int tiles_y = (height + tile_size - 1) / tile_size;
    int total_tiles = tiles_x * tiles_y;
    
    int num_threads = get_optimal_threads();
    if (num_threads > total_tiles) {
        num_threads = total_tiles;
    }
    
    // Create dispatch group and queue
    dispatch_group_t group = dispatch_group_create();
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
    
    // Process tiles in parallel
    for (int tile_idx = 0; tile_idx < total_tiles; tile_idx++) {
        dispatch_group_async(group, queue, ^{
            int tile_y = tile_idx / tiles_x;
            int tile_x = tile_idx % tiles_x;
            
            parallel_work_t work;
            work.data = data;
            work.start_row = tile_y * tile_size;
            work.end_row = (tile_y + 1) * tile_size;
            if (work.end_row > height) work.end_row = height;
            
            work.width = tile_size;
            if ((tile_x + 1) * tile_size > width) {
                work.width = width - tile_x * tile_size;
            }
            
            work.thread_id = tile_idx;
            
            func(&work);
        });
    }
    
    // Wait for completion
    dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
}