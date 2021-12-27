#include "quadtree.h"
#include <mpi.h>
#include <fstream>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{

    int n = stoi(argv[2]);

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

    int k = 4;
    const int x = image.cols;
    const int y = image.rows;
    int cx = x / k;
    int cy = y / k;
    int count = 0;

    for (size_t i = 0; i < k; i++)
    {
        for (size_t j = 0; j < k; j++)
        {
            if (count % world_size == world_rank)
            {
                int y0 = i * cy;
                int y1 = (i + 1) * cy;
                int x0 = j * cx;
                int x1 = (j + 1) * cx;
                if (i == k - 1)
                    y1 = y;
                if (j == k - 1)
                    x1 = x;

                char processor_name[MPI_MAX_PROCESSOR_NAME];
                int name_len;
                MPI_Get_processor_name(processor_name, &name_len);

                cout << world_rank << " " << processor_name << "\t" << x0 << " " << x1 << " " << y0 << " " << y1 << endl;
                int w = x1 - x0;
                int h = y1 - y0;
                cv::Mat new_image = cv::Mat::zeros(h, w, CV_8UC3);

                Rectangle *boundary = new Rectangle(x0, x1, y0, y1);
                QuadTree compressor(boundary, image, n, new_image);
                compressor.compress_image();

                imwrite("compressed_image/comprimido" + to_string(count) + ".jpg", new_image);
                if (world_rank == 0)
                {
                    ofstream file;
                    file.open("compressed_image/resolution.txt");
                    file << image.rows;
                    file << "\n";
                    
                    file << image.cols;
                    file << "\n";
                    string prueba = argv[1];
                    file << prueba.substr(12,6);
                    file << "\n";
                    file.close();
                }
            }
            ++count;
        }
    }
    MPI_Finalize();
    return 0;
}