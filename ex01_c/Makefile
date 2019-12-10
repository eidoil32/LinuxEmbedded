PROG 		= prog
current_dir = $(shell pwd) #find current directory
CC      	= gcc
LIBS		= -lpthread

all: $(PROG)

libmanager.so: libmanager.c
	$(CC) -shared -fPIC -o libmanager.so libmanager.c

SRCS := $(subst ./,,$(shell find . -name "*.c" ! -name "libmanager.c")) # Finding all files with .c

OBJS := $(patsubst %.c,%.out,$(SRCS))

$(PROG): libmanager.so $(OBJS)

%.out: %.c
	$(CC) $^ -o $@ $(LIBS) -L=$(current_dir) -lmanager -Wl,-rpath=$(current_dir)

clean:
	rm -vf *.o ${PROG} *.so *.out  *.csv


#add blockdb
