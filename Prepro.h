#ifndef PREPROCESSOR
#define PREPROCESSOR
//Common Header Files and properties
#include <CommonInclude.h>

class Prepro{

public:
    static void roi(Mat &input,Mat &roi, int width, int height);
    static void roi(Mat &input, Mat &roi, int width, int height, int factor);
    static void cvtBGR2Gray(Mat color,Mat &gray);
    static void cvtBGR2Gray(Mat &image);
    static void blur3(Mat &image);
    static void blurX(Mat &image,int size);
    static int matWidth(Mat s);
    static int matHeight(Mat s);
    static Size int2Size(int x);

};

inline void Prepro::roi(Mat &input, Mat &roi, int width, int height)
{
    roi= input(Rect(input.size().width/2-width/2,0, width,height));
}
inline void Prepro::roi(Mat &input, Mat &roi, int width, int height,int factor)
{
    roi = input(Rect( factor+matWidth(input)/2-width/2,0, width,height));
}
inline void Prepro::cvtBGR2Gray(Mat color,Mat &gray){ cvtColor( color, gray, CV_BGR2GRAY);}

inline void Prepro::cvtBGR2Gray(Mat &image){ cvtColor( image, image, CV_BGR2GRAY); }

inline void Prepro::blur3(Mat &image){ blur(image,image,Size(3,3)); }

inline void Prepro::blurX(Mat &image,int size){ blur(image,image,Size(size,size)); }

inline int  Prepro::matWidth(Mat s){ return s.size().width; }

inline int  Prepro::matHeight(Mat s){ return s.size().height; }

inline Size Prepro::int2Size(int x){ return Size(x,x); }
#endif // PREPROCESSOR

