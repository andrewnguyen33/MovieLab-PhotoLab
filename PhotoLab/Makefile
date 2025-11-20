all: libfilter.a PhotoLab PhotoLabTest

PhotoLab: PhotoLab.o FileIO.o DIPs.o Advanced.o Image.o Test.o
	gcc -Wall -std=c11 -g PhotoLab.o FileIO.o DIPs.o Advanced.o Image.o Test.o -o PhotoLab -L. -lfilter -lm

PhotoLabTest: PhotoLabTest.o FileIO.o DIPs.o Advanced.o Image.o Test.o
	gcc -Wall -std=c11 -g -DDEBUG PhotoLabTest.o FileIO.o DIPs.o Advanced.o Image.o Test.o -o PhotoLabTest -L. -lfilter -lm

libfilter.a: DIPs.o Advanced.o Image.o Test.o
	ar rc libfilter.a DIPs.o Advanced.o Image.o Test.o
	ranlib libfilter.a

PhotoLab.o: PhotoLab.c FileIO.h Constants.h DIPs.h Advanced.h Image.h Test.h
	gcc -Wall -std=c11 -g -c PhotoLab.c -o PhotoLab.o

PhotoLabTest.o: PhotoLab.c FileIO.h Constants.h DIPs.h Advanced.h Image.h Test.h
	gcc -Wall -std=c11 -g -DDEBUG -c PhotoLab.c -o PhotoLabTest.o

FileIO.o: FileIO.c FileIO.h Constants.h
	gcc -Wall -std=c11 -g -c FileIO.c -o FileIO.o

DIPs.o: DIPs.c DIPs.h Constants.h
	gcc -Wall -std=c11 -g -c DIPs.c -o DIPs.o

Advanced.o: Advanced.c Advanced.h Constants.h
	gcc -Wall -std=c11 -g -c Advanced.c -o Advanced.o

Image.o: Image.c Image.h Constants.h
	gcc -Wall -std=c11 -g -c Image.c -o Image.o

Test.o: Test.c Test.h Constants.h
	gcc -Wall -std=c11 -g -c Test.c -o Test.o

clean:
	rm -f *.o *.a PhotoLab PhotoLabTest