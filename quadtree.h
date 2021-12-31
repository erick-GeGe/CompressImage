#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

class Rectangle
{
public:
    // Rango del rectangulo
    int a, b, c, d;
    cv::Vec3b average;
    

    Rectangle()
    {
        a = b = c = d = 0;
    }

    Rectangle(int _a, int _b, int _c, int _d)
    {
        a = _a;
        b = _b;
        c = _c;
        d = _d;
    }

    // Se calcula el color promedio de la imagen
    void calculateAverage(cv::Mat img)
    {
        int count = 0;
        unsigned int cr, cg, cb;
        cr = cg = cb = 0;
        // Sumando todos los colores del rectangulo
        for (int i = a; i <= b; i++)
        {
            for (int j = c; j <= d; j++)
            {
                ++count;
                cv::Vec3b color = img.at<cv::Vec3b>(cv::Point(i, j));
                cr += color.val[0];
                cg += color.val[1];
                cb += color.val[2];
            }
        }
        // Obteniendo promedio
        average.val[0] = cr / count;
        average.val[1] = cg / count;
        average.val[2] = cb / count;
    }

    // Se le pasa un color para asignarlo a todo un rectangulo
    void changeColor(cv::Vec3b color, cv::Mat img, Rectangle *boundary_b)
    {
        for (int i = a; i < b; i++)
            for (int j = c; j < d; j++)
                img.at<cv::Vec3b>(cv::Point(i - boundary_b->a, j - boundary_b->c)) = color;
    }
};

class QuadTree
{
    int threshold;
    cv::Mat img;
    cv::Mat new_img;
    Rectangle *boundary;
    QuadTree *childrens[4];
    Rectangle *childrensb[4];

    // Funcion que se encarga de comprimir la imagen
    void compress(Rectangle *boundary_b)
    {
        int c1, c2;
        // Se obtiene las mitades de la imagen
        c1 = (boundary->b - boundary->a) / 2;
        c2 = (boundary->d - boundary->c) / 2;
        subdivide(c1, c2);

        if (partible() && c1 && c2)
        {
            // Ya que debe partirse, se crean los 4 nuevos hijos
            for (size_t i = 0; i < 4; i++)
            {
                childrens[i] = new QuadTree(childrensb[i], img, threshold, new_img);
                childrens[i]->compress(boundary_b);
            }
        }
        else
        {
            // Ya que no ser parte se asigna el color promedio de la imagen al rectangulo
            for (size_t i = 0; i < 4; i++)
                childrensb[i]->changeColor(childrensb[i]->average, new_img, boundary_b);
        }
    }

    // Se verifica si el rectangulo debe partirse
    bool partible()
    {
        // Se obtiene el promedio de colores
        int averages[3]{0};
        for (size_t i = 0; i < 4; i++)
        {
            childrensb[i]->calculateAverage(img);
            for (size_t j = 0; j < 3; j++)
                averages[j] += (childrensb[i]->average.val[j]) / 4;
        }

        // Se calcula la diferencia entre el los colores promedio y el promedio de promedios
        for (size_t i = 0; i < 3; i++)
        {
            int dif = 0;
            for (size_t j = 0; j < 4; j++)
                dif += abs(childrensb[j]->average.val[i] - averages[i]);
            
            // Se verifica que no supere el threshold
            if (dif > threshold)
                return true;
        }
        return false;
    }

    // El rectangulo se divide en 4 rectangulos mas pequeños
    void subdivide(int c1, int c2)
    {
        childrensb[0] = new Rectangle(boundary->a, boundary->a + c1, boundary->c, boundary->c + c2);
        childrensb[1] = new Rectangle(boundary->a + c1, boundary->b, boundary->c, boundary->c + c2);
        childrensb[2] = new Rectangle(boundary->a, boundary->a + c1, boundary->c + c2, boundary->d);
        childrensb[3] = new Rectangle(boundary->a + c1, boundary->b, boundary->c + c2, boundary->d);
    }

public:
    QuadTree(Rectangle *b, cv::Mat image, int t, cv::Mat n_image)
    {
        new_img = n_image;
        img = image;
        threshold = t;
        boundary = b;
        for (size_t i = 0; i < 4; i++)
        {
            childrens[i] = NULL;
            childrensb[i] = NULL;
        }
    }

    ~QuadTree()
    {
        if (boundary)
            delete boundary;
    }
    
    // Funcion publica para comprimir imagen
    void compress_image()
    {

        compress(boundary);
    }
};
