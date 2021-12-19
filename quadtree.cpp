#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
    const int PING_PONG_LIMIT = 10;
    
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    // Find out rank, size
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // parallel rank para ver que nodo tiene mejores recursos
    // hacer aqui el parallel rank

    // Divsion de la imagen
    const int x = 801;
    const int y = 601;
    int cx = 800/2;
    int cy = 600/2;
    int count = 0;
    
    int k = 2;

    for (size_t i = 0; i < k; i++)
    {
        for (size_t j = 0; j < k; j++)
        {
            if (count % world_size == world_rank)
            {
                int y0 = i*cy;
                int y1 = (i+1)*cy;
                int x0 = j*cx;
                int x1 = (j+1)*cx;
                if(i == k-1)
                    y1 = y;
                if(j == k-1)    
                    x1 = x;
                ofstream file;
                file.open("compressimage" + to_string(count) + ".txt");
                file << to_string(x0)  + " " + to_string(x1) + "\n";
                file << to_string(y0)  + " " + to_string(y1) + "\n";
                
                file.close();
                
            }
            ++count;
        }
    }
    
    MPI_Finalize();

}
