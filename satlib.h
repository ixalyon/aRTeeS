#ifndef LIB
#define LIB
#include <CommonInclude.h>

void remove_contours(vector<vector<Point> > &contours, double cmin, double cmax)
{
    auto it = partition(begin(contours), end(contours), [=](vector<Point> const &data)
    {
        auto const size = contourArea(data);
        return !(size < cmin || size > cmax);
    });
    contours.erase(it, end(contours));
}


void get_rect_points(Rect const &rect, Point points[])
{
    points[0].x = rect.x;
    points[0].y = rect.y;
    points[1].x = rect.x + rect.width - 1;
    points[1].y = rect.y;
    points[2].x = points[1].x;
    points[2].y = rect.y + rect.height - 1;
    points[3].x = rect.x;
    points[3].y = points[2].y;
}

void sort_rect_corners(RotatedRect &rct)
{

    float angle = rct.angle;
    Size rect_size = rct.size;
    Point2f center = rct.center;

    if (rct.angle < -45.) {
        angle += 90.0;
        swap(rect_size.width, rect_size.height);
    }
    rct = RotatedRect(center,rect_size,angle);
//    rct.points(rectPoints);


}
void sort_rect_corners( RotatedRect &rct,Point2f rectPoints[])
{
    std::vector<Point2f> left, right;
    Point2f const center = rct.center;
    for (auto i = 0; i < 4; i++){
        if (rectPoints[i].x < center.x)
            left.emplace_back(rectPoints[i]);
        else
            right.emplace_back(rectPoints[i]);
    }

    if(rct.angle<-45.){
        rectPoints[0] = left[0].y  > left[1].y  ? left[1]  : left[0];
        rectPoints[1] = left[0].y  > left[1].y  ? left[0]  : left[1];
        rectPoints[2] = right[0].y > right[1].y ? right[1] : right[0];
        rectPoints[3] = right[0].y > right[1].y ? right[0] : right[1];
    }else{
        rectPoints[1] = left[0].y  > left[1].y  ? left[1]  : left[0];
        rectPoints[2] = left[0].y  > left[1].y  ? left[0]  : left[1];
        rectPoints[3] = right[0].y > right[1].y ? right[1] : right[0];
        rectPoints[0] = right[0].y > right[1].y ? right[0] : right[1];
    }
}

bool containsCheck(Rect &rc,int centerX, int begin, int end){
    bool contains=false;
    for (int i = begin; i <= end; i+=30){
        contains=contains||rc.contains(Point(centerX,i));
//        cout<<contains;
    }
//    waitKey();
    return contains;
}
bool containsCheck(Rect &rc, Point3i p){
    return containsCheck(rc,p.x,p.y,p.z);
}

bool matIsEqual(const Mat mat1, const Mat mat2){
    // treat two empty mat as identical as well

    if (mat1.empty() && mat2.empty()) {
        return true;
    }

    // if dimensionality of two mat is not identical, these two mat is not identical
    if (mat1.cols != mat2.cols || mat1.rows != mat2.rows || mat1.dims != mat2.dims) {
        return false;
    }

    Mat diff;
    compare(mat1, mat2, diff, CMP_NE);
    int nz = cv::countNonZero(diff);
    return nz==0;
}

void Hough(Mat input,Mat &output,bool P){
    if(P){
        vector<Vec2f> lines;
        HoughLines(input, lines, 1, CV_PI/360,35, 0, 0 );

    //          for( auto i = 0; i < lines.size()-1; i++ )
    //              if(lines[i][1] >CV_PI/3/*||theta>CV_PI*2/3*/)

//        cout<<endl;

        for( size_t i = 0; i < lines.size(); i++ )
        {
           float rho = lines[i][0], theta = lines[i][1];
           if(!(theta >CV_PI/18&&theta<CV_PI*17/18)
                    &&abs(lines[i][0])<100&&abs(lines[i][0])>10&&i!=lines.size()-1)
               cout<<" Distances : "<<abs(lines[i][0])<<endl;
           else
               continue;

           Point pt1, pt2;
           double a = cos(theta), b = sin(theta);
           double x0 = a*rho, y0 = b*rho;
           pt1.x = cvRound(x0 + 1000*(-b));
           pt1.y = cvRound(y0 + 1000*(a));
           pt2.x = cvRound(x0 - 1000*(-b));
           pt2.y = cvRound(y0 - 1000*(a));
           line( output, pt1, pt2, Scalar(255), 1, CV_AA);
        }
   }else{
        vector<Vec4i> lines;
        HoughLinesP(input, lines, 1, CV_PI/360, 20,  50, 10 );
        for( size_t i = 0; i < lines.size(); i++ )
        {
          Vec4i l = lines[i];
          line( output, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255), 1, CV_AA);
        }
    }
    imshow("sdsdcv",output);
    waitKey();
}

#endif // LIB

