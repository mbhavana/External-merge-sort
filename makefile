#define the C compiler to use
CC = gcc

#define any compile-time flags
CFLAGS = -g

#
INCLUDES = -I/seperate

#define the C source files
#SRCS1 = quicksort.c heapsort.c heapify.c ext_merge_sort.c main.c

# define the C object files 
#
#OBJS1 = $(SRCS1:.c=.o)


#SRCS2 = randomizedquicksort.c

#
#OBJS2 = $(SRCS2:.c=.o)

# define the executable file 
MAIN1 = program1

MAIN2 = program2

.PHONY: depend clean

all: $(MAIN1) $(MAIN2)
	@echo Compiler named: program1 has been compiled
	@echo Compiler named: program2 has been compiled


$(MAIN1): quicksort.c heapsort.c heapify.c ext_merge_sort.c main.c 
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(MAIN2): randomizedquicksort.c 
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ 




#.c.o:
#	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@


#clean:
#	$(RM) *.o *~ $(MAIN1)

#depend: $(SRCS1)
#	makedepend $(INCLUDES) $^

#depend: $(SRCS2)
#	makedepend $(INCLUDES) $^