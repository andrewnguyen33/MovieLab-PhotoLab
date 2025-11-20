/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* MovieLab.c: source file for the main function                     */
/* Please use this template for your HW5.                            */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "FileIO.h"
#include "DIPs.h"
#include "Movie.h"
#include "Constants.h"
#include "Image.h"

/* Load one movie frame from the input file */
YUVImage* LoadOneFrame(const char* fname, int n,
	unsigned int width, unsigned height);

/* Load the movie frames from the input file */
Movie *LoadMovie(const char *fname, int frameNum,
	unsigned int width, unsigned height);

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVImage *image, const char *fname, FILE *file);

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, Movie *movie);

/* Print the command-line arguments usage of the program */
void PrintUsage();

int main(int argc, char *argv[])
{
	int x = 0;
	char *fin = NULL;
	char *fout = NULL;
	
	unsigned int width = 512;
	unsigned int height = 288;
	int frameNum = 0;
	int bw = 0, hmirror = 0, edge = 0;
	int cropStart = -1, cropEnd = -1;
	int fastFactor = 1;
	int reverse = 0;
	char *watermarkFile = NULL;
	int spotlight = 0;
	int rotate = 0;
	/* the while loop to check options in the command line */
	while (x < argc) {
		/* the input file name */
		if (strcmp(argv[x], "-i") == 0) {
			if (x < argc - 1) {
				fin = argv[x + 1];
			} /*fi*/
			else {
				printf("Missing argument for the input file name!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

		/* the output file name */
		if (strcmp(argv[x], "-o") == 0) {
			if (x < argc - 1) {
				fout = argv[x + 1];
			} /*fi*/
			else {
				printf("Missing argument for the output file name!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

		/* the help option */
		if (strcmp(argv[x], "-h") == 0) {
			PrintUsage();
			return 0;
		} /*fi*/

		if (strcmp(argv[x], "-f") == 0) {
			if (x < argc - 1) {
				frameNum = atoi(argv[x + 1]);
				x += 2;
				continue;
			} else {
				printf("Missing argument for frame number!\n");
				return 5;
			}
		}

		if (strcmp(argv[x], "-s") == 0 && x + 1 < argc) {
			if (sscanf(argv[++x], "%ux%u", &width, &height) != 2) {
				printf("Invalid format for -s. Use WidthxHeight (e.g. 512x288)\n");
				return 5;
			}
			x++;
			continue;
		}
		
		/* black and white filter */
		if (strcmp(argv[x], "-bw") == 0) {
			bw = 1;
			x++;
			continue;
		}

		/* horizontal mirror filter */
		if (strcmp(argv[x], "-hmirror") == 0) {
			hmirror = 1;
			x++;
			continue;
		}

		/* edge detection filter */
		if (strcmp(argv[x], "-edge") == 0) {
			edge = 1;
			x++;
			continue;
		}
		if (strcmp(argv[x], "-crop") == 0 && x + 1 < argc) {
			if (sscanf(argv[++x], "%d-%d", &cropStart, &cropEnd) != 2) {
				printf("Invalid format for -crop. Use start-end\n");
				return 5;
			}
			x++;
			continue;
		}
		if (strcmp(argv[x], "-fast") == 0 && x + 1 < argc) {
			fastFactor = atoi(argv[++x]);
			if (fastFactor < 1) {
				printf("Invalid fast forward factor (must be >= 1)\n");
				return 5;
			}
			x++;
			continue;
		}
		if (strcmp(argv[x], "-reverse") == 0) {
			reverse = 1;
			x++;
			continue;
		}
		if (strcmp(argv[x], "-watermark") == 0 && x + 1 < argc) {
			watermarkFile = argv[++x];
			x++;
			continue;
		}
		if (strcmp(argv[x], "-spotlight") == 0) {
			spotlight = 1;
			x++;
			continue;
		}

		if (strcmp(argv[x], "-rotate") == 0) {
			rotate = 1;
			x++;
			continue;
		}

		x++;
	} /* elihw */

	if (fin == NULL) {
		printf("Missing argument for the input file name!\n");
		PrintUsage();
		return 5;
	}

	if (fout == NULL) {
		printf("Missing argument for the output file name!\n");
		PrintUsage();
		return 5;
	}

	Movie *movie = LoadMovie(fin, frameNum, width, height);
	if (!movie) {
		printf("Error loading movie. Aborting.\n");
		return 1;
	}
	printf("The movie file %s has been read successfully!\n", fin);
	//go through each filter
	if (bw) {
		YUV2RGBMovie(movie);
		IENTRY *entry = movie->Frames->First;
		while (entry) {
			entry->image = (void *)BlackNWhite((Image *)entry->image);
			entry = entry->Next;
		}
		RGB2YUVMovie(movie);
		printf("Operation Black and White is done!\n");
	}
	
	if (hmirror) {
		YUV2RGBMovie(movie);
		IENTRY *entry = movie->Frames->First;
		while (entry) {
			entry->image = (void *)HMirror((Image *)entry->image);
			entry = entry->Next;
		}
		RGB2YUVMovie(movie);
		printf("Operation HMirror is done!\n");
	}
	
	if (edge) {
		YUV2RGBMovie(movie);
		IENTRY *entry = movie->Frames->First;
		while (entry) {
			entry->image = (void *)Edge((Image *)entry->image);
			entry = entry->Next;
		}
		RGB2YUVMovie(movie);
		printf("Operation Edge is done!\n");
	}
	if (cropStart >= 0 && cropEnd >= 0) {
		CropImageList(movie->Frames, cropStart, cropEnd);
		printf("Operation Crop is done! New number of frames is %u.\n", movie->Frames->Length);
	}

	if (fastFactor > 1) {
		FastImageList(movie->Frames, fastFactor);
		printf("Operation Fast Forward is done! New number of frames is %u.\n", movie->Frames->Length);
	}

	if (reverse) {
		ReverseImageList(movie->Frames);
		printf("Operation Reverse is done!\n");
	}

	if (watermarkFile) {
		YUV2RGBMovie(movie);
		Image *wm = LoadImage(watermarkFile);
		assert(wm);
		srand(time(NULL));
		unsigned int x = rand() % (width - wm->W);
		unsigned int y = rand() % (height - wm->H);
		IENTRY *entry = movie->Frames->First;
		int count = 0;
		while (entry) {
			if (count % WATERMARK_PER_FRAMES == 0) {
				x = rand() % (width - wm->W);
				y = rand() % (height - wm->H);
			}
			entry->image = (void *)Watermark((Image *)entry->image, wm, x, y);
			entry = entry->Next;
			count++;
		}
		DeleteImage(wm);
		RGB2YUVMovie(movie);
		printf("Operation Watermark is done!\n");
	}

	if (spotlight) {
		if (movie->Frames->Length < 2) {
			printf("Not enough frames for Spotlight operation.\n");
		} else {
			YUV2RGBMovie(movie);
			IENTRY *first = movie->Frames->First;
			IENTRY *last = movie->Frames->Last;
			Image *firstCopy = (Image *)first->image;
			Image *lastCopy = (Image *)last->image;
			for (int i = 0; i < 20; i++) {
				unsigned int radius = (unsigned int)((sqrt(width * width + height * height) / 2.0) * (i + 1) / 20.0);
				PrependRGBImage(movie->Frames, CopyRGBImage(Spotlight(CopyRGBImage(firstCopy), width / 2, height / 2, radius)));
			}
			for (int i = 19; i >= 0; i--) {
				unsigned int radius = (unsigned int)((sqrt(width * width + height * height) / 2.0) * (i + 1) / 20.0);
				AppendRGBImage(movie->Frames, CopyRGBImage(Spotlight(CopyRGBImage(lastCopy), width / 2, height / 2, radius)));
			}
			RGB2YUVMovie(movie);
			printf("Operation Spotlight is done!\n");
		}
	}

	if (rotate) {
		YUV2RGBMovie(movie);
		IENTRY *first = movie->Frames->First;
		IENTRY *last = movie->Frames->Last;
		Image *firstCopy = (Image *)first->image;
		Image *lastCopy = (Image *)last->image;
		for (int i = 0; i < 20; i++) {
			double angle = i * (360.0 / 20);
			double scale = 1.0 + 0.05 * i;
			PrependRGBImage(movie->Frames, Rotate(CopyRGBImage(firstCopy), angle, scale));
		}
		for (int i = 19; i >= 0; i--) {
			double angle = i * (360.0 / 20);
			double scale = 1.0 + 0.05 * i;
			AppendRGBImage(movie->Frames, Rotate(CopyRGBImage(lastCopy), angle, scale));
		}
		RGB2YUVMovie(movie);
		printf("Operation Rotate is done!\n");
	}

	SaveMovie(fout, movie);
	DeleteMovie(movie);

	fin = NULL;
	fout = NULL;

	return 0;
}

void PrintUsage()
{
	printf("\nUsage: MovieLab -i <file> -o <file> -f <framenum> [options]\n"
	       "Options:\n"
	       "-s <WidthxHeight>     To set the resolution of the input stream (WidthxHeight)\n"
	       "-bw                   Activate the black and white filter on every movie frame\n"
	       "-hmirror              Activate horizontal mirror on every movie frame\n"
	       "-edge                 Activate the edge filter on every movie frame\n"
	       "-crop <start-end>     Crop the movie frames from <start> to <end>\n"
	       "-fast <factor>        Fast forward the movie by <factor>\n"
	       "-reverse              Reverse the frame order of the input movie\n"
	       "-watermark <file>     Add a watermark from <file> to every movie frame\n"
	       "-spotlight            Spotlight the fade in and fade out\n"
	       "-rotate               Rotate and zoom in/out the input movie\n"
	       "-h                    Display this usage information\n"
	      );
}

/* Load one movie frame from the input file */
YUVImage* LoadOneFrame(const char* fname, int n,
	unsigned int width, unsigned height)
{
	FILE *file;
	unsigned int x, y;
	unsigned char c;
	YUVImage* YUVimage;

	/* Check errors */
	assert(fname);
	assert(n >= 0);

	YUVimage = CreateYUVImage(width, height);
	if (YUVimage == NULL) {
		return NULL;
	}

	/* Open the input file */
	file = fopen(fname, "r");
	if (file == NULL) {
		DeleteYUVImage(YUVimage);
		return NULL;
	}

	/* Find the desired frame */
	fseek(file, 1.5 * n * width * height, SEEK_SET);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			c = fgetc(file);
			SetPixelY(YUVimage, x, y, c);
		} /*rof*/
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelU(YUVimage, x, y, c);
			SetPixelU(YUVimage, x + 1, y, c);
			SetPixelU(YUVimage, x, y + 1, c);
			SetPixelU(YUVimage, x + 1, y + 1, c);
		}
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelV(YUVimage, x, y, c);
			SetPixelV(YUVimage, x + 1, y, c);
			SetPixelV(YUVimage, x, y + 1, c);
			SetPixelV(YUVimage, x + 1, y + 1, c);
		}
	}

	/* Check errors */
	assert(ferror(file) == 0);

	/* Close the input file and return */
	fclose(file);
	file = NULL;
	return YUVimage;
}

/* Load the movie frames from the input file */
Movie *LoadMovie(const char *fname, int frameNum,
	unsigned int width, unsigned height)
{
	assert(fname);
    assert(frameNum > 0);

    Movie *movie = CreateMovie();
    assert(movie);

    for (int i = 0; i < frameNum; i++) {
        YUVImage *frame = LoadOneFrame(fname, i, width, height);
        if (frame == NULL) {
            printf("Error: Failed to load frame %d from %s\n", i, fname);
            DeleteMovie(movie);
            return NULL;
        }
        AppendYUVImage(movie->Frames, frame);
    }

    return movie;
}

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, Movie *movie)
{
	int count;
	FILE *file;
	IENTRY *curr;

	/* Open the output file */
	file = fopen(fname, "w");
	if (file == NULL) {
		return 1;
	}

	count = 0;
	curr = movie->Frames->First;
	while (curr != NULL) {

		SaveOneFrame(curr->image, fname, file);
		curr = curr->Next;
		count++;
	}

	fclose(file);
	file = NULL;

	printf("The movie file %s has been written successfully!\n", fname);
	printf("%d frames are written to the file %s in total.\n", count, fname);
	return 0;
}

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVImage *image, const char *fname, FILE *file)
{
	int x, y;
	for (y = 0; y < image->H; y++) {
		for (x = 0; x < image->W; x++) {
			fputc(GetPixelY(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelU(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelV(image, x, y), file);
		}
	}
}

/* EOF */
