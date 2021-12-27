PROJECT = compress_image
MPICC = mpiCC
SRC = main.cpp

LIBS = `pkg-config --cflags --libs opencv`


$(PROJECT): $(SRC)
	${MPICC} $(SRC) -o $(PROJECT) $(LIBS)

clean:
	rm ${PROJECT}


