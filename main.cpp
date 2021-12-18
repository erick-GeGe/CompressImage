#include "quadtree.h"
#include <fstream>

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
    namedWindow("Image", WINDOW_NORMAL);
    Rectangle *boundary = new Rectangle(0, image.cols, 0, image.rows);
    QuadTree compressor(boundary, image, n);
    compressor.compress_image();
    ofstream outfile;
    outfile.open("comprimido.sip");
    outfile << compressor.saveCompress();
    outfile.close();
    imwrite("comprimido.jpg", image);
    
    return 0;
}