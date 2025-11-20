#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "Advanced.h"

Image *Crop(Image *image, int x, int y, int W, int H) {
    assert(image != NULL);
    assert(image->R != NULL && image->G != NULL && image->B != NULL);
    
    unsigned int width = ImageWidth(image);
    unsigned int height = ImageHeight(image);
    
    // Ensure cropping parameters do not exceed image dimensions
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + W > width) W = width - x;
    if (y + H > height) H = height - y;
    
    // Create new cropped image
    Image *croppedImage = CreateImage(W, H);
    
    // Copy pixel values from the original image to the cropped image
    for (unsigned int j = 0; j < H; j++) {
        for (unsigned int i = 0; i < W; i++) {
            unsigned char r = GetPixelR(image, x + i, y + j);
            unsigned char g = GetPixelG(image, x + i, y + j);
            unsigned char b = GetPixelB(image, x + i, y + j);
            
            SetPixelR(croppedImage, i, j, r);
            SetPixelG(croppedImage, i, j, g);
            SetPixelB(croppedImage, i, j, b);
        }
    }
    DeleteImage(image);
    image=NULL;
    return croppedImage;
}

Image *Resize(Image *image, int newWidth, int newHeight) {
    assert(image != NULL);
    assert(image->R != NULL && image->G != NULL && image->B != NULL);
    
    unsigned int oldWidth = ImageWidth(image);
    unsigned int oldHeight = ImageHeight(image);
    
    // Scale factors
    double scaleX = (double)oldWidth / newWidth;
    double scaleY = (double)oldHeight / newHeight;
    
    // Create new resized image
    Image *resizedImage = CreateImage(newWidth, newHeight);
    if (!resizedImage) {
        printf("Failed to allocate memory for resized image.\n");
        return NULL;
    }
    
    // Iterate over the target image (new size)
    for (unsigned int y = 0; y < newHeight; y++) {
        for (unsigned int x = 0; x < newWidth; x++) {
            // Map target pixel (x, y) back to the original image
            unsigned int srcX = (unsigned int)(x * scaleX);
            unsigned int srcY = (unsigned int)(y * scaleY);
            
            // Ensure the mapped coordinates are within bounds
            if (srcX >= oldWidth) srcX = oldWidth - 1;
            if (srcY >= oldHeight) srcY = oldHeight - 1;
            
            // Get pixel values from the original image
            unsigned char r = GetPixelR(image, srcX, srcY);
            unsigned char g = GetPixelG(image, srcX, srcY);
            unsigned char b = GetPixelB(image, srcX, srcY);
            
            // Assign values to the new image
            SetPixelR(resizedImage, x, y, r);
            SetPixelG(resizedImage, x, y, g);
            SetPixelB(resizedImage, x, y, b);
        }
    }
    DeleteImage(image);
    image=NULL;
    return resizedImage;
}

Image *Watermark(Image *image, const Image *watermark_image) {
    assert(image != NULL && watermark_image != NULL);
    assert(image->R != NULL && image->G != NULL && image->B != NULL);
    assert(watermark_image->R != NULL && watermark_image->G != NULL && watermark_image->B != NULL);
    
    unsigned int width = ImageWidth(image);
    unsigned int height = ImageHeight(image);
    unsigned int watermarkWidth = ImageWidth(watermark_image);
    unsigned int watermarkHeight = ImageHeight(watermark_image);
    
    // Apply the watermark
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            // Get the corresponding pixel in the watermark (wrapping around if necessary)
            unsigned int wx = x % watermarkWidth;
            unsigned int wy = y % watermarkHeight;
            
            unsigned char wmR = GetPixelR(watermark_image, wx, wy);
            unsigned char wmG = GetPixelG(watermark_image, wx, wy);
            unsigned char wmB = GetPixelB(watermark_image, wx, wy);
            
            if (wmR == 0 && wmG == 0 && wmB == 0) { // Apply watermark effect where watermark is black
                unsigned char origR = GetPixelR(image, x, y);
                unsigned char origG = GetPixelG(image, x, y);
                unsigned char origB = GetPixelB(image, x, y);
                
                // Apply watermark effect with saturation handling
                unsigned char newR = (origR * 1.45 > 255) ? 255 : (unsigned char)(origR * 1.45);
                unsigned char newG = (origG * 1.45 > 255) ? 255 : (unsigned char)(origG * 1.45);
                unsigned char newB = (origB * 1.45 > 255) ? 255 : (unsigned char)(origB * 1.45);
                
                SetPixelR(image, x, y, newR);
                SetPixelG(image, x, y, newG);
                SetPixelB(image, x, y, newB);
            }
        }
    }
    
    return image;
}

Image *RotateBy90(Image *image, int direction) {
    assert(image != NULL);
    assert(image->R != NULL && image->G != NULL && image->B != NULL);
    
    unsigned int oldWidth = ImageWidth(image);
    unsigned int oldHeight = ImageHeight(image);
    
    // Create new image with swapped width and height
    Image *rotatedImage = CreateImage(oldHeight, oldWidth);
    
    // Rotate image
    for (unsigned int y = 0; y < oldHeight; y++) {
        for (unsigned int x = 0; x < oldWidth; x++) {
            unsigned char r = GetPixelR(image, x, y);
            unsigned char g = GetPixelG(image, x, y);
            unsigned char b = GetPixelB(image, x, y);
            
            if (direction == 0) { // Clockwise
                SetPixelR(rotatedImage, oldHeight - 1 - y, x, r);
                SetPixelG(rotatedImage, oldHeight - 1 - y, x, g);
                SetPixelB(rotatedImage, oldHeight - 1 - y, x, b);
            } else { // Counterclockwise
                SetPixelR(rotatedImage, y, oldWidth - 1 - x, r);
                SetPixelG(rotatedImage, y, oldWidth - 1 - x, g);
                SetPixelB(rotatedImage, y, oldWidth - 1 - x, b);
            }
        }
    }
    DeleteImage(image);
    image=NULL;
    return rotatedImage;
}
