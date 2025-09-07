// shadow_gradient.h
// Header for improved shadow rendering with gradient-based edges

#ifndef SHADOW_GRADIENT_H
#define SHADOW_GRADIENT_H 1

#include "settings.h"
#include "rendering.h"

/**
 * Calculate improved shadow darkness with smooth gradients
 */
double calculate_shadow_gradient(double shadow_fraction, const settings *config);

/**
 * Apply shadow to a color with improved gradient rendering
 */
colour apply_shadow_with_gradient(colour colour_in, double shadow_fraction, const settings *config);

/**
 * Apply atmospheric scattering effects at shadow edges
 */
colour apply_atmospheric_scattering(colour colour_in, double shadow_fraction, double distance_from_center);

#endif