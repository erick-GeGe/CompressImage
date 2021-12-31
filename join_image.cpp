#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    // Obteniendo dimension y nombre de la imagen a comprimir
    string cadena;
    int rows, cols;
    ifstream archivo("compressed_image/resolution.txt");
    getline(archivo, cadena);
    rows = stoi(cadena);
    cout << rows << endl;
    getline(archivo, cadena);
    cols = stoi(cadena);
    cout << cols << endl;
    getline(archivo, cadena);
    archivo.close();

    // Creando la nueva imagen con las dimensiones leidas
    int k = 4;
    cv::Mat new_image = cv::Mat::zeros(rows, cols, CV_8UC3);

    int count_i, count_j, count;
    count = count_i = count_j = 0;

    int cx = rows / k;
    int cy = cols / k;

  
    // Recorriendo las 16 imagenes pequeñas
    for (size_t i = 0; i < k; i++)
    {
        for (size_t j = 0; j < k; j++)
        {
            int x0 = i * cx;
            int y0 = j * cy;
            
            // Leyendo cada imagen
            string imagen_src = "compressed_image/comprimido" + to_string(count) + ".jpg";
            Mat image_n = imread(imagen_src, cv::IMREAD_COLOR);
            for (int i_aux = 0; i_aux < image_n.rows; i_aux++)
            {
                for (int j_aux = 0; j_aux < image_n.cols; j_aux++)
                {
                    // Juntando imagen
                    cv::Vec3b color = image_n.at<cv::Vec3b>(cv::Point(j_aux, i_aux));
                    new_image.at<cv::Vec3b>(cv::Point(j_aux + y0, i_aux + x0)) = color;
                }
            }
            count++;
        }
    }

    // Escribiendo imagen final
    imwrite("test_images/comprimido_" + cadena + ".jpg", new_image);

    return 0;
}
