/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* Movie.c: source file for basic movie manipulations                */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "Movie.h"

/* Allocate the memory space for the movie and the memory space */
/* for the frame list. Return the pointer to the movie. */
Movie *CreateMovie(void)
{
	Movie *movie = (Movie *)malloc(sizeof(Movie));
	if(!movie)
	{ 
        perror("Out of memory! Aborting...");  // Print an error message
        exit(10);  // Terminate the program with exit code 10
    }
	movie->Frames = CreateImageList();
	return movie;
}

/* Release the memory space for the frame list. */
/* Release the memory space for the movie. */
void DeleteMovie(Movie *movie)
{
	assert(movie);
    DeleteImageList(movie->Frames);
    free(movie);
}
/*clip function to limit values between 0 and 255 */
static int clip(int value) 
{
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}

/* Convert a YUV movie to a RGB movie */
void YUV2RGBMovie(Movie *movie)
{
	assert(movie);
    IENTRY *entry = NULL;
	entry= movie->Frames->First;

    while (entry) {
        YUVImage *yuv = (YUVImage *)entry->image;
        assert(yuv);

        unsigned int w = yuv->W;
        unsigned int h = yuv->H;
        Image *rgb = CreateImage(w, h);
        assert(rgb);

        for (unsigned int y = 0; y < h; y++) {
            for (unsigned int x = 0; x < w; x++) {
                int Y = GetPixelY(yuv, x, y);
                int U = GetPixelU(yuv, x, y);
                int V = GetPixelV(yuv, x, y);

                int C = Y - 16;
                int D = U - 128;
                int E = V - 128;

                int R = (298 * C + 409 * E + 128) >> 8;
                int G = (298 * C - 100 * D - 208 * E + 128) >> 8;
                int B = (298 * C + 516 * D + 128) >> 8;

                SetPixelR(rgb, x, y, clip(R));
                SetPixelG(rgb, x, y, clip(G));
                SetPixelB(rgb, x, y, clip(B));
            }
        }

        DeleteYUVImage(yuv);
        entry->image = (void *)rgb;
        entry = entry->Next;
    }
}

/* Convert a RGB movie to a YUV movie */
void RGB2YUVMovie(Movie *movie)
{
	assert(movie);
    IENTRY *entry = NULL;
	entry= movie->Frames->First;

    while (entry) {
        Image *rgb = (Image *)entry->image;
        assert(rgb);

        unsigned int w = ImageWidth(rgb);
        unsigned int h = ImageHeight(rgb);
        YUVImage *yuv = CreateYUVImage(w, h);
        assert(yuv);

        for (unsigned int y = 0; y < h; y++) {
            for (unsigned int x = 0; x < w; x++) {
                int R = GetPixelR(rgb, x, y);
                int G = GetPixelG(rgb, x, y);
                int B = GetPixelB(rgb, x, y);

                int Y = (( 66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
                int U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
                int V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

                SetPixelY(yuv, x, y, clip(Y));
                SetPixelU(yuv, x, y, clip(U));
                SetPixelV(yuv, x, y, clip(V));
            }
        }

        DeleteImage(rgb);
        entry->image = (void *)yuv;
        entry = entry->Next;
    }
}

/* EOF */
