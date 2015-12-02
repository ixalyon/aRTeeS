#include <ImageCapture.h>
#include <Prepro.h>
#include <ForegroundFilter.h>
#include <DataExtraction.h>
#include <RegionProccessing.h>
//#include <TemplateClassifier.h>


int main()
{
    ImageCapture im(DataPath+"write3.mp4");
    Prepro pp;
    ForegroundFilter f;
    DataExtraction pe;
    RegionProcessing rp;
//    TemplateClassifier tc;


    Mat img,roi,mask,newImage,region;
//    f.valSet(Point3i(46,30,100),Point3i(161,255,255),CV_BGR2HSV,"tests");
    f.filterPen(roi,&mask);
    for(;im.getSrcAval();){

            im.capture(img);
            pp.roi(img,roi,roiW,roiH);
            f.filter(roi,&mask);
            imshow("d",mask);
//            bitwise_not(mask,mask);
//            Mat roiMasked;
//            roi.copyTo(roiMasked,mask);
//            roi=mask.clone();

            vector<vector<Point> > contours;
            pe.Image2Contour(mask,contours);

            pe.PatternSegment(mask,contours,Point3i(roiW/2-20,0,100));
            pe.PatternsSelection(roi,contours,region);
            if(region.empty())
                continue;
            rp.Output(region,region,2);
//            if(test.empty())
//                continue;
//        f.filter(roi,&mask);
//        roi.copyTo(newImage,mask);
//        imshow("Image",newImage);
//        imwrite("sample1roi.png",roi);
//        waitKey(15);
        imshow("test",region);
//        newImage.release();
        waitKey();
    }
    return(0);
}
