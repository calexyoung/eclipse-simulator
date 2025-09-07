// shadow_gradient.c
// Improved shadow rendering with gradient-based edges and penumbra/umbra distinction

#include <math.h>
#include "settings.h"
#include "rendering.h"

/**
 * calculate_shadow_gradient - Calculate improved shadow darkness with smooth gradients
 * 
 * @param shadow_fraction - Eclipse magnitude (0-1, where 1 is total eclipse)
 * @param config - Configuration settings
 * @return - Shadow darkness factor (0 = no shadow, 1 = maximum darkness)
 */
double calculate_shadow_gradient(double shadow_fraction, const settings *config) {
    if (shadow_fraction <= 0) {
        return 0.0;
    }
    
    // Define threshold for umbra (total shadow) - typically above 95% coverage
    const double umbra_threshold = 0.95;
    
    // Define penumbra gradient zones
    const double penumbra_start = 0.001;  // Very light shadow starts here
    const double penumbra_mid = 0.5;      // Medium shadow
    const double penumbra_heavy = 0.8;    // Heavy shadow begins
    
    double darkness;
    
    if (shadow_fraction >= umbra_threshold) {
        // Umbra zone - maximum darkness
        darkness = 0.95 + 0.05 * ((shadow_fraction - umbra_threshold) / (1.0 - umbra_threshold));
    } else if (shadow_fraction >= penumbra_heavy) {
        // Heavy penumbra - rapid darkness increase
        double t = (shadow_fraction - penumbra_heavy) / (umbra_threshold - penumbra_heavy);
        // Use cubic easing for smooth transition
        darkness = 0.7 + 0.25 * (t * t * (3.0 - 2.0 * t));
    } else if (shadow_fraction >= penumbra_mid) {
        // Medium penumbra - moderate darkness
        double t = (shadow_fraction - penumbra_mid) / (penumbra_heavy - penumbra_mid);
        // Use quadratic easing
        darkness = 0.3 + 0.4 * (t * t);
    } else if (shadow_fraction >= penumbra_start) {
        // Light penumbra - gentle shadow
        double t = (shadow_fraction - penumbra_start) / (penumbra_mid - penumbra_start);
        // Use smooth step function for very gradual onset
        darkness = 0.3 * (t * t * (3.0 - 2.0 * t));
    } else {
        darkness = 0.0;
    }
    
    return darkness;
}

/**
 * apply_shadow_with_gradient - Apply shadow to a color with improved gradient rendering
 * 
 * @param colour_in - Original color
 * @param shadow_fraction - Eclipse magnitude (0-1)
 * @param config - Configuration settings
 * @return - Modified color with shadow applied
 */
colour apply_shadow_with_gradient(colour colour_in, double shadow_fraction, const settings *config) {
    colour result = colour_in;
    
    if (shadow_fraction <= 0) {
        return result;
    }
    
    // Calculate shadow darkness using gradient function
    double darkness = calculate_shadow_gradient(shadow_fraction, config);
    
    // Calculate color temperature shift for realistic shadow
    // Shadows during eclipse have a bluish tint due to atmospheric scattering
    double blue_shift = darkness * 0.1;  // Subtle blue shift in shadows
    double red_reduction = darkness * 1.1;  // Slightly more red reduction
    
    // Apply shadow with color temperature adjustment
    result.red = (int)(colour_in.red * (1.0 - darkness * red_reduction) + 
                       config->shadow_col_r * darkness);
    result.grn = (int)(colour_in.grn * (1.0 - darkness) + 
                       config->shadow_col_g * darkness);
    result.blu = (int)(colour_in.blu * (1.0 - darkness * (1.0 - blue_shift)) + 
                       config->shadow_col_b * darkness * (1.0 + blue_shift));
    
    // Ensure values stay in valid range
    if (result.red < 0) result.red = 0;
    if (result.red > 255) result.red = 255;
    if (result.grn < 0) result.grn = 0;
    if (result.grn > 255) result.grn = 255;
    if (result.blu < 0) result.blu = 0;
    if (result.blu > 255) result.blu = 255;
    
    return result;
}

/**
 * apply_atmospheric_scattering - Apply atmospheric scattering effects at shadow edges
 * 
 * @param colour_in - Input color
 * @param shadow_fraction - Eclipse magnitude
 * @param distance_from_center - Normalized distance from eclipse center (0-1)
 * @return - Color with atmospheric effects applied
 */
colour apply_atmospheric_scattering(colour colour_in, double shadow_fraction, double distance_from_center) {
    colour result = colour_in;
    
    // Only apply scattering near shadow edges
    if (shadow_fraction > 0.01 && shadow_fraction < 0.9) {
        // Calculate edge proximity (highest at transition zones)
        double edge_factor = 4.0 * shadow_fraction * (1.0 - shadow_fraction);
        
        // Reddish tint at shadow edges (sunset-like effect)
        double red_boost = edge_factor * 0.15 * (1.0 - distance_from_center);
        double blue_reduction = edge_factor * 0.1 * (1.0 - distance_from_center);
        
        result.red = (int)(colour_in.red * (1.0 + red_boost));
        result.grn = colour_in.grn;
        result.blu = (int)(colour_in.blu * (1.0 - blue_reduction));
        
        // Clamp values
        if (result.red > 255) result.red = 255;
        if (result.blu < 0) result.blu = 0;
    }
    
    return result;
}