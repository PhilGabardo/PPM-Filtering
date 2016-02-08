#include <stdlib.h>
#include <stdio.h>
#include "a1.h"
#include <string.h>

void quickSort(unsigned char *a, int l, int r);
unsigned char partition(unsigned char *a, int l, int r);
unsigned char mean(unsigned char *a, int size);




RGB *processImage(int width, int height, RGB *image, int windowSize, char* filterType)
{
	int i, j, k, l;
	if (windowSize == 1){
		return image;
	}
	
	// Process pixel by pixel
	RGB *filtered = (RGB*)malloc(height*width*sizeof(RGB));
	for (i=0; i < height; i++){
		for (j=0; j < width; j++){
			unsigned char *windowR = (unsigned char*) malloc(sizeof(unsigned char)*windowSize*windowSize);
			unsigned char *windowB = (unsigned char*) malloc(sizeof(unsigned char)*windowSize*windowSize);
			unsigned char *windowG = (unsigned char*) malloc(sizeof(unsigned char)*windowSize*windowSize);
			
			int start = 0;
			int left = i - windowSize / 2;
			int right = i + windowSize / 2;
			int up = j - windowSize / 2;
			int down = j + windowSize / 2;
			for (k = left; k < right; k++){
				for (l = up; l < down; l++){
					if (k >= 0 && l >= 0 && k < height && l < width){
						windowR[start] = image[k*width + l].r;
						windowG[start] = image[k*width + l].g;
						windowB[start] = image[k*width + l].b;
						start++;
					}
				}
			}

			unsigned char filteredElementR;
			unsigned char filteredElementG;
			unsigned char filteredElementB;

			if (strcmp(filterType, "A") == 0){
				filteredElementR = mean(windowR, start);
				filteredElementG = mean(windowG, start);
				filteredElementB = mean(windowB, start);
			}
			if (strcmp(filterType, "M") == 0){
				quickSort(windowR, 0, start);
				quickSort(windowG, 0, start);
				quickSort(windowB, 0, start);
				filteredElementR = windowR[start/2];
				filteredElementG = windowR[start/2];
				filteredElementB = windowR[start/2];
			}

			filtered[i*width + j].r = filteredElementR;
			filtered[i*width + j].g = filteredElementG;
			filtered[i*width + j].b = filteredElementB;
		}
	}

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			image[i*width + j] = filtered[i*width + j];
		}
	}



	return image;

}


void quickSort(unsigned char *a, int l, int r)
{
	int j;

	if( l < r )
	{
		j = partition( a, l, r);
		quickSort( a, l, j-1);
		quickSort( a, j+1, r);
	}

}



unsigned char partition(unsigned char *a, int l, int r) {
	unsigned char pivot, t;
	int i, j;
	pivot = a[l];
	i = l; j = r+1;

	while( 1)
	{
		do ++i; while( a[i] <= pivot && i <= r );
		do --j; while( a[j] > pivot );
		if( i >= j ) break;
		t = a[i]; a[i] = a[j]; a[j] = t;
	}
	t = a[l]; a[l] = a[j]; a[j] = t;
	return j;
}


unsigned char mean(unsigned char *a, int size){
	int sum = 0;
	for (int i = 0; i < size; i++){
		sum += a[i];
	}
	sum /= size;
	unsigned char mean = (unsigned char) sum;
	return mean;
}
