#ifndef RECONSTRUCTION
#define RECONSTRUCTION
#include <CommonInclude.h>
#include <Prepro.h>

class Reconstruction{
public:
    Reconstruction();
    void ApplyPosition(Mat region,vector<int> pos);
    Point pos2Point(vector<int> pos);
private:
    int posDecode(vector<int> pos);
    Mat PaperTemplate;
    Point CurrentPoint;
    int pWidth=27,pHeight=10,bWidth=3,bHeight=34,hCount=33,vCount=29,initX=3,initY=2;
};

Reconstruction::Reconstruction(){
    FileStorage fs1("PaperTemplate.yml", FileStorage::READ);
    fs1["B18"]>>PaperTemplate;
    fs1.release();
    CurrentPoint=Point(initX,initY+pHeight);
}
void Reconstruction::ApplyPosition(Mat region, vector<int> pos)
{
    Point current=pos2Point(pos);
         cout<<"Works"<<current<<endl;
    if(region.empty())
        return;
    if(current.x==-1||current.y==-1)
        current=CurrentPoint;
    Mat roi=PaperTemplate(Rect(current.x,current.y,pWidth,bHeight));
    resize(region,region,roi.size());
    imshow("test",region);
    Prepro::cvtBGR2Gray(region);
    threshold(region,region,0.3,1,THRESH_BINARY);
    bitwise_not(region,region);
//    region.copyTo(roi);
    roi=Scalar(0);
    Mat showTemplate;
    resize(PaperTemplate,showTemplate,Size(),0.5,0.5);
        imshow("Template",showTemplate);
}

Point Reconstruction::pos2Point(vector<int> pos)
{

    int position=posDecode(pos);
    if(position==-1||position>hCount*vCount)
        return Point(-1,-1);
    int y = position/hCount*(pHeight+bHeight)+initY;
    int x = position%hCount*(pWidth+bWidth)+initX;
    CurrentPoint=Point(x,y+pHeight);
    return CurrentPoint;
}

int Reconstruction::posDecode(vector<int> pos)
{
    if(pos.empty())
        return -1;

    sort(pos.begin(),pos.end());
    return pos[0]%100*256+pos[1]%100*16+pos[2]%100;
}

#endif // RECONSTRUCTION

