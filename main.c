#include <stdlib.h>
#include "a1.h"
#include <mpi.h>
#include <stdio.h>
int main(int argc, char** argv)
{
	RGB *image, *filteredImage;
	int global_width, global_height;
	int width, height, max;
	int my_rank, p;
	int dest, source;
	int offset;
	MPI_Status status;
	Dimension *my_dim;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int tag = 0;
	int windowSize = atoi(argv[3]);
	
	if (my_rank == 0){
		image = readPPM(argv[1], &global_width, &global_height, &max);
		offset = 0;
		for (dest = 1; dest < p; dest++){
			int rowsToCompute = global_height/p;
			if (dest < global_height % p)
				rowsToCompute++;
			offset += rowsToCompute;
			if (dest == p-1){
				rowsToCompute += (windowSize/2);
			}
			else{
				rowsToCompute += (windowSize/2)*2;
			}
			Dimension *dim = (Dimension*) malloc(sizeof(Dimension));
			dim->width = global_width;
			dim->height = rowsToCompute;
			MPI_Send(dim, 2, MPI_INT, dest, tag, MPI_COMM_WORLD);
		}
		width = global_width;
		height = global_height / p;
		if (global_height % p != 0){
			height += 1;
		}
		height += (windowSize/2);
	}

	else {	
		my_dim = (Dimension*)malloc(sizeof(Dimension));
		MPI_Recv(my_dim, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		width = my_dim->width;
		height = my_dim->height;
		image = (RGB*)malloc(height*width*sizeof(RGB));
	}

	

	if (my_rank == 0){
		offset = global_height / p;
		if (global_height % p != 0){
			offset += 1;
		}
		offset += (windowSize/2);
		for (dest = 1; dest < p; dest++){
			int rowsToCompute = global_height/p;
			if (dest < global_height % p)
				rowsToCompute++;

			if (dest == p-1){
                                rowsToCompute += (windowSize/2);
                        }
                        else{
                                rowsToCompute += (windowSize/2)*2;
                        }

			if (dest == p-1){
				offset -= (windowSize/2)*2;
			}
			else{
				offset -= (windowSize/2)*2;
			}
		
			
			MPI_Send(image + offset*width, 3*rowsToCompute*width, MPI_UNSIGNED_CHAR, dest, tag, MPI_COMM_WORLD);
			offset += rowsToCompute;
		}		
	}
	else{

		MPI_Recv(image, height*width*3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD, &status);
	}
	
		
	filteredImage = processImage(width, height, image, windowSize, argv[4]);
	
	if (my_rank != 0 && my_rank != p-1){
		MPI_Send(image + (windowSize/2)*width, (height-2*(windowSize/2))*width*3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
	}
	else if (my_rank == p-1){
		MPI_Send(image + (windowSize/2)*width, (height-(windowSize/2))*width*3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
	}

	else{
		offset = global_height / p;
                if (global_height % p != 0){
                        offset += 1;
                }
		for (source = 1; source < p; source++){
			int rowsToCompute = global_height/p;
			if (source < global_height % p)
				rowsToCompute++;

			MPI_Recv(image+offset*width, rowsToCompute*width*3, MPI_UNSIGNED_CHAR, source, tag, MPI_COMM_WORLD, &status);
			
			offset += rowsToCompute;
		}

	}

	if (my_rank == 0){
		writePPM(argv[2], global_width, global_height, max, filteredImage);
		free(image);
	}

	MPI_Finalize();
	return(0);
}
