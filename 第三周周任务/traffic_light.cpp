#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

void processFrame(Mat const &frame)
{
    Mat red, yellow, green, hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    Scalar red1lower(0, 100, 100);
    Scalar red1upper(10, 255, 255);
    Scalar red2lower(160, 100, 100);
    Scalar red2upper(180, 255, 255);
    Scalar greenlower(35, 50, 50);
    Scalar greenupper(95, 255, 255);
    Scalar yellowlower(22, 150, 150);
    Scalar yellowupper(28, 255, 255);
    //生成不同颜色对应的二值图
    Mat maskR, maskr1, maskr2, maskG, maskY;
    inRange(hsv, red1lower, red1upper, maskr1);
    inRange(hsv, red2lower, red2upper, maskr2);
    maskR = maskr1 | maskr2;
    inRange(hsv, greenlower, greenupper, maskG);
    inRange(hsv, yellowlower, yellowupper, maskY);
    //开运算去噪
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(30, 30));
    morphologyEx(maskR, maskR, MORPH_OPEN, kernel);
    morphologyEx(maskG, maskG, MORPH_OPEN, kernel);
    morphologyEx(maskY, maskY, MORPH_OPEN, kernel);
    //查找轮廓
    vector<vector<Point>> contourR, contourG, contourY;
    findContours(maskR, contourR, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(maskG, contourG, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(maskY, contourY, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    //画框
    //红色
    for (size_t i = 0; i < contourR.size(); i++)
    {
        double area = contourArea(contourR[i]);
        if (area > 30000)
        {
            Rect box = boundingRect(contourR[i]);
            rectangle(frame, box, Scalar(0, 0, 255), 2);
            putText(frame, "RED", Point(700, 150), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 3);
        }
    }
    //绿色
    for (size_t i = 0; i < contourG.size(); i++)
    {
        double area = contourArea(contourG[i]);
        if (area > 30000)
        {
            Rect box = boundingRect(contourG[i]);
            rectangle(frame, box, Scalar(0, 255, 0), 2);
            putText(frame, "GREEN", Point(700, 150), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 3);
        }
    }
    imshow("红绿灯检测", frame);
}

int main()
{
    VideoCapture trafficlight;
    trafficlight.open("../TrafficLight.mp4");

    if (!trafficlight.isOpened())
    {
        cout << "视频打开失败" << endl;
        return -1;
    }

    int row = trafficlight.get(CAP_PROP_FRAME_WIDTH);
    int col = trafficlight.get(CAP_PROP_FRAME_HEIGHT);
    double fps = trafficlight.get(CAP_PROP_FPS);

    VideoWriter result;
    result.open("../result.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(row, col));

    Mat frame;
    while (true)
    {
        trafficlight >> frame;
        if (frame.empty())
        {
            break;
        }
        processFrame(frame);

        result.write(frame);

        if (waitKey(30) == 27)
        {
            break;
        }
    }

    return 0;
}