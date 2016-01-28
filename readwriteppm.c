#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "a1.h"

void writePPM(char* file, int width, int height, int max, const RGB *image)
{
  int i;

  // open file for writing
  FILE *fd;
  fd = fopen(file, "w");
  
  // output the header
  fprintf(fd, "P3\n");  
  fprintf(fd, "%d %d\n%d\n", width, height, max);
  
  // write the image
  for(i = 0; i < height*width; i++)
    {
      const RGB *p = image+i;
      fprintf(fd, "%d %d %d ", p->r, p->g, p->b);
    }
}


RGB * readPPM(char* file, int* width, int* height, int* max)
{
  /* Read a PPM P3 image from a file into a buffer.  Return the
     buffer, width, height, and the max value in the image. */

  FILE *fd;
  char c;
  int i,n;
  char b[100];
  int red, green, blue;
  
  // check if P3 ppm format
  fd = fopen(file, "r");
  n = fscanf(fd,"%[^\n] ",b);
  if(b[0]!='P'|| b[1] != '3')
    {
      printf("%s is not a PPM file!\n",file); 
      exit(0);
    }
  printf("%s is a PPM file\n", file);
  n = fscanf(fd, "%c",&c);
  while(c == '#') 
    {
      n = fscanf(fd, "%[^\n] ", b);
      printf("%s\n",b);
      n = fscanf(fd, "%c",&c);
    }
  ungetc(c,fd); 
  n = fscanf(fd, "%d %d %d", width, height, max);
  assert(n==3);

  printf("%d x %d image, max value= %d\n", *width, *height, *max);

  // size of image
  int size = *width*(*height);
  
  RGB *image = (RGB*)malloc(size*sizeof(RGB));
  assert(image);

  for(i=0; i < size; i++) 
    {
      n =  fscanf(fd,"%d %d %d",&red, &green, &blue );
      assert(n==3);
      image[i].r = red;
      image[i].g = green;
      image[i].b = blue;
    }
  
  return image;
}

