CFLAGS=-g -O2 -Wall

CC = mpicc

PROGRAM_NAME= ppmf
OBJS = main.o readwriteppm.o processimage.o 

$(PROGRAM_NAME): $(OBJS)
	$(CC) -o $@ $? 

clean:
	rm  *.o $(PROGRAM_NAME) *~
