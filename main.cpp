#include "quadtree.h"
#include <mpi.h>


using namespace std;
using namespace cv;  

int main(int argc, char** argv){
    
    int n=stoi(argv[2]);

    Mat image = imread(argv[1], cv::IMREAD_COLOR);
    if (!image.data)                                                                          
    {  
        cout << "No image data \n";  
        return -1;  
    } 

    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    const int x = image.cols;
    const int y = image.rows;
    int cx = x/2;
    int cy = y/2;
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

                cout << world_rank << "\t" << x0 << " " << x1 << " " << y0 << " " << y1 << endl;
                Rectangle *boundary = new Rectangle(x0, x1, y0, y1);
                QuadTree compressor(boundary, image, n);
                Mat new_image = compressor.compress_image();
                
                imwrite("comprimido" + to_string(count) + ".jpg", new_image);
                
            }
            ++count;
        }
    }
    
    MPI_Finalize();

    
    return 0;
}