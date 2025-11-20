#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Image.h"

unsigned char GetPixelR(const Image *image, unsigned int x, unsigned int y) {
    assert(image != NULL); 
    assert(image->R != NULL);
    assert(x < image->W && y < image->H);
    return image->R[x + y * image->W];
}

unsigned char GetPixelG(const Image *image, unsigned int x, unsigned int y) {
    assert(image != NULL);
    assert(image->G != NULL);
    assert(x < image->W && y < image->H);
    return image->G[x + y * image->W];
}

unsigned char GetPixelB(const Image *image, unsigned int x, unsigned int y) {
    assert(image != NULL);
    assert(image->B != NULL);
    assert(x < image->W && y < image->H);
    return image->B[x + y * image->W];
}

void SetPixelR(Image *image, unsigned int x, unsigned int y, unsigned char r) {
    assert(image != NULL);
    assert(image->R != NULL);
    assert(x < image->W && y < image->H);
    image->R[x + y * image->W] = r;
}

void SetPixelG(Image *image, unsigned int x, unsigned int y, unsigned char g) {
    assert(image != NULL);
    assert(image->G != NULL);
    assert(x < image->W && y < image->H);
    image->G[x + y * image->W] = g;
}

void SetPixelB(Image *image, unsigned int x, unsigned int y, unsigned char b) {
    assert(image != NULL);
    assert(image->B != NULL);
    assert(x < image->W && y < image->H);
    image->B[x + y * image->W] = b;
}

unsigned int ImageWidth(const Image *image) {
    assert(image != NULL);
    return image->W;
}

unsigned int ImageHeight(const Image *image) {
    assert(image != NULL);
    return image->H;
}

Image *CreateImage(unsigned int Width, unsigned int Height) {
    if (Width == 0 || Height == 0) {
        return NULL;
    }

    Image *img = (Image *)malloc(sizeof(Image));
    if (!img) {
        return NULL;
    }

    img->W = Width;
    img->H = Height;
    img->R = NULL;
    img->G = NULL;
    img->B = NULL;

    img->R = (unsigned char *)malloc(Width * Height * sizeof(unsigned char));
    img->G = (unsigned char *)malloc(Width * Height * sizeof(unsigned char));
    img->B = (unsigned char *)malloc(Width * Height * sizeof(unsigned char));

    if (!img->R || !img->G || !img->B) {
        DeleteImage(img);  // Use DeleteImage to free all allocated memory
        return NULL;
    }
    return img;
}

void DeleteImage(Image *image) {
    if (image) {
        if (image->R) {
            free(image->R);
            image->R = NULL;
        }
        if (image->G) {
            free(image->G);
            image->G = NULL;
        }
        if (image->B) {
            free(image->B);
            image->B = NULL;
        }
        free(image);
        image = NULL; // Prevent dangling pointer
    }
}