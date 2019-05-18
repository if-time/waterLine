//#include <iostream>
//#include <opencv2/opencv.hpp>
//
//using namespace std;
//using namespace cv;
//
//int main() {
//    Mat image = imread("/home/dong/01.jpg");
//    namedWindow("Display frame", WINDOW_NORMAL);
//    cv::resizeWindow("Display frame", image.cols, image.rows);
//    imshow("Display frame", image);
//    waitKey(0);
//    return 0;
//}

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


void stretch(Mat &src, Mat &dst, int nMin, int nMax)
{
    int low_value = nMin;    //拉伸后像素的最小值
    int high_value = nMax;   //拉伸后像素的最大值

    float rate = 0;          //图像的拉伸率

    float stretch_p[256], stretch_p1[256], stretch_num[256];
    //清空三个数组,初始化填充数组元素为0
    memset(stretch_p, 0, sizeof(stretch_p));
    memset(stretch_p1, 0, sizeof(stretch_p1));
    memset(stretch_num, 0, sizeof(stretch_num));
    //统计图像各个灰度级出现的次数
    uchar* srcData = src.ptr<uchar>(0);
    uchar* dstData = dst.ptr<uchar>(0);
    //相当于c语言版本opencv的：
    //uchar* srcData = (uchar*)src->imageData;
    //uchar* dstData = (uchar*)dst->imageData;
    int nHeight = src.rows;
    int nWidth = src.cols;
    int i, j;
    uchar nVal = 0;
    for (i = 0; i<nHeight; i++)
    {
        for (j = 0; j<nWidth; j++)
        {
            nVal = srcData[i*nWidth + j];
            stretch_num[nVal]++;
        }
    }
    //统计各个灰度级出现的概率
    for (i = 0; i<256; i++)
    {
        stretch_p[i] = stretch_num[i] / (nHeight*nWidth);
    }
    //统计各个灰度级的概率和
    for (i = 0; i<256; i++)
    {
        for (j = 0; j <= i; j++)
        {
            stretch_p1[i] += stretch_p[j];
        }
    }
    //计算两个阈值点的值
    for (i = 0; i<256; i++)
    {
        if (stretch_p1[i]<0.1)     //low_value取值接近于10%的总像素的灰度值
        {
            low_value = i;
        }
        if (stretch_p1[i]>0.9)     //high_value取值接近于90%的总像素的灰度值
        {
            high_value = i;
            break;
        }
    }
    rate = (float)255 / (high_value - low_value + 1);
    //进行灰度拉伸
    for (i = 0; i<nHeight; i++)
    {
        for (j = 0; j<nWidth; j++)
        {
            nVal = srcData[i*nWidth + j];
            if (nVal<low_value)
            {
                dstData[i*nWidth + j] = 0;
            }
            else if (nVal>high_value)
            {
                dstData[i*nWidth + j] = 255;
            }
            else
            {
                dstData[i*nWidth + j] = (uchar)((nVal - low_value)*rate + 0.5);
                if (dstData[i*nWidth + j]>255)
                {
                    dstData[i*nWidth + j] = 255;
                }
            }
        }
    }
}


int main(int argc, char* argv[])
{
    Mat img = imread("/home/dong/waterLine.png", 0);
    if (img.empty())
    {
        cout << "Error: Could not load image" << endl;
        return 0;
    }
    imshow("img", img);

    Mat dst = img.clone();

    //计算图像的最大最小值 最大值赋0，最小值赋255
    int pixMax = 0, pixMin = 255;
    uchar * pdata = NULL;
    for (size_t r = 0; r < img.rows; r++)
    {
        pdata = img.ptr<uchar>(r);
        for (size_t c = 0; c < img.cols; c++)
        {
            if (pdata[c] > pixMax)
                pixMax = pdata[c];
            if (pdata[c] < pixMin)
                pixMin = pdata[c];
        }
    }

    stretch(img, dst, pixMin, pixMax);
    imshow("dst", dst);
    waitKey();


    return 0;
}