// shadow_calc_parallel.c
// Parallelized shadow calculation using GCD

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <gsl/gsl_math.h>

#include "mathsTools/sphericalAst.h"
#include "mathsTools/julianDate.h"

#include "shadow_calc.h"
#include "parallel_gcd.h"
#include "settings.h"
#include "constants.h"
#include "projection.h"

// Structure to pass shadow calculation data to parallel workers
typedef struct {
    const settings *config;
    double jd;
    const double *pos_sun;
    const double *pos_earth;
    const double *pos_moon;
    double sidereal_time;
    double lat_sun;
    double lng_sun;
    shadow_map *shadow_map;
    shadow_map *greatest_shadow;
    time_span *span_output;
} shadow_calc_data_t;

// Worker function for parallel 2D shadow calculation
static void calculate_shadow_2d_worker(parallel_work_t *work) {
    shadow_calc_data_t *data = (shadow_calc_data_t *)work->data;
    const settings *config = data->config;
    
    // Pre-calculate constants that don't change per pixel
    const double inv_y_size = 180.0 / config->y_size_2d;
    const double inv_x_size = 360.0 / config->x_size_2d;
    
    for (int y = work->start_row; y < work->end_row; y++) {
        // Calculate latitude once per row
        double lat = 90 - (y * inv_y_size);
        double lat_rad = lat * M_PI / 180;
        
        for (int x = 0; x < config->x_size_2d; x++) {
            // Calculate longitude
            double lng = (x * inv_x_size) - 180;
            while (lng > 180) lng -= 360;
            double lng_rad = lng * M_PI / 180;
            
            // Check if Sun is above horizon
            const double ang_dist_sun = angDist_RADec(lng_rad, lat_rad, 
                                                      data->lng_sun, data->lat_sun);
            const int night_time = (ang_dist_sun > M_PI / 2);
            
            // Calculate shadow
            double shadow;
            if (night_time) {
                shadow = -1;
            } else {
                shadow = getShadowFraction(lat, lng, data->jd, 1, 
                                         data->pos_sun, data->pos_moon, 
                                         data->pos_earth, data->sidereal_time);
            }
            
            // Update shadow map
            const int offset = x + y * config->x_size_2d;
            data->shadow_map->map[offset] = shadow;
            data->shadow_map->lat[offset] = lat;
            data->shadow_map->lng[offset] = lng;
            
            // Update greatest shadow map if needed
            if (data->greatest_shadow && shadow > data->greatest_shadow->map[offset]) {
                data->greatest_shadow->map[offset] = shadow;
                
                // Thread-safe update of span_output would require mutex
                // For now, this is handled in the main thread after parallel section
            }
        }
    }
}

// Worker function for parallel 3D shadow calculation  
static void calculate_shadow_3d_worker(parallel_work_t *work) {
    shadow_calc_data_t *data = (shadow_calc_data_t *)work->data;
    const settings *config = data->config;
    
    for (int y = work->start_row; y < work->end_row; y++) {
        for (int x = 0; x < config->x_size_3d; x++) {
            const int offset = x + y * config->x_size_3d;
            
            // Calculate 3D projection coordinates
            double lng, lat, p_radius;
            project_3d(config, x, y, data->lng_sun, data->lat_sun, &lng, &lat, &p_radius);
            
            // Calculate shadow
            double shadow = 0;
            if (gsl_finite(lng) && gsl_finite(lat)) {
                shadow = getShadowFraction(lat, lng, data->jd, 1,
                                         data->pos_sun, data->pos_moon,
                                         data->pos_earth, data->sidereal_time);
            }
            
            // Update shadow map
            data->shadow_map->map[offset] = shadow;
            data->shadow_map->lat[offset] = lat;
            data->shadow_map->lng[offset] = lng;
        }
    }
}

/**
 * calculate_eclipse_map_2d_parallel - Parallel version of 2D eclipse map calculation
 */
shadow_map *calculate_eclipse_map_2d_parallel(const settings *config,
                                             double jd, const double *pos_sun, 
                                             const double *pos_earth, const double *pos_moon,
                                             time_span *span_output, shadow_map *greatest_shadow) {
    
    // Calculate sun position
    double sidereal_time, lat_sun, lng_sun;
    calculate_where_sun_overhead(&lat_sun, &lng_sun, &sidereal_time, 
                                pos_sun, pos_earth, jd);
    
    // Allocate shadow map
    shadow_map *shadow_map = allocate_shadow_map(config->x_size_2d, config->y_size_2d);
    
    // Setup parallel work data
    shadow_calc_data_t calc_data = {
        .config = config,
        .jd = jd,
        .pos_sun = pos_sun,
        .pos_earth = pos_earth,
        .pos_moon = pos_moon,
        .sidereal_time = sidereal_time,
        .lat_sun = lat_sun,
        .lng_sun = lng_sun,
        .shadow_map = shadow_map,
        .greatest_shadow = greatest_shadow,
        .span_output = span_output
    };
    
    // Execute in parallel
    parallel_for_rows(0, config->y_size_2d, config->x_size_2d,
                     &calc_data, calculate_shadow_2d_worker, 0);
    
    // Post-process for span_output updates (needs to be thread-safe)
    if (span_output && greatest_shadow) {
        for (int i = 0; i < config->x_size_2d * config->y_size_2d; i++) {
            if (shadow_map->map[i] > span_output->greatest_eclipse_magnitude) {
                span_output->greatest_eclipse_magnitude = shadow_map->map[i];
                span_output->greatest_eclipse_latitude = shadow_map->lat[i];
                span_output->greatest_eclipse_longitude = shadow_map->lng[i];
            }
        }
    }
    
    return shadow_map;
}

/**
 * calculate_eclipse_map_3d_parallel - Parallel version of 3D eclipse map calculation
 */
shadow_map *calculate_eclipse_map_3d_parallel(const settings *config,
                                             double jd, const double *pos_sun,
                                             const double *pos_earth, const double *pos_moon) {
    
    // Calculate sun position
    double sidereal_time, lat_sun, lng_sun;
    calculate_where_sun_overhead(&lat_sun, &lng_sun, &sidereal_time,
                                pos_sun, pos_earth, jd);
    
    // Allocate shadow map
    shadow_map *shadow_map = allocate_shadow_map(config->x_size_3d, config->y_size_3d);
    
    // Setup parallel work data
    shadow_calc_data_t calc_data = {
        .config = config,
        .jd = jd,
        .pos_sun = pos_sun,
        .pos_earth = pos_earth,
        .pos_moon = pos_moon,
        .sidereal_time = sidereal_time,
        .lat_sun = lat_sun,
        .lng_sun = lng_sun,
        .shadow_map = shadow_map,
        .greatest_shadow = NULL,
        .span_output = NULL
    };
    
    // Execute in parallel using tiles for better cache locality
    int tile_size = 64;  // Process in 64x64 tiles
    parallel_for_tiles(config->x_size_3d, config->y_size_3d, tile_size,
                      &calc_data, calculate_shadow_3d_worker);
    
    return shadow_map;
}