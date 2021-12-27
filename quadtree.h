#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

class Rectangle
{
public:
    int a, b, c ,d;
    cv::Vec3b average;
    bool isleaf;

    Rectangle(){
        a = b = c = d = 0;
        isleaf = false;
    }
    
    Rectangle(int _a, int _b, int _c, int _d){
        isleaf = false;
        a = _a;
        b = _b;
        c = _c;
        d = _d;
    }
    
    void calculateAverage(cv::Mat img){
        int count = 0;
        unsigned int cr,cg,cb;
        cr = cg = cb = 0;
        for(int i = a; i <= b; i++){
            for(int j = c; j <= d; j++){
                ++count;
                cv::Vec3b color = img.at<cv::Vec3b>(cv::Point(i,j));
                cr+= color.val[0];
                cg+= color.val[1];
                cb+= color.val[2];
            }
        }
        average.val[0] = cr/count;
        average.val[1] = cg/count;
        average.val[2] = cb/count;
    }

    void changeColor(cv::Vec3b color, cv::Mat img, Rectangle *boundary_b){
        for(int i = a; i<b; i++){
           for(int j = c; j<d; j++){
                img.at<cv::Vec3b>(cv::Point(i - boundary_b->a,j - boundary_b->c)) = color;
            }
        }
    }
};

class QuadTree
{
	int threshold;
    cv::Mat img;
    Rectangle *boundary;
	QuadTree *childrens[4];
    Rectangle *childrensb[4];
    std::string filecompress;

    void compress(cv::Mat n_image, Rectangle *boundary_b ){
        int c1, c2;
        c1 = (boundary->b - boundary->a) / 2;
        c2 = (boundary->d - boundary->c) / 2;
        subdivide(c1, c2);
        
        if(partible() && c1 && c2){
            for (size_t i = 0; i < 4; i++)
            {
                childrens[i] = new QuadTree(childrensb[i], img, threshold);
                childrens[i]->compress(n_image, boundary_b);
            }
        }
        else{
            // cv::Vec3b averages;
            // for(int i = 0; i < 4; i++)
            //     for (size_t j = 0; j < 3; j++)
            //         averages.val[j] += (childrensb[i]->average.val[j])/4;
            
            // boundary->average = averages;
            // boundary->changeColor(boundary->average, img);
            for (size_t i = 0; i < 4; i++)
            {
                childrensb[i]->changeColor(childrensb[i]->average, n_image, boundary_b);
            }
            
            // boundary->isleaf = true;
        }
    }

    bool partible(){
        
        int averages[3] {0};
        for (size_t i = 0; i < 4; i++)
        {
            childrensb[i]->calculateAverage(img);
            for (size_t j = 0; j < 3; j++)
                averages[j] += (childrensb[i]->average.val[j])/4;
        }
        
        for (size_t i = 0; i < 3; i++)
        {
            int dif = 0;
            for (size_t j = 0; j < 4; j++)
                dif+= abs(childrensb[j]->average.val[i] - averages[i]);

            if(dif > threshold)
                return true;
        }
        return false;
    }

    void subdivide(int c1, int c2){
        childrensb[0] = new Rectangle(boundary->a, boundary->a + c1, boundary->c, boundary->c + c2);
        childrensb[1] = new Rectangle(boundary->a + c1, boundary->b, boundary->c, boundary->c + c2);
        childrensb[2] = new Rectangle(boundary->a, boundary->a + c1, boundary->c + c2, boundary->d);
        childrensb[3] = new Rectangle(boundary->a + c1, boundary->b, boundary->c + c2, boundary->d);
    }

public:
	
    QuadTree(Rectangle *b, cv::Mat image, int t)
	{
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
	
    ~QuadTree(){
        if(boundary)
            delete boundary;
	}
	
    cv::Mat compress_image(){
        int w = boundary->b - boundary->a;
        int h = boundary->d - boundary->c;
        cv::Mat new_image = cv::Mat::zeros(h, w, CV_8UC3);
        compress(new_image, boundary);
        return new_image;
    }

    std::string saveCompress(){
        if(boundary->isleaf){
            std::string output = std::to_string(boundary->a) + ' ' +std::to_string(boundary->b) + ' ' + std::to_string(boundary->c) + ' ' + std::to_string(boundary->d) + '\n';
            output += std::to_string(boundary->average.val[0]) + ' ' + std::to_string(boundary->average.val[1]) + ' ' + std::to_string(boundary->average.val[2]) + '\n';
            return output;
        }
        else{
            for (size_t i = 0; i < 4; i++)
            {
                if(childrens[i]){
                    filecompress += childrens[i]->saveCompress();
                }
            }
        }
        return filecompress;
    }
};
