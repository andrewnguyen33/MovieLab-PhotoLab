#######################################################################
# Homework Assignment 5, for EECS 22, Winter 2024
#
# Initial author: Tim Schmidt
#
# Makefile: Makefile for HW 5
#
#######################################################################

# Variables
CC = gcc                    
CFLAGS = -Wall -ansi -std=c99 -lm  
OBJS = MovieLab.o FileIO.o DIPs.o Image.o ImageList.o Movie.o  
HDRS = Constants.h FileIO.h DIPs.h Image.h ImageList.h Movie.h
TARGET=MovieLab
# Default target
all: $(TARGET)
######################### Generate object files #######################
# Target for MovieLab.o
MovieLab.o: MovieLab.c $(HDRS)
	$(CC) $(CFLAGS) -c MovieLab.c -o MovieLab.o
# Target for FileIO.o
FileIO.o: FileIO.c $(HDRS)
	$(CC) $(CFLAGS) -c FileIO.c -o FileIO.o
# Target for DIPs.o
DIPs.o: DIPs.c $(HDRS)
	$(CC) $(CFLAGS) -c DIPs.c -o DIPs.o
# Target for Image.o
Image.o: Image.c $(HDRS)
	$(CC) $(CFLAGS) -c Image.c -o Image.o
# Target for ImageList.o
ImageList.o: ImageList.c $(HDRS)
	$(CC) $(CFLAGS) -c ImageList.c -o ImageList.o
# Target for Movie.o
Movie.o: Movie.c $(HDRS)
	$(CC) $(CFLAGS) -c Movie.c -o Movie.o
######################### Generate the executable #####################
# Target for MovieLab
MovieLab: $(OBJS)
	$(CC) $(CFLAGS) -o MovieLab  MovieLab.c Movie.c DIPs.c FileIO.c Image.c ImageList.c -lm
###############################  others  ##############################
# Target for clean-up
clean:
	rm -f *.o out.yuv $(TARGET)