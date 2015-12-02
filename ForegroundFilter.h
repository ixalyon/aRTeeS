#ifndef FOREGROUNDFILTER_H
#define FOREGROUNDFILTER_H

#include <CommonInclude.h>



class ForegroundFilter
{

public:
    static void trackbarCallback(int, void* param);
    void filterText(Mat image, Mat *outputMask);
    void filterPen (Mat image, Mat *outputMask);
    void valSet(Point3i minRange, Point3i maxRange, int cvtType, string windowName="");
    void filter(Mat src, Mat* mask);
private:
    int minHue, maxHue, minSat, maxSat, minVal, maxVal,cvtType;
    void addTracker(string windowName);
    void filter();
    Mat src;
    Mat* mask;
    string windowName="";
};

void ForegroundFilter::filter(Mat src,Mat* mask)
{
    this->src=src;
    this->mask=mask;


    Mat dst,out,put,imbackup;
    cvtColor(src,dst,cvtType);
    //              blur(dst,dst,Size(3,3));
    medianBlur(src,dst,5);
    inRange(dst, Scalar(minHue,minSat,minVal), Scalar(maxHue,maxSat,maxVal),out);
    src.copyTo(put,out);
    //              morphologyEx(put,put,MORPH_CLOSE,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    Mat tepl(src.size(),src.type());
    //              tepl=Scalar(255,255,255);
    tepl=Scalar(217,184,180);
    bitwise_not(out,out);
    morphologyEx(out,out,MORPH_OPEN,getStructuringElement(MORPH_RECT,Size(5,5)),Point(1,1));
    morphologyEx(out,out,MORPH_DILATE,getStructuringElement(MORPH_CROSS,Size(3,3)),Point(1,1));
    morphologyEx(out,out,MORPH_ERODE,getStructuringElement(MORPH_CROSS,Size(3,3)),Point(1,1),2);
    //              morphologyEx(out,out,MORPH_CLOSE,getStructuringElement(MORPH_ELLIPSE,Size(2,5)),Point(1,1),5);
//                  bitwise_not(out,out);
    //              morphologyEx(out,out,MORPH_CLOSE,getStructuringElement(MORPH_ELLIPSE,Size(3,3)),Point(1,1),2);
    src.copyTo(tepl,out);

    imbackup=tepl.clone();
//    imwrite("filteredmask2.png",out);
//    imwrite("filteredorigin2.png",tepl);
    *mask=tepl.clone();

    src.copyTo(imbackup,*mask);
//    imshow("bla",imbackup);
//    namedWindow(windowName);
//    addTracker( "Image");
//    imshowWithTracker( "Image1", tepl );

}
void ForegroundFilter::filterText(Mat image,Mat* outputMask)
{
    valSet(Point3i(133,69,80),Point3i(255,255,255),CV_BGR2HSV,"FilterWindow");
//    filter(image,outputMask);
}
void ForegroundFilter::filterPen(Mat image, Mat* outputMask)
{
    valSet(Point3i(0,0,100),Point3i(109,255,255),CV_RGB2HSV,"FilterWindow");
//    filter(image,outputMask);
}
void ForegroundFilter::addTracker(string windowName)
{
        namedWindow(windowName);
        createTrackbar( "TopH", windowName, &minHue, 255,trackbarCallback,this);
        createTrackbar( "BotH", windowName, &maxHue, 255,trackbarCallback,this);
        createTrackbar( "TopS", windowName, &minSat, 255,trackbarCallback,this);
        createTrackbar( "BotS", windowName, &maxSat, 255,trackbarCallback,this);
        createTrackbar( "TopV", windowName, &minVal, 255,trackbarCallback,this);
        createTrackbar( "BotV", windowName, &maxVal, 255,trackbarCallback,this);

}
void ForegroundFilter::valSet(Point3i minRange, Point3i maxRange, int cvtType,string windowName)
{
    minHue=minRange.x;
    maxHue=maxRange.x;
    minSat=minRange.y;
    maxSat=maxRange.y;
    minVal=minRange.z;
    maxVal=maxRange.z;
    this->cvtType=cvtType;

    this->windowName=windowName;
    if(windowName!="")
        addTracker(windowName);
}

void ForegroundFilter::trackbarCallback(int,void *param)
{
    ForegroundFilter *self = static_cast<ForegroundFilter*>(param);
    self->filter();
}

void ForegroundFilter::filter()
{
    filter(src,mask);
    imshow(windowName,*mask);
}
#endif // FOREGROUNDFILTER_H
