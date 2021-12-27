#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

class Rectangle
{
public:
    int a, b, c, d;
    cv::Vec3b average;
    bool isleaf;

    Rectangle()
    {
        a = b = c = d = 0;
        isleaf = false;
    }

    Rectangle(int _a, int _b, int _c, int _d)
    {
        isleaf = false;
        a = _a;
        b = _b;
        c = _c;
        d = _d;
    }

    void calculateAverage(cv::Mat img)
    {
        int count = 0;
        unsigned int cr, cg, cb;
        cr = cg = cb = 0;
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
        average.val[0] = cr / count;
        average.val[1] = cg / count;
        average.val[2] = cb / count;
    }

    void changeColor(cv::Vec3b color, cv::Mat img, Rectangle *boundary_b)
    {
        for (int i = a; i < b; i++)
        {
            for (int j = c; j < d; j++)
            {
                // if( j - boundary_b->c > img.rows)
                //     continue;
                // if( i - boundary_b->a > img.cols)
                //     continue;
                // std::cout << img.cols << " " << img.rows << std::endl;
                // std::cout << c << " " << d << std::endl;
                // std::cout << i - boundary_b->a << " " << c << std::endl;
                img.at<cv::Vec3b>(cv::Point(i - boundary_b->a, j - boundary_b->c)) = color;
                // img.at<cv::Vec3b>(cv::Point(i, j)) = color;
            }
        }
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
    std::string filecompress;

    void compress(Rectangle *boundary_b)
    {
        int c1, c2;
        c1 = (boundary->b - boundary->a) / 2;
        c2 = (boundary->d - boundary->c) / 2;
        subdivide(c1, c2);
        // std::cout << "alo?\n";

        if (partible() && c1 && c2)
        {
            for (size_t i = 0; i < 4; i++)
            {
                childrens[i] = new QuadTree(childrensb[i], img, threshold, new_img);
                childrens[i]->compress(boundary_b);
            }
        }
        else
        {
            // cv::Vec3b averages;
            // for(int i = 0; i < 4; i++)
            //     for (size_t j = 0; j < 3; j++)
            //         averages.val[j] += (childrensb[i]->average.val[j])/4;

            // boundary->average = averages;
            // boundary->changeColor(boundary->average, img);
            // std::cout << "fue aqui\n";

            for (size_t i = 0; i < 4; i++)
            {
                childrensb[i]->changeColor(childrensb[i]->average, new_img, boundary_b);
            }
            // std::cout << "fue aqui2\n";
            // boundary->isleaf = true;
        }
    }

    bool partible()
    {

        int averages[3]{0};
        for (size_t i = 0; i < 4; i++)
        {
            childrensb[i]->calculateAverage(img);
            for (size_t j = 0; j < 3; j++)
                averages[j] += (childrensb[i]->average.val[j]) / 4;
        }

        for (size_t i = 0; i < 3; i++)
        {
            int dif = 0;
            for (size_t j = 0; j < 4; j++)
                dif += abs(childrensb[j]->average.val[i] - averages[i]);

            if (dif > threshold)
                return true;
        }
        return false;
    }

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
        filecompress = "";
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

    void compress_image()
    {

        compress(boundary);
    }
};
