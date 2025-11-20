/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* DIPs.c: source file for DIP operations                            */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "DIPs.h"
#include "Constants.h"
#include "string.h"
#include <math.h>

/* Black and White */
Image *BlackNWhite(Image *image)
{
	assert(image);
	int x, y, tmp;

	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image); x++) {
		tmp = (GetPixelR(image, x, y) + GetPixelG(image, x, y) + GetPixelB(image, x, y)) / 3;
			SetPixelR(image, x, y, tmp);
			SetPixelG(image, x, y, tmp);
		SetPixelB(image, x, y, tmp);
		}
	}
	return image;
}

/* Mirror image horizontal  */
Image *HMirror(Image *image) {
	assert(image);
	int x, y;
	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image)/2; x++) {
			SetPixelG(image, ImageWidth(image) - 1 - x, y, GetPixelG(image, x, y));
			SetPixelB(image, ImageWidth(image) - 1 - x, y, GetPixelB(image, x, y));
			SetPixelR(image, ImageWidth(image) - 1 - x, y, GetPixelR(image, x, y));
		}
	}
	return image;
}

/* Edge detection */
Image *Edge(Image *image) {
	assert(image);
	Image *tmpImage = CreateImage(ImageWidth(image), ImageHeight(image));
	assert(tmpImage);

	int x, y, m, n;
	memcpy(tmpImage->R, image->R, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	memcpy(tmpImage->G, image->G, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	memcpy(tmpImage->B, image->B, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	for (y = 1; y < ImageHeight(image) - 1; y++) {
		for (x = 1; x < ImageWidth(image) - 1; x++) {
			int tmpR = 0;
			int tmpG = 0;
			int tmpB = 0;
			for (m = -1; m <= 1; m++) {
				for (n = -1; n <= 1; n++) {
					tmpR += (GetPixelR(tmpImage, x, y) - GetPixelR(tmpImage, x + n, y + m));
					tmpG += (GetPixelG(tmpImage, x, y) - GetPixelG(tmpImage, x + n, y + m));
					tmpB += (GetPixelB(tmpImage, x, y) - GetPixelB(tmpImage, x + n, y + m));
				}
			}
			SetPixelR(image, x, y, (tmpR > 255) ? 255 : (tmpR < 0) ? 0 : tmpR);
			SetPixelG(image, x, y, (tmpG > 255) ? 255 : (tmpG < 0) ? 0 : tmpG);
			SetPixelB(image, x, y, (tmpB > 255) ? 255 : (tmpB < 0) ? 0 : tmpB);
		}
	}
	for (y = 0; y < ImageHeight(image); y++) {
		x = 0;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
		x = ImageWidth(image) - 1;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
	}
	for (x = 0; x < ImageWidth(image); x++) {
		y = 0;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
		y = ImageHeight(image) - 1;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
	}
	DeleteImage(tmpImage);
	tmpImage = NULL;
	return image;
}

/* Add a watermark to an image */
Image *Watermark(Image *image, const Image *watermark,
	 unsigned int topLeftX, unsigned int topLeftY)
{
	assert(image);
    assert(watermark);

    unsigned int wW = ImageWidth(watermark);
    unsigned int wH = ImageHeight(watermark);
    unsigned int iW = ImageWidth(image);
    unsigned int iH = ImageHeight(image);

    for (unsigned int y = 0; y < wH; y++) {
        for (unsigned int x = 0; x < wW; x++) {
            unsigned int ix = topLeftX + x;
            unsigned int iy = topLeftY + y;

            if (ix < iW && iy < iH) {
                unsigned char r = (GetPixelR(image, ix, iy) + GetPixelR(watermark, x, y)) / 2;
                unsigned char g = (GetPixelG(image, ix, iy) + GetPixelG(watermark, x, y)) / 2;
                unsigned char b = (GetPixelB(image, ix, iy) + GetPixelB(watermark, x, y)) / 2;

                SetPixelR(image, ix, iy, r);
                SetPixelG(image, ix, iy, g);
                SetPixelB(image, ix, iy, b);
            }
        }
    }

    return image;
}

/* Spotlight */
Image *Spotlight(Image *image, int centerX, int centerY, unsigned int radius)
{
	assert(image);

    int w = ImageWidth(image);
    int h = ImageHeight(image);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int dx = x - centerX;
            int dy = y - centerY;
            double dist = sqrt(dx * dx + dy * dy);

            if (dist > radius) {
                SetPixelR(image, x, y, 0);
                SetPixelG(image, x, y, 0);
                SetPixelB(image, x, y, 0);
            }
        }
    }

    return image;
}

/* Rotate and zoom an image */
Image *Rotate(Image *image, double Angle, double ScaleFactor)
{
	assert(image);

    int w = ImageWidth(image);
    int h = ImageHeight(image);
    Image *newImage = CreateImage(w, h);
    assert(newImage);

    int centerX = w / 2;
    int centerY = h / 2;
    double radians = Angle * M_PI / 180.0;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int dx = x - centerX;
            int dy = y - centerY;

            int srcX = (int)(dx / ScaleFactor * cos(-radians) - dy / ScaleFactor * sin(-radians) + centerX);
            int srcY = (int)(dx / ScaleFactor * sin(-radians) + dy / ScaleFactor * cos(-radians) + centerY);

            if (srcX >= 0 && srcX < w && srcY >= 0 && srcY < h) {
                SetPixelR(newImage, x, y, GetPixelR(image, srcX, srcY));
                SetPixelG(newImage, x, y, GetPixelG(image, srcX, srcY));
                SetPixelB(newImage, x, y, GetPixelB(image, srcX, srcY));
            } else {
                SetPixelR(newImage, x, y, 0);
                SetPixelG(newImage, x, y, 0);
                SetPixelB(newImage, x, y, 0);
            }
        }
    }

    DeleteImage(image);
    return newImage;
}
/* EOF */
