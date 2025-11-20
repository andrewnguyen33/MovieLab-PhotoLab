#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "Constants.h"
#include "Advanced.h"
#include "DIPs.h"
#include "FileIO.h"
#include "Test.h"

void PrintMenu();
int AutoTest(void);


int main() {
    int option;			/* user input option */
	char fname[SLEN];		/* input file name */
	Image *image= NULL;
	
	#ifdef DEBUG
    AutoTest();
    return 0;  // Exit after testing
	#endif

	PrintMenu();
	printf("Please make your choice: ");
	scanf("%d", &option);


    int target_r, target_g, target_b, threshold, replace_r, 
    replace_g, replace_b; // ColorFilter Parameters
    //Add border parameters
	char color[20];
    int border_width;
    //Pixelate parameter
    int block_size;
    //Shift Parameters
    int shiftX ,shiftY;
	//Crop parameters
	int x, y, W, H;
	//Resize parameters
	int newWidth, newHeight;
	//Watermark parameter
	const Image *watermark_image;
	watermark_image=NULL;
	//Rotateby90 parameter
	int rotateDirection;


	while (option != 18) {
		if (option == 1) {
			DeleteImage(image);
			printf("Please input the file name to load: ");
			scanf("%75s", fname);
			image = LoadImage(fname);
		}
		else if (option ==17){
			int result = AutoTest();
					if (result != 0) {
						printf("AutoTest failed, error code %d.\n", result);
					} else {
						printf("AutoTest finished successfully.\n");
					}
		}
		/* menu item 2 - 16 requires image is loaded first */
		else if (option >= 2 && option <= 16) 
			if (!image)	 {
				printf("No image to process!\n");
			}
			/* now image is loaded */
			else {
				switch(option) {
					case 2:
						printf("Please input the file name to save: ");
						scanf("%75s", fname);
						SaveImage(fname, image);
						break;
					case 3:
						image = BlackNWhite(image);
						printf("\"Black & White\" operation is done!\n");
						break;
					case 4:
						image = Negative(image);
						printf("\"Negative\" operation is done!\n");
						break;
					case 5:
						printf("Enter Red   component for the target color: ");
						scanf("%d", &target_r);
						printf("Enter Green component for the target color: ");
						scanf("%d", &target_g);
						printf("Enter Blue  component for the target color: ");
						scanf("%d", &target_b);
						printf("Enter threshold for the color difference: ");
						scanf("%d", &threshold);
						printf("Enter value for Red component in the target color: ");
						scanf("%d", &replace_r);
						printf("Enter value for Green component in the target color: ");
						scanf("%d", &replace_g);
						printf("Enter value for Blue  component in the target color: ");
						scanf("%d", &replace_b);
						image = ColorFilter(image, target_r, target_g, target_b, threshold, replace_r, replace_g, replace_b);
						printf("\"Color Filter\" operation is done!\n");
						break;
					case 6:
						image = Edge(image);
						printf("\"Edge\" operation is done!\n");
						break;
					case 7:
						image = Shuffle(image);
						printf("\"Shuffle\" operation is done!\n");
						break;
					case 8:
						image = VFlip(image);
						printf("\"VFlip\" operation is done!\n");
						break;
					case 9:
						image = HMirror(image);
						printf("\"HMirror\" operation is done!\n");
						break;
					case 10:
						printf("Enter border width: ");
						scanf("%d", &border_width);
						printf("Available border colors : black, white, red, green, blue, yellow, cyan, pink, orange\n");
						printf("Select border color from the options: ");
						image = AddBorder(image, color, border_width);
						printf("\"Border\" operation is done!\n");
                        break;
					case 11:
						printf("Enter block size: "); //input for blocks
						scanf("%d", &block_size);
						image = Pixelate(image, block_size);
						printf("\"Pixelate\" operation is done!\n");
						break;
					case 12:
						printf("Enter X direction shift: ");
						scanf("%d", &shiftX);
						printf("Enter Y direction shift: ");
						scanf("%d", &shiftY);
						image = Shift(image, shiftX, shiftY);
						printf("\"Shift\" operation is done!\n");
						break;
					case 13:
						printf("Please enter the X offset value: ");
						scanf("%d", &x);
						printf("Please enter the Y offset value: ");
						scanf("%d", &y);
						printf("Please input the crop width: ");
						scanf("%d", &W);
						printf("Please input the crop height: ");
						scanf("%d", &H);
						image = Crop(image, x, y, W, H);
						printf("\"Crop\" operation is done!\n");
						break;
					case 14:
						printf("Please input the new image width: ");
						scanf("%d", &newWidth);
						printf("Please input the new image height: ");
						scanf("%d", &newHeight);
						printf("\"Resizing the image\" operation is done!\n");
						break;
					case 15:
						image= Watermark(image, watermark_image);
						printf("\"Watermark\" operation is done!\n");
						break;
					case 16:
						printf("Please input the direction of rotation (0:clockwise, 1:counterclockwise): ");
						scanf("%d", &rotateDirection);
						image= RotateBy90(image,rotateDirection);
						printf("\"RotateBy90\" operation is done!\n");
						break;
					default:
						printf("Invalid selection!\n");
						break;
				}
			}
		
		else {
			printf("Invalid selection!\n");
		}

		/* Process finished, waiting for another input */
		PrintMenu();
		printf("Please make your choice: ");
		scanf("%d", &option);
	}
	printf("You exit the program.\n");
	if (image) {
        DeleteImage(image);
    }
    return 0;
}


void PrintMenu() {
	printf("\n----------------------------\n");
	printf(" 1: Load a PPM image\n");
	printf(" 2: Save an image in PPM and JPEG format\n");
	printf(" 3: Change a color image to Black & White\n");
	printf(" 4: Make a negative of an image\n");
	printf(" 5: Color filter an image\n");
	printf(" 6: Sketch the edge of an image\n");
	printf(" 7: Shuffle an image\n");
	printf(" 8: Flip an image vertically\n");
	printf(" 9: Mirror an image horizontally\n");
	printf("10: Add border to an image\n");
    printf("11: Pixelate the image\n");
	printf("12: Shift an image\n");
	printf("13: Crop an image\n");
	printf("14: Resize an image\n");
	printf("15: Add Watermark to an image\n");
	printf("16: Rotate an image by 90 degree\n");
	printf("17: Test all functions\n");
    printf("18: Exit\n");
}