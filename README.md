# MovieLab/PhotoLab

A C-coded image and video processing toolkit made for my course, EECS 22 (Advanced C Programming) that performs advanced digital image processing on still images and YUV movies using dynamic memory, linked lists, and command-line interfaces.

## How It's Made:

The project is a digital image-and-video processing unit built in two stages:

PhotoLab: console-based image editor for PPM images that uses a custom Image struct with dynamically allocated 1D arrays to store RGB channels.
Supports a menu-driven interface to apply multiple Digital Image Processing (DIP) operations such as:
Black & white conversion

Negative filter

Color filtering

Edge detection

Vertical flip & horizontal mirror

Shuffling and pixelation

Image shifting

Cropping, resizing, watermarking, adding borders

90° rotation with width/height swap

 MovieLab: A command-line movie processor for raw YUV video streams that represents a movie as a doubly linked list of frames using custom ImageList and Movie structures. Can load and save .yuv movie files, converting between YUV and RGB color spaces using integer-based formulas and clipping.
Supports various video operations controlled via command-line flags:

-bw – black & white filter on all frames

-hmirror – horizontal mirror

-edge – edge detection

-crop start-end – crop frame range

-fast factor – fast-forward by taking every nth frame

-reverse – reverse frame order

-watermark file – overlay a watermark at pseudo-random positions

-spotlight – fade-in/fade-out circular spotlight on first and last 20 frames

-rotate – rotate & zoom the first/last 20 frames

## Optimizations

Instead of allocating a new Image for each filter, I updated the code so each operation works directly on the existing image buffer through a pointer. This eliminates unnecessary malloc/free calls, reduces memory fragmentation, and improves cache locality by keeping operations on the same continuous pixel array.

## Lessons Learned:

Improved proficiency in image-processing pipelines:
Gained experience breaking down filters into modular functions and understanding how pixel-level operations propagate across an image.

Learned to work with raw memory buffers in C:
Implemented operations directly on byte arrays, reinforcing concepts like pointers, stack vs. heap memory, and avoiding unnecessary allocations.

Developed debugging strategies using visual output:
Used the rendered SDL window as a live debugging tool, allowing real-time verification of filter behavior and faster iteration.

Better understanding of performance optimization:
Identified inefficiencies (e.g., repeated malloc/free), implemented in-place modifications, and saw first-hand how cache locality and memory reuse improve performance.

Strengthened ability to structure larger C programs:
Organized the project into headers, separate .c modules, and reusable helper functions to maintain clarity as the project grew.

Gained confidence writing and testing custom algorithms:
Validated filter correctness through step-by-step pixel inspection, printf debugging, and comparing results to reference outputs.
