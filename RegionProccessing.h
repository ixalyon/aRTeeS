#ifndef REGIONPROCCESSING
#define REGIONPROCCESSING
#include <CommonInclude.h>

class RegionProcessing{
public:
    void Output(Mat input, Mat &output, int K);
    void OutputBinary(Mat input, Mat &output);
private:
    void sampleImage(Mat src, Mat &samples);
    void applyKMeans(Mat samples, int clusterCount, int attempts,Mat src, Mat &clustedImage);
};

void RegionProcessing::Output(Mat input, Mat &output, int K)
{
    if(input.empty())
        return;
    Mat samples(input.total(), 3, CV_32F),labels,centers;
    sampleImage(input,samples);
    applyKMeans(samples,K,5,input,output);
}

void RegionProcessing::OutputBinary(Mat input, Mat &output)
{
    Output(input,output,2);
    cv::Mat binary;
    cv::cvtColor(output, binary, CV_BGR2GRAY);
    cv::threshold(binary,binary,0.1,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
    output=binary;
}

void RegionProcessing::sampleImage(Mat src, Mat &samples)
{
    float* samples_ptr = samples.ptr<float>(0);
    for( int row = 0; row != src.rows; ++row){
        auto src_begin = src.ptr<uchar>(row);
        auto src_end = src_begin + src.cols * src.channels();
        //auto samples_ptr = samples.ptr<float>(row * src.cols);
        while(src_begin != src_end){
            samples_ptr[0] = src_begin[0];
            samples_ptr[1] = src_begin[1];
            samples_ptr[2] = src_begin[2];
            samples_ptr += 3; src_begin +=3;
        }
    }
}

void RegionProcessing::applyKMeans(Mat samples, int clusterCount, int attempts,Mat src, Mat &clustedImage)
{
    Mat labels,centers;
    kmeans(samples, clusterCount, labels,
           TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS,10, 0.01),
                attempts,KMEANS_PP_CENTERS, centers);

    Mat new_image(src.size(), src.type());
    for( int row = 0; row != src.rows; ++row){
        uchar* new_image_begin = new_image.ptr<uchar>(row);
        uchar* new_image_end = new_image_begin + new_image.cols * 3;
        int* labels_ptr = labels.ptr<int>(row * src.cols);

        while(new_image_begin != new_image_end){
            int const cluster_idx = *labels_ptr;
            float* centers_ptr = centers.ptr<float>(cluster_idx);
            new_image_begin[0] = centers_ptr[0];
            new_image_begin[1] = centers_ptr[1];
            new_image_begin[2] = centers_ptr[2];
            new_image_begin += 3; ++labels_ptr;
        }
    }
    clustedImage=new_image;
}

#endif // REGIONPROCCESSING

