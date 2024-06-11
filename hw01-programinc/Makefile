
# DEPS=make.h
# OBJ = TaoM.CS5600.LearnC.o mpg2km.o 

# &.o: %.c $(DEPS)
# 	$(CC) -c -o $@ $< $(CFLAGS)
# make: $(OBJ)
# 	$(CC) -o $@ $^ $(CFLAGS)

# .PHONY: clean

# clean:
# 	rm -f $(OBJ) make

# 为什么要加包装起ifndef
#ifndef HEADER_FILE
#define HEADER_FILE
# CC=gcc
# CFLAGS=-I.
# DEPS=make.h
# make: TaoM.CS5600.LearnC.o mpg2km.o 
# 	$(CC) -o make TaoM.CS5600.LearnC.o mpg2km.o
# clean:
	
#endif

learnc: TaoM.CS5600.LearnC.c mpg2km.c
	gcc TaoM.CS5600.LearnC.c mpg2km.c -o learnc

clean:
	rm -f learnc *.o