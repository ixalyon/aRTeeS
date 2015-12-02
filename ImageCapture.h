#ifndef IMAGECAPTURE_H
#define IMAGECAPTURE_H

#include "CommonInclude.h"

class ImageCapture
{

public:
    ImageCapture(int source);
    ImageCapture(string source);
    ~ImageCapture();
    Mat capture();
    void capture(Mat&);
    const Mat blank=Mat(480,720,CV_8UC3,Scalar(0,0,0));
    bool getSrcAval();

private:
    bool srcAval=false;
    VideoCapture cap;
    Mat temp;
};

Mat ImageCapture::capture()
{

    if(!srcAval){
        cout<<"Source Failed"<<endl;
        return blank.clone();
    }
    cap.retrieve(temp);
    return temp;
}
void ImageCapture::capture(Mat& output)
{

    if(!srcAval){
        cout<<"Source Failed"<<endl;
        output = blank;
    }
    cap.retrieve(output);
}
inline bool ImageCapture::getSrcAval(){ return srcAval=cap.grab(); }

inline ImageCapture::ImageCapture(string source){ srcAval=cap.open(source); }

inline ImageCapture::ImageCapture(int source){ srcAval=cap.open(source); }


ImageCapture::~ImageCapture()
{
    cap.release();
}

#endif // IMAGECAPTURE_H
