# include <iostream>
# include <opencv2/opencv.hpp>
#include <string>
#include<chrono>
using namespace std;
using namespace cv;
int lowThreshold = 0;   
int highThreshold = 0; 
Mat src, gray, edges;
//主函数
int main()
{
    Mat kernel =getStructuringElement(MORPH_RECT,Size(5,5),Point(-1,-1));
    VideoCapture video;
    video.open("../video.mp4");
    if(!video.isOpened()){
        return -1;
    }
    Mat frame,imgHsv,mask;
    Rect Roi;
    Scalar scalarL =Scalar(86,43,46);
    Scalar scalarH=Scalar(93,255,255);
    while(video.read(frame)){
        resize(frame,frame,Size(1000,500));
        cvtColor(frame,imgHsv,COLOR_BGR2HSV);
        inRange(imgHsv,scalarL,scalarH,mask);
        morphologyEx(mask,mask,MORPH_OPEN,kernel);
        dilate(mask,mask,kernel);
        auto start = chrono::high_resolution_clock::now();
        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        if(contours.size()>0)
        {
            double max=0.0;
            for(size_t i=0;i<contours.size();i++)
            {
                double area =contourArea(contours[static_cast<int>(i)]);
                if(area>max)
                {
                    Roi=boundingRect(contours[static_cast<int>(i)]);
                    for(size_t i=0;i<contours.size();i++)
                    {
                    Point2f P[4];
                    RotatedRect rect = minAreaRect(contours[static_cast<int>(i)]);
                    rect.points(P);
                    rectangle(frame,Roi,Scalar(0,0,255));
                    auto end =chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = end -start;
                    string time = to_string(elapsed.count());
                    putText(frame,"aim:"+time,P[2],FONT_HERSHEY_SIMPLEX,1,Scalar(0,0,255),1,1);
                    }
                }
            }
        }
            else
            {
                Roi.x=Roi.y=Roi.width=Roi.height=0;
            }
            //auto end =chrono::high_resolution_clock::now();
            //std::chrono::duration<double> elapsed = end -start;
            //string time = to_string(elapsed.count());
            //putText(frame,time,P[2],FONT_HERSHEY_SIMPLEX,1,Scalar(0,0,255),1,1);
        imshow("hsv",mask);
        imshow("video",frame);
        waitKey(50);
    } 
    return 0;
}
void on_change(int, void*)
{
    GaussianBlur(gray, gray, Size(3, 3), 0);
    Canny(gray, edges, lowThreshold, highThreshold);
    imshow("edges", edges);
}
