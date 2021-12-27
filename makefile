EXECS = compress_image join_image
MPICC = mpiCC
GCC = g++

LIBS = `pkg-config --cflags --libs opencv` 

all: $(EXECS)

compress_image: main.cpp
	${MPICC} main.cpp -o compress_image $(LIBS)

join_image: join_image.cpp
	${GCC} join_image.cpp -o join_image $(LIBS)


clean:
	rm -f ${EXECS}


