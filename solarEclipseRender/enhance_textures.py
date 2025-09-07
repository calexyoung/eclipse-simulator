#!/usr/bin/env python3

import sys
from PIL import Image, ImageEnhance, ImageFilter

def enhance_earth_texture(input_path, output_path, target_width=8192):
    """
    Enhance Earth texture quality through upscaling and image processing.
    """
    print(f"Processing {input_path}...")
    
    # Open the image
    img = Image.open(input_path)
    original_width, original_height = img.size
    
    # Calculate target dimensions maintaining aspect ratio
    aspect_ratio = original_height / original_width
    target_height = int(target_width * aspect_ratio)
    
    # Upscale using LANCZOS resampling (high quality)
    img_resized = img.resize((target_width, target_height), Image.LANCZOS)
    
    # Enhance sharpness slightly
    enhancer = ImageEnhance.Sharpness(img_resized)
    img_sharp = enhancer.enhance(1.2)
    
    # Enhance color saturation slightly for more vibrant colors
    enhancer = ImageEnhance.Color(img_sharp)
    img_color = enhancer.enhance(1.1)
    
    # Enhance contrast slightly
    enhancer = ImageEnhance.Contrast(img_color)
    img_final = enhancer.enhance(1.05)
    
    # Save with high quality
    img_final.save(output_path, 'JPEG', quality=95, optimize=True)
    
    print(f"  Original: {original_width}x{original_height}")
    print(f"  Enhanced: {target_width}x{target_height}")
    print(f"  Saved to: {output_path}")

def main():
    # Enhance day texture
    enhance_earth_texture('earth_day.jpg', 'earth_day_enhanced.jpg', target_width=8192)
    
    # Enhance night texture
    enhance_earth_texture('earth_night.jpg', 'earth_night_enhanced.jpg', target_width=4096)
    
    print("\nTexture enhancement complete!")
    print("Original textures backed up as earth_day_original.jpg and earth_night_original.jpg")

if __name__ == "__main__":
    main()