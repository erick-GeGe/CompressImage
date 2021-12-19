EXECS=compress_image
MPICC?=mpiCC

all: ${EXECS}

compress_image: quadtree.cpp
	${MPICC} -o quadtree quadtree.cpp

clean:
	rm -f ${EXECS}


