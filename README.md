# PPM-Filtering

### Summary. 
This program performs simple noise reduction on images using MPI and C.

### PPM. 
The Portable Pixmap Format (PPM) uses ASCII encoding of pixels for image files; for
details, see https://en.wikipedia.org/wiki/Netpbm_format#PPM_example. This program only works with P3 encoding.
### Mean filter. 
A simple filter for noise reduction in image processing is to replace a pixel by
the average of the neighbouring pixels in a “sliding” window. For example, with a 3 × 3 window
and the numbers on the left,
45 4 255
78 124 56
1 0 34
⇒
× × ×
× 66 ×
× × ×
we obtain the center of the window on the right as (rounded to the nearest integer)
(45 + 4 + 255 + 78 + 124 + 56 + 1 + 0 + 34)/9 = 66.
Such a window goes through each entry and replaces it by the mean of the entries in the window,
where the entry in the middle is included in computing the average.
### Median filter. 
Another filter is to replace a pixel by the median of the pixels in the window.
For details, see https://en.wikipedia.org/wiki/Median_filter.
## RGB. 
In an RGB encoding, we take the average (or median) over each of the colors red,
green, and blue independently. That is, the average of all red, all green, and all blue pixels in the
window.

### Work Distribution
The MPI program performs the following tasks:
* Process 0 reads a given input PPM file and distributes pixel data to p processes.
* Each process applies an N × N filter to the pixels distributed to this process.
* When finished, each process sends the result to process 0, which stores the filtered image in
a file in PPM format.

### Make and run

The project includes makefile such that when make is typed, an executable with name ppmf is
created in the current directory.

The program is trun as
mpirun -np p ./ppmf input.ppm output.ppm N F
where
* p is the number of processes
* input.ppm is the name of the input file
* output.ppm is the name of the output file
* N specifies the size of the window, that is N × N window, where N is an odd integer ≥ 3.
* F is the type of filtering and can have a value A meaning mean filter, or a value M meaning
median filter.
For example,
mpirun -np 4 ./ppmf input.ppm output.ppm 3 A
would run on 4 processes and apply a 3 × 3 mean filter.

The program can be tested with t.ppm.
