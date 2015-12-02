#ifndef TEMPLATECLASSIFIER
#define TEMPLATECLASSIFIER
#include <CommonInclude.h>
#include <satlib.h>

class TemplateClassifier{
public:
    TemplateClassifier();
    int categorizer(Mat input);
private:
    void preprocess(Mat &roin);
    Mat M[16];
};

void TemplateClassifier::preprocess(Mat &roin){
    cvtColor(roin,roin,CV_BGR2GRAY);
    threshold(roin,roin,20,255,THRESH_BINARY|THRESH_OTSU);
    distanceTransform(roin,roin,CV_DIST_L2,3);
    normalize(roin, roin, 0.0, 1.0, NORM_MINMAX);
    threshold(roin,roin,0.25,1,THRESH_BINARY);
//    imshow("ss2",roin);
    morphologyEx(roin,roin,MORPH_DILATE,getStructuringElement(MORPH_RECT,Size(3,3)),Point(-1,-1),3);
    morphologyEx(roin,roin,MORPH_ERODE,getStructuringElement(MORPH_RECT,Size(3,3)),Point(-1,-1),1);
//    imshow("ss1",roin);

    std::vector<cv::Point> whiteList;
    whiteList.reserve(roin.rows*roin.cols);
    for(int j=0; j<roin.rows; ++j)
        for(int i=0; i<roin.cols; ++i)
        {
            // if not black: add to the list
            if(roin.at<int>(j,i) != 0)
            {
                whiteList.push_back(cv::Point(i,j));
            }
        }
    Rect boundary = boundingRect(whiteList);
    roin=roin(boundary);
    resize(roin,roin,Size(3,3),0,0,INTER_LANCZOS4);
    threshold(roin,roin,0.9,1,THRESH_BINARY);

    Mat ronx;
    resize(roin,ronx,Size(18,18),0,0,INTER_NEAREST);
//    imshow("ss",ronx);

}

int TemplateClassifier::categorizer(Mat input)
{
    Mat copy=input.clone();

    preprocess(copy);
    for (int i = 0; i < 16; ++i){
        if(matIsEqual(copy,M[i]))
            return i;
    }

    return -1;
}
TemplateClassifier::TemplateClassifier(){
    FileStorage fs1("numberSystem.yml", FileStorage::READ);
    Mat N;
    for (int i = 0; i < 16; ++i) {
        fs1["N"+to_string(i)]>>N;
        M[i]=N.clone();
    }
    fs1.release();
}

#endif // TEMPLATECLASSIFIER

