PROG = prog
current_dir = $(shell pwd)
all: $(PROG)

libmanager.so: libmanager.c
	gcc -shared -fPIC -o libmanager.so libmanager.c

$(PROG): libmanager.so
	gcc -o prog program.c -L=${current_dir} -lmanager -Wl,-rpath=${current_dir}

clean:
	rm -vf *.o ${PROG} *.so *.out blockdb *.csv
