#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "DIPs.h"

Image *BlackNWhite(Image *image){
    //make sure image is within bounds
    assert(image != NULL);
    assert(image->R != NULL && image->G != NULL && image->B != NULL);
    
    unsigned int width = ImageWidth(image);
    unsigned int height = ImageHeight(image);
    //gets all pixels and divide by 3
    for (unsigned int y = 0; y < height; y++) { 
        for (unsigned int x = 0; x < width; x++) {
            unsigned char r = GetPixelR(image, x, y);
            unsigned char g = GetPixelG(image, x, y);
            unsigned char b = GetPixelB(image, x, y);
            
            unsigned char gray = (r + g + b) / 3;
            //sets image to grayscale
            SetPixelR(image, x, y, gray);
            SetPixelG(image, x, y, gray);
            SetPixelB(image, x, y, gray);
        }
    }
    
    return image;
}


/* Reverse image color */
Image *Negative(Image *image){
    //makes sure image is within bounds
    assert(image != NULL);
    assert(image->R != NULL && image->G != NULL && image->B != NULL);
    
    unsigned int width = ImageWidth(image);
    unsigned int height = ImageHeight(image);
    //loops through RGB values
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            unsigned char r = GetPixelR(image, x, y);
            unsigned char g = GetPixelG(image, x, y);
            unsigned char b = GetPixelB(image, x, y);
            //sets image to the inverted version
            SetPixelR(image, x, y, 255 - r);
            SetPixelG(image, x, y, 255 - g);
            SetPixelB(image, x, y, 255 - b);
        }
    }
    
    return image;
}


/* Color filter */
Image *ColorFilter(Image *image, int target_r, int target_g, int target_b, int threshold, int replace_r, int replace_g, int replace_b){
    //makes sure image is within bounds
    assert(image != NULL);
    assert(image->R != NULL && image->G != NULL && image->B != NULL);
    
    unsigned int width = ImageWidth(image);
    unsigned int height = ImageHeight(image);
    
    // Ensure replacement colors are within range
    replace_r = (replace_r > 255) ? 255 : (replace_r < 0) ? 0 : replace_r;
    replace_g = (replace_g > 255) ? 255 : (replace_g < 0) ? 0 : replace_g;
    replace_b = (replace_b > 255) ? 255 : (replace_b < 0) ? 0 : replace_b;
    
    //loops through pixels
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            unsigned char r = GetPixelR(image, x, y);
            unsigned char g = GetPixelG(image, x, y);
            unsigned char b = GetPixelB(image, x, y);
            //replaces color with replacement color
            if (abs(r - target_r) <= threshold &&
                abs(g - target_g) <= threshold &&
                abs(b - target_b) <= threshold) {
                SetPixelR(image, x, y, replace_r);
                SetPixelG(image, x, y, replace_g);
                SetPixelB(image, x, y, replace_b);
            }
        }
    }
    
    return image;
}


/* Edge detection */
Image *Edge(Image *image){
    assert(image != NULL);
    assert(image->R != NULL && image->G != NULL && image->B != NULL);
    
    unsigned int width = ImageWidth(image);
    unsigned int height = ImageHeight(image);
    //store orginal pixel values in temp memory
    int *tempR = (int *)malloc(width * height * sizeof(int));
    int *tempG = (int *)malloc(width * height * sizeof(int));
    int *tempB = (int *)malloc(width * height * sizeof(int));
    
    if (!tempR || !tempG || !tempB) {
        free(tempR);
        free(tempG);
        free(tempB);
        return image;
    }
    //copy image data into temp memory
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            tempR[x + y * width] = GetPixelR(image, x, y);
            tempG[x + y * width] = GetPixelG(image, x, y);
            tempB[x + y * width] = GetPixelB(image, x, y);
        }
    }
    //apply edge detection filter
    for (unsigned int y = 1; y < height - 1; y++) {
        for (unsigned int x = 1; x < width - 1; x++) {
            int sumR = 8 * tempR[x + y * width];
            int sumG = 8 * tempG[x + y * width];
            int sumB = 8 * tempB[x + y * width];
            //subtract surrounding pixel values
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;
                    sumR -= tempR[(x + i) + (y + j) * width];
                    sumG -= tempG[(x + i) + (y + j) * width];
                    sumB -= tempB[(x + i) + (y + j) * width];
                }
            }
            // ensure values are within range
            SetPixelR(image, x, y, (sumR > 255) ? 255 : (sumR < 0) ? 0 : sumR);
            SetPixelG(image, x, y, (sumG > 255) ? 255 : (sumG < 0) ? 0 : sumG);
            SetPixelB(image, x, y, (sumB > 255) ? 255 : (sumB < 0) ? 0 : sumB);
        }
    }
    
    for (unsigned int x = 0; x < width; x++) {
        SetPixelR(image, x, 0, 0);
        SetPixelG(image, x, 0, 0);
        SetPixelB(image, x, 0, 0);
        SetPixelR(image, x, height - 1, 0);
        SetPixelG(image, x, height - 1, 0);
        SetPixelB(image, x, height - 1, 0);
    }
    for (unsigned int y = 0; y < height; y++) {
        SetPixelR(image, 0, y, 0);
        SetPixelG(image, 0, y, 0);
        SetPixelB(image, 0, y, 0);
        SetPixelR(image, width - 1, y, 0);
        SetPixelG(image, width - 1, y, 0);
        SetPixelB(image, width - 1, y, 0);
    }
    //free memory
    free(tempR);
    free(tempG);
    free(tempB);
    
    return image;
}


/* Flip image vertically */
Image *VFlip(Image *image){
        assert(image != NULL);
        assert(image->R != NULL && image->G != NULL && image->B != NULL);
        
        unsigned int width = ImageWidth(image);
        unsigned int height = ImageHeight(image);
        
        // Loop through half of the image height to swap rows
        for (unsigned int y = 0; y < height / 2; y++) {
            for (unsigned int x = 0; x < width; x++) {
                // Swap pixels between top and bottom
                unsigned char tempR = GetPixelR(image, x, y);
                unsigned char tempG = GetPixelG(image, x, y);
                unsigned char tempB = GetPixelB(image, x, y);
                
                unsigned int oppositeY = height - 1 - y;
                
                SetPixelR(image, x, y, GetPixelR(image, x, oppositeY));
                SetPixelG(image, x, y, GetPixelG(image, x, oppositeY));
                SetPixelB(image, x, y, GetPixelB(image, x, oppositeY));
                
                SetPixelR(image, x, oppositeY, tempR);
                SetPixelG(image, x, oppositeY, tempG);
                SetPixelB(image, x, oppositeY, tempB);
            }
        }
        
        return image;
    }
    

/* Mirror image vertically */
Image *HMirror(Image *image) {
    assert(image != NULL);
    assert(image->R != NULL && image->G != NULL && image->B != NULL);
    
    unsigned int width = ImageWidth(image);
    unsigned int height = ImageHeight(image);
    
    // Loop through each row to mirror the pixels horizontally
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width / 2; x++) {
            unsigned int oppositeX = width - 1 - x;
            
            // Copy left side to the right side to create the mirror effect
            SetPixelR(image, oppositeX, y, GetPixelR(image, x, y));
            SetPixelG(image, oppositeX, y, GetPixelG(image, x, y));
            SetPixelB(image, oppositeX, y, GetPixelB(image, x, y));
        }
    }
    
    return image;
}


/* Zoom an image */
Image *AddBorder(Image *image, char *color, int border_width){
    assert(image != NULL);
    assert(image->R != NULL && image->G != NULL && image->B != NULL);
    
    unsigned int width = ImageWidth(image);
    unsigned int height = ImageHeight(image);
    
    // Define color values for different border colors
    unsigned char border_r = 0, border_g = 0, border_b = 0;
    
    if (strcmp(color, "black") == 0) {
        border_r = border_g = border_b = 0;
    } else if (strcmp(color, "white") == 0) {
        border_r = border_g = border_b = 255;
    } else if (strcmp(color, "red") == 0) {
        border_r = 255; border_g = 0; border_b = 0;
    } else if (strcmp(color, "green") == 0) {
        border_r = 0; border_g = 255; border_b = 0;
    } else if (strcmp(color, "blue") == 0) {
        border_r = 0; border_g = 0; border_b = 255;
    } else if (strcmp(color, "yellow") == 0) {
        border_r = 255; border_g = 255; border_b = 0;
    } else if (strcmp(color, "cyan") == 0) {
        border_r = 0; border_g = 255; border_b = 255;
    } else if (strcmp(color, "pink") == 0) {
        border_r = 255; border_g = 192; border_b = 203;
    } else if (strcmp(color, "orange") == 0) {
        border_r = 255; border_g = 165; border_b = 0;
    }
    
    // Apply the border to the image
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            if (x < border_width || x >= width - border_width || y < border_width || y >= height - border_width) {
                SetPixelR(image, x, y, border_r);
                SetPixelG(image, x, y, border_g);
                SetPixelB(image, x, y, border_b);
            }
        }
    }
    return image;
}


/* Shuffle an image */
Image *Shuffle(Image *image){
    assert(image != NULL);
    assert(image->R != NULL && image->G != NULL && image->B != NULL);
    
    unsigned int width = ImageWidth(image);
    unsigned int height = ImageHeight(image);
    
    int blockSizeX = width / 4; // Divide width and height into 16 equal blocks
    int blockSizeY = height / 4;
    
    // Loop through the first 8 blocks and swap with target blocks
    for (int i = 0; i < 8; i++) {
        int target = 15 - i;  // Matching block to swap
        
        // Current block location
        int bx = (i % 4) * blockSizeX;
        int by = (i / 4) * blockSizeY;
        
        // Target block location
        int swapX = (target % 4) * blockSizeX;
        int swapY = (target / 4) * blockSizeY;
        
        // Swap pixel values between the two blocks
        for (int x = 0; x < blockSizeX; x++) {
            for (int y = 0; y < blockSizeY; y++) {
                unsigned char tempR = GetPixelR(image, bx + x, by + y);
                unsigned char tempG = GetPixelG(image, bx + x, by + y);
                unsigned char tempB = GetPixelB(image, bx + x, by + y);
                
                SetPixelR(image, bx + x, by + y, GetPixelR(image, swapX + x, swapY + y));
                SetPixelG(image, bx + x, by + y, GetPixelG(image, swapX + x, swapY + y));
                SetPixelB(image, bx + x, by + y, GetPixelB(image, swapX + x, swapY + y));
                
                SetPixelR(image, swapX + x, swapY + y, tempR);
                SetPixelG(image, swapX + x, swapY + y, tempG);
                SetPixelB(image, swapX + x, swapY + y, tempB);
            }
        }
    }
    
    return image;
}


Image *Pixelate(Image *image, int block_size){
    assert(image != NULL);
    assert(image->R != NULL && image->G != NULL && image->B != NULL);
    
    unsigned int width = ImageWidth(image);
    unsigned int height = ImageHeight(image);
    
    for (unsigned int x = 0; x < width; x += block_size) { // Loop through blocks
        for (unsigned int y = 0; y < height; y += block_size) {
            int sumR = 0, sumG = 0, sumB = 0, count = 0;
            
            // Compute average color of the block
            for (unsigned int i = 0; i < block_size && (x + i) < width; i++) {
                for (unsigned int j = 0; j < block_size && (y + j) < height; j++) {
                    sumR += GetPixelR(image, x + i, y + j);
                    sumG += GetPixelG(image, x + i, y + j);
                    sumB += GetPixelB(image, x + i, y + j);
                    count++;
                }
            }
            
            unsigned char avgR = sumR / count;
            unsigned char avgG = sumG / count;
            unsigned char avgB = sumB / count;
            
            // Assign each pixel in the block the average RGB value
            for (unsigned int i = 0; i < block_size && (x + i) < width; i++) {
                for (unsigned int j = 0; j < block_size && (y + j) < height; j++) {
                    SetPixelR(image, x + i, y + j, avgR);
                    SetPixelG(image, x + i, y + j, avgG);
                    SetPixelB(image, x + i, y + j, avgB);
                }
            }
        }
    }
    
    return image;
}

Image *Shift(Image *image, int shiftX, int shiftY){
    assert(image != NULL);
    assert(image->R != NULL && image->G != NULL && image->B != NULL);
    
    unsigned int width = ImageWidth(image);
    unsigned int height = ImageHeight(image);
    
    // Allocate memory for the shifted image
    unsigned char *tempR = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    unsigned char *tempG = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    unsigned char *tempB = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    
    if (!tempR || !tempG || !tempB) {
        free(tempR);
        free(tempG);
        free(tempB);
        return image;
    }
    
    // Apply shift with wrap-around effect
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            unsigned int newX = (x + shiftX) % width;
            unsigned int newY = (y + shiftY) % height;
            
            tempR[newX + newY * width] = GetPixelR(image, x, y);
            tempG[newX + newY * width] = GetPixelG(image, x, y);
            tempB[newX + newY * width] = GetPixelB(image, x, y);
        }
    }
    
    // Copy shifted values back to the original image
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            SetPixelR(image, x, y, tempR[x + y * width]);
            SetPixelG(image, x, y, tempG[x + y * width]);
            SetPixelB(image, x, y, tempB[x + y * width]);
        }
    }
    
    // Free temporary memory
    free(tempR);
    free(tempG);
    free(tempB);
    return image;
}


