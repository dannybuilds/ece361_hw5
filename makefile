# Makefile for Homework #5
#
# Author: 		Danny V. Restrepo
# Instructor: 	Prof. Roy Kravitz
# School:		Portland State University
# Course:		ECE 361
# Term:			Fall 2024

CC = gcc
CFLAGS = -Wall -std=c99 -g

# Source files
SRCS = float_rndm.c iom361_r2.c temp_humid_bst.c hw5_app.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = hw5_app

# Default target
all: $(EXEC)

# Links object files to create executable
$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

# Compiles source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Cleans target to remove generated files
clean:
	rm -f $(OBJS) $(EXEC)

# Dependencies
float_rndm.o: float_rndm.c float_rndm.h
iom361_r2.o: iom361_r2.c iom361_r2.h
temp_humid_bst.o: temp_humid_bst.c temp_humid_bst.h
hw5_app.o: hw5_app.c tempHumidtree.h iom361_r2.h float_rndm.h
