#ifndef PATTERNEXTRACTION_H
#define PATTERNEXTRACTION_H

#include <CommonInclude.h>
#include <satlib.h>
#include <Prepro.h>
#include <TemplateClassifier.h>


class DataExtraction
{
public:
    void Image2Contour(Mat input,vector<vector<Point>> &contours);
    Mat CenterPattern(Mat input, vector<vector<Point>> &contours, Point3i p);
    void PatternSegment(Mat input, vector<vector<Point>> &contours, Point3i p);
    int isCenterPattern=-1;
    void PatternsSelection(Mat input, vector<vector<Point>> &contours, Mat &region);
private:
    Mat CurrentPattern;
    Rect skewCorrection(Mat input,vector<Point> contour,Mat &output);
    void RegionSelection(Mat src, vector<RotatedRect> selectedPattern, Mat &region);
};
void DataExtraction::Image2Contour(Mat input, vector<vector<Point> > &contours)
{
    Mat image = input.clone();
    Prepro::cvtBGR2Gray(image);
//    Prepro::blurX(image,3);
//    imshow("ssa",image);
    medianBlur(image,image,3);
//    GaussianBlur(image,image,Size(5,5),0,0);
//    imshow("ssa",image);
    Canny(image,image,70,140,3);
//    Hough(image,input,false);
//    imshow("sssa",image);

    findContours(image,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE,Point(0,0));
    remove_contours(contours,1500,12000);
    int j=0;
    for(auto contour:contours)
        cout<<"Contour : "<<++j<<" Area : "<<contourArea(contour)<<endl;

}

Rect DataExtraction::skewCorrection(Mat input, vector<Point> contour, Mat &output)
{
    RotatedRect rect = minAreaRect( Mat(contour));
    Mat rotated,M;
    Point2f vertices[4];
    rect.points(vertices);
    float angle = rect.angle;
    Size rect_size = rect.size;

    if (rect.angle < -45.) {
        angle += 90.0;
        swap(rect_size.width, rect_size.height);
    }

    M = getRotationMatrix2D(rect.center, angle, 1.0);

    warpAffine(input, rotated, M, input.size(), INTER_CUBIC);

    getRectSubPix(rotated, rect_size, rect.center, output);

    auto rec=rect.center;
    int heightRect=rect_size.height;
    int widthRect=rect_size.width;
    Rect rc((int)rec.x-widthRect/2,(int)rec.y-heightRect/2,widthRect,heightRect);
    return rc ;
}

Mat DataExtraction::CenterPattern(Mat input, vector<vector<Point>> &contours, Point3i p)
{
    isCenterPattern=-1;
    Mat output,null;
    for(uint i=0;i<contours.size();++i){
        Rect rc = skewCorrection(input,contours[i],output);

        if(containsCheck(rc,p)){
            isCenterPattern=i;
            return output;
        }
    }
    return null;
}

void DataExtraction::PatternSegment(Mat input,vector<vector<Point>> &contours, Point3i p)
{
    Mat roi = CenterPattern(input,contours,p);
    if(roi.empty())
        return;
    resize(roi,roi,Size(84,30));
    Mat roix=roi.clone();
    cvtColor(roi,roi,CV_BGR2GRAY);
//      adaptiveThreshold(roi,roi,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,3,0);
    threshold(roi,roi,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);
      cv::Mat structuringElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
      cv::morphologyEx( roi, roi, cv::MORPH_OPEN, structuringElement ,Point(1,1));
      cv::morphologyEx( roi, roi, cv::MORPH_CLOSE, structuringElement ,Point(1,1));
      cv::morphologyEx( roi, roi, cv::MORPH_OPEN, structuringElement ,Point(1,1));
//      blur(roi,roi,Size(3,3));

      std::vector<std::vector<cv::Point>> contoursR;
      cv::findContours(roi, contoursR, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
//        cv::drawContours(roi, contours, -1, cv::Scalar(128), CV_FILLED);
      int j=0;
      TemplateClassifier tc;
    for(auto contour:contoursR){
        if(contourArea(contour)>50){
            Rect boundary = boundingRect(contour);
            Mat roin = roix(boundary);
            imshow("Aropped1",roin);

                cout<<"Number"<<tc.categorizer(roin)<<endl;
//              imwrite(name.str(),roin);
              imshow("Cropped",roix);
//              waitKey();
        }
    }


}

void DataExtraction::PatternsSelection(Mat input, vector<vector<Point>> &contours,Mat &region)
{
    if(isCenterPattern==-1||contours.size()<2)
        return;
    Mat output,null;
    vector<Rect> patternBoundaryList;
    vector<RotatedRect> selectedPattern;
    Rect rc;
    for(uint i=0;i<contours.size();++i){
        rc=skewCorrection(input,contours[i],output);
        if(!(containsCheck(rc,roiW/2,0,300)))
            continue;
        patternBoundaryList.push_back(skewCorrection(input,contours[i],output));
        selectedPattern.push_back(minAreaRect(contours[i]));
        Point points[4];
        get_rect_points(patternBoundaryList[i],points);
//        line(input, {points[0].x, points[0].y}, {points[1].x, points[1].y}, Scalar(0,0,255), 2);
//        line(input, {points[1].x, points[1].y}, {points[2].x, points[2].y}, Scalar(0,0,255), 2);
//        line(input, {points[2].x, points[2].y}, {points[3].x, points[3].y}, Scalar(0,0,255), 2);
//        line(input, {points[3].x, points[3].y}, {points[0].x, points[0].y}, Scalar(0,0,255), 2);
//        for (int j = 0; j < 4; j++)
//            line(output, vertices[j], vertices[(j+1)%4], Scalar(0,255,0),2);
        imshow( "Source", input );
        RegionSelection(input,selectedPattern,region);
    }

}

void DataExtraction::RegionSelection(Mat src,vector<RotatedRect> selectedPattern,Mat &region)
{
    if(selectedPattern.size()<2)
        return;
    {
//        cout<<selectedPattern<<endl;
    Point2f rctP[2][4];
    int i = 0 ;
    for (RotatedRect rct:selectedPattern) {
      if(i<2){
          vector<Point2f> pointVector;
          sort_rect_corners(rct);
          rct.points(rctP[i]);
          for(auto rectPoint:rctP[i])
              pointVector.push_back(rectPoint);

          int j=0;
          std::ostringstream name;
          for(Point2f point:pointVector){
              name<<++j;
              if(rct.angle<-45.)
                  name<<"-";
    //                putText( src,name.str(), Point(int(point.x+5.0),int(point.y)),  FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,0),2);
//              circle(src,Point(int(point.x),int(point.y)),3,Scalar(255,0,0),2);
              name.str("");
          }
//          cout<<" Pattern Number : "<<i<<" Center : "<<rct.center<<endl;

      }
      i++;

    }
    line(src,rctP[0][0],rctP[1][1],Scalar(255,255,0),2);
    line(src,rctP[0][0],rctP[0][3],Scalar(255,255,0),2);
    line(src,rctP[1][2],rctP[1][1],Scalar(255,255,0),2);
    line(src,rctP[1][2],rctP[0][3],Scalar(255,255,0),2);
    vector<Point2f> oPoints
    {
        rctP[0][0],rctP[0][3],rctP[1][2],rctP[1][1]
    };
    Mat target(108,96,src.type());
    std::vector<cv::Point2f> target_points
    {
     {0, 0}, {target.cols - 1, 0},
     {target.cols - 1, target.rows - 1},
     {0, target.rows - 1}
    };
    Mat const trans_mat = cv::getPerspectiveTransform(oPoints, target_points);
    warpPerspective(src, target, trans_mat, target.size());

//    imwrite("regionSelect.png",src);
//    imwrite("region.png",target);
    imshow("Region",target);
//    waitKey();
    region=target;
    }
//    contours.clear();
//    src_out.release();
}

/*
 * void test(){
Mat canny_output;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
bool flag=true;


Canny( src_gray, canny_output, thresh, thresh*2, 3 );

findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1, Point(0, 0) );
//    imshow("tests",canny_output);

OCV::remove_contours(contours,1500,12000);
  if(contours.size()==0)
      return;
RotatedRect rect;

Mat M, rotated, roi,correctedImage;
Mat mask=Mat::zeros( canny_output.size(), CV_8UC1);
vector<Point> approx;
vector<RotatedRect> selectedPattern;
vector<Point2f> scale;
for( auto i = 0; i<contours.size(); i++ )
{
//      cout<<rect_size.height<<" - "<<rect_size.width<<" - "<<i;
    rect = minAreaRect( Mat(contours[i]));
    Point2f vertices[4];
    rect.points(vertices);
    float angle = rect.angle;
    Size rect_size = rect.size;

    if (rect.angle < -45.) {
        angle += 90.0;
        swap(rect_size.width, rect_size.height);
    }

    M = getRotationMatrix2D(rect.center, angle, 1.0);

    warpAffine(canny_output, canny_output, M, src.size(), INTER_CUBIC);
    warpAffine(src, rotated, M, src.size(), INTER_CUBIC);

//          imshow("Srcs",src);
    auto rec=rect.center;
    int heightRect=rect_size.height;
    int widthRect=rect_size.width;
    Rect rc((int)rec.x-widthRect/2,(int)rec.y-heightRect/2,widthRect,heightRect);

    cout<<"Contour : "<<rect.center<<" Area : "<<contourArea(contours[i])<<" Corners : "<<rect.size<<endl;
    getRectSubPix(rotated, rect_size, rect.center, roi);
    imshow("sss",roi);
    waitKey();
    if(containsCheck(rc,10,270)){
////          cout<<"hit"<<endl;
//          Point points[4];
//          get_rect_points(rc,points);
//          line(rotated, {points[0].x, points[0].y}, {points[1].x, points[1].y}, Scalar(0,0,255), 2);
//          line(rotated, {points[1].x, points[1].y}, {points[2].x, points[2].y}, Scalar(0,0,255), 2);
//          line(rotated, {points[2].x, points[2].y}, {points[3].x, points[3].y}, Scalar(0,0,255), 2);
//          line(rotated, {points[3].x, points[3].y}, {points[0].x, points[0].y}, Scalar(0,0,255), 2);
        for (int j = 0; j < 4; j++)
            line(rotated, vertices[j], vertices[(j+1)%4], Scalar(0,255,0),2);
        imshow( "Source", rotated );
        imwrite(to_string(i)+"patternSelect.png",rotated);
        if(flag){
            flag=false;
            correctedImage=rotated.clone();
        }
        scale.push_back(Point2f(widthRect/27.0,heightRect/10.0));
        selectedPattern.push_back(rect);
//              cout<<" x - "<<rc.x<<" y- "<<rc.y<<endl;
    }

}
if(selectedPattern.size()<2)
    return;
{
Point2f rctP[2][4];
int i = 0 ;
for (RotatedRect rct:selectedPattern) {
  if(i<2){
      vector<Point2f> pointVector;
      sort_rect_corners(rct,rctP[i]);
      rct.points(rctP[i]);
      for(auto rectPoint:rctP[i])
          pointVector.push_back(rectPoint);

      int j=0;
      std::ostringstream name;
      for(Point2f point:pointVector){
          name<<++j;
          if(rct.angle<-45.)
              name<<"-";
//                putText( src,name.str(), Point(int(point.x+5.0),int(point.y)),  FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,0),2);
          circle(src,Point(int(point.x),int(point.y)),3,Scalar(255,0,0),2);
          name.str("");
      }
      cout<<" Pattern Number : "<<i<<" Center : "<<rct.center<<endl;

  }
  i++;

}
line(src,rctP[0][0],rctP[1][1],Scalar(255,255,0),2);
line(src,rctP[0][0],rctP[0][3],Scalar(255,255,0),2);
line(src,rctP[1][2],rctP[1][1],Scalar(255,255,0),2);
line(src,rctP[1][2],rctP[0][3],Scalar(255,255,0),2);
vector<Point2f> oPoints
{
    rctP[0][0],rctP[0][3],rctP[1][2],rctP[1][1]
};
Mat target(108,96,src.type());
std::vector<cv::Point2f> target_points
{
 {0, 0}, {target.cols - 1, 0},
 {target.cols - 1, target.rows - 1},
 {0, target.rows - 1}
};
Mat const trans_mat = cv::getPerspectiveTransform(oPoints, target_points);
warpPerspective(src, target, trans_mat, target.size());

imwrite("regionSelect.png",src);
imwrite("region.png",target);
imshow("Region",src);
waitKey();
}
contours.clear();
src_out.release();
}
*/
#endif // PATTERNEXTRACTION_H
