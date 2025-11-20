/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* ImageList.c: source file for image list manipulations             */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "ImageList.h"

/* Create a new image list */
ILIST *CreateImageList(void)
{
	ILIST *l;  // Declare a pointer to an ILIST structure
    l = malloc(sizeof(ILIST));  // Allocate memory dynamically for an SLIST structure
    if (!l)  // Check if memory allocation was successful
    { 
        perror("Out of memory! Aborting...");  // Print an error message
        exit(10);  // Terminate the program with exit code 10
    } /* fi */
    // Initialize the ILIST structure
    l->Length = 0;  // Initialize the list length to 0
    l->First = NULL; // Set the First pointer to NULL (empty list)
    l->Last = NULL;  // Set the Last pointer to NULL (empty list)
    return l; // Return the pointer to the newly created list
} /* end of CreateImageList */

/* Delete an image list (and all entries) */
void DeleteImageList(ILIST *list)
{
	assert(list);
    IENTRY *e = list->First;
    IENTRY *n;

    while (e) {
        n = e->Next;
        if (e->image) {
                DeleteImage(e->image);
        }
        free(e);
        e=n;
    }
    free(list);
}

/* Insert a RGB image to the image list at the end */
void AppendRGBImage(ILIST *list, Image *RGBimage)
{
	{
		assert(list);
		assert(RGBimage);
	
		IENTRY *entry = (IENTRY *)malloc(sizeof(IENTRY));
        if (!entry) {
            perror("Out of memory!");
            exit(10);
        }
	
		entry->image = (void *)RGBimage;
		entry->List = list;
		entry->Next = NULL;
		entry->Prev = list->Last;
	
		if (list->Last) {
			list->Last->Next = entry;
		} else {
			list->First = entry;
		}
	
		list->Last = entry;
		list->Length++;
	}
}

/* Insert a YUV image to the image list at the end */
void AppendYUVImage(ILIST *list, YUVImage *YUVimage)
{
	assert(list);
    assert(YUVimage);

    IENTRY *entry = (IENTRY *)malloc(sizeof(IENTRY));
    if (!entry) {
    perror("Out of memory!");
    exit(10);
    }
    entry->image = YUVimage;
	if (list->Last) {
    entry->List = list;
    entry->Next = NULL;
    entry->Prev = list->Last;
	list->Last->Next = entry;  // Update the current last entry to point to the new entry
    list->Last = entry;        // Update the last pointer to the new entry
	} 
	else {
		entry->List = list;  // Set the list reference in the entry
        entry->Next = NULL; 
        entry->Prev = NULL; 
        list->First = entry;  // Set both first and last pointers to the new entry
        list->Last = entry;
    }
    list->Length++;
}

/* Crop an image list */
void CropImageList(ILIST *list, unsigned int start, unsigned int end)
{
	assert(list);
    assert(start <= end);
    assert(end < list->Length);

    IENTRY *curr = list->First;
    unsigned int index = 0;

    while (curr) {
        IENTRY *next = curr->Next;

        if (index < start || index > end) {
            if (curr->image) {
                DeleteImage((Image *)curr->image);  // Or DeleteYUVImage()
            }

            if (curr->Prev) {
                curr->Prev->Next = curr->Next;
            } else {
                list->First = curr->Next;
            }

            if (curr->Next) {
                curr->Next->Prev = curr->Prev;
            } else {
                list->Last = curr->Prev;
            }

            free(curr);
            list->Length--;
        }

        curr = next;
        index++;
    }
}

/* Fast forward an image list */
void FastImageList(ILIST *list, unsigned int factor)
{
	assert(list);
    assert(factor > 0);

    IENTRY *curr = list->First;
    unsigned int index = 0;

    while (curr) {
        IENTRY *next = curr->Next;

        if (index % factor != 0) {
            if (curr->image) {
                DeleteImage((Image *)curr->image); 
            }

            if (curr->Prev) {
                curr->Prev->Next = curr->Next;
            } else {
                list->First = curr->Next;
            }

            if (curr->Next) {
                curr->Next->Prev = curr->Prev;
            } else {
                list->Last = curr->Prev;
            }

            free(curr);
            list->Length--;
        }

        curr = next;
        index++;
    }
}

/* Reverse an image list */
void ReverseImageList(ILIST *list)
{
	assert(list);

    IENTRY *curr = list->First;
    IENTRY *temp = NULL;

    while (curr) {
        temp = curr->Next;
        curr->Next = curr->Prev;
        curr->Prev = temp;
        curr = temp;
    }

    temp = list->First;
    list->First = list->Last;
    list->Last = temp;

}

/* Insert a RGB image to the image list in the front */
void PrependRGBImage(ILIST *list, Image *RGBimage)
{
	assert(list);
    assert(RGBimage);

    IENTRY *entry = (IENTRY *)malloc(sizeof(IENTRY));
        if (!entry) {
            perror("Out of memory!");
            exit(10);
        }
    entry->image = RGBimage;
	if (list->First) {
    entry->List = list;
    entry->Next = list->First;
    entry->Prev = NULL;
	list->First->Prev = entry;  // Update the current last entry to point to the new entry
    list->First = entry;        // Update the last pointer to the new entry
	} 
	else {
		entry->List = list;  // Set the list reference in the entry
        entry->Next = NULL; 
        entry->Prev = NULL; 
        list->First = entry;  // Set both first and last pointers to the new entry
        list->Last = entry;
    }
    list->Length++;
}


/* EOF */
