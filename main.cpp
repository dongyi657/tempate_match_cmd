#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include "QDebug"
#include <QString>
using namespace cv;
using namespace std;
#define MATCH_NUM 20
#define PRECISION_THRESHOLD_MAX 0.99
#define PRECISION_THRESHOLD_MIN 0.01
static Mat g_srcImage, g_tempalteImage, g_resultImage;
static int g_nMatchMethod;
//static QScreen *screen = QGuiApplication::primaryScreen();
//int g_nMaxTrackbarNum = 5;
//cv::Mat QImageToMat(QImage image)
//{
//    cv::Mat mat;
//    switch (image.format())
//    {
//    case QImage::Format_ARGB32:
//    case QImage::Format_RGB32:
//    case QImage::Format_ARGB32_Premultiplied:
//        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
//        break;
//    case QImage::Format_RGB888:
//        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
//        cv::cvtColor(mat, mat, CV_BGR2RGB);
//        break;
//    case QImage::Format_Indexed8:
//        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
//        break;
//    }
//    return mat;
//}

QString on_matching()
{
    Mat srcImage;
    QString result="";
    g_srcImage.copyTo(srcImage);
    int resultImage_cols = g_srcImage.cols - g_tempalteImage.cols + 1;
    int resultImage_rows = g_srcImage.rows - g_tempalteImage.rows + 1;
    g_resultImage.create(resultImage_cols, resultImage_rows, CV_32FC1);
    if (g_nMatchMethod%2==0)g_nMatchMethod=g_nMatchMethod+1;
    //g_resultImage It must be single-channel 32-bit floating-point
    matchTemplate(g_srcImage, g_tempalteImage, g_resultImage, g_nMatchMethod);

//    normalize(g_resultImage, g_resultImage, 0, 1, NORM_MINMAX, -1, Mat());
    double minValue, maxValue;
    Point minLocation, maxLocation, matchLocation[MATCH_NUM];
    int i=0, match_num;
    for (i=0; i<MATCH_NUM;i++){
        minMaxLoc(g_resultImage, &minValue, &maxValue, &minLocation, &maxLocation);
        //cout<< minValue <<minLocation <<maxValue <<maxLocation<<endl;
        if (g_nMatchMethod == TM_CCORR_NORMED || g_nMatchMethod == TM_CCOEFF_NORMED)
        {
            if (maxValue<PRECISION_THRESHOLD_MAX)break;
            matchLocation[i] = maxLocation;
            //cout<<QString::number(g_resultImage.ptr<float>(maxLocation.y)[maxLocation.x]).toStdString()<<endl;
            g_resultImage.ptr<float>(maxLocation.y)[maxLocation.x]=0;
            //rectangle(g_resultImage, matchLocation[i], Point(matchLocation[i].x + g_tempalteImage.cols, matchLocation[i].y + g_tempalteImage.rows), Scalar(0, 0, 0), -1, 8, 0);
        }
        else
        {
            if (minValue>PRECISION_THRESHOLD_MIN)break;
            matchLocation[i] = minLocation;
            //cout<<QString::number(g_resultImage.ptr<float>(minLocation.y)[minLocation.x]).toStdString()<<endl;
            g_resultImage.ptr<float>(minLocation.y)[minLocation.x]=1;
            //rectangle(g_resultImage, matchLocation[i], Point(matchLocation[i].x + g_tempalteImage.cols, matchLocation[i].y + g_tempalteImage.rows), Scalar(255, 255, 255), -1, 8, 0);
        }
        result += "("+QString::number(matchLocation[i].x)+","+QString::number(matchLocation[i].y)+")"+QString::number(minValue,10,5)+" "+QString::number(maxValue,10,5)+"\n";
        //circle(g_resultImage, matchLocation[i], 3, Scalar(255, 0, 0), 2, 8, 0);

    }
    match_num=i;
#ifdef SHOW_DEBUG_IMAGE
    for (i=i-1; i>=0;i--){
        rectangle(srcImage, matchLocation[i], Point(matchLocation[i].x + g_tempalteImage.cols, matchLocation[i].y + g_tempalteImage.rows), Scalar(0, 0, 255), 2, 8, 0);
        putText(g_resultImage, QString::number(i).toStdString(), Point(matchLocation[i].x-10,matchLocation[i].y+20), cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 1);

    }
    imshow("src_match", srcImage);
    imshow("match_table", g_resultImage);
#endif
    return QString::number(match_num)+"\n"+result;
    //imshow("原始图", srcImage);
    //imshow("效果图", g_resultImage);
}
//QString get_window_title(HWND hwnd)
//{
//    QString retStr;
//    wchar_t *temp;
//    int len;

//    len = GetWindowTextLengthW(hwnd);
//    if (!len)
//        return retStr;

//    temp = (wchar_t *)malloc(sizeof(wchar_t) * (len+1));
//    if (GetWindowTextW(hwnd, temp, len+1))
//    {
//        retStr = QString::fromWCharArray(temp);
//    }
//    free(temp);
//    return retStr;
//}

int main(int argc, char *argv[])
{
    QString Method;
    QString fileName1;
    QString fileName2;
//    g_nMatchMethod = 3;
//    g_srcImage = imread("C:/Users/H237428/Pictures/src.JPG");//读取图片数据
//    g_tempalteImage = imread("C:/Users/H237428/Pictures/temp2.JPG");//读取图片数据
    if (argc!=4)return -1;
    Method = argv[1];
    fileName1 = argv[2];
    fileName2 = argv[3];
    if (Method.isEmpty() || fileName1.isEmpty() || fileName2.isEmpty())
    {
        cout<<"11111111";
        return -1;
    }else{
        g_nMatchMethod = Method.toInt();
        g_srcImage = cv::imread(fileName1.toLatin1().data());//读取图片数据
        g_tempalteImage = cv::imread(fileName2.toLatin1().data());//读取图片数据
    }

    if (!g_srcImage.data || !g_tempalteImage.data)
    {
        cout<<"2222222";
        return -2;
        //g_srcImage = imread("D:/workcache/OpenCV/PROJECT/INSTALL_TEST/template_match_cmd/src.JPG");//读取图片数据
        //g_tempalteImage = imread("D:/workcache/OpenCV/PROJECT/INSTALL_TEST/template_match_cmd/temp.JPG");//读取图片数据
    }

    //namedWindow("原始图", CV_WINDOW_AUTOSIZE);
    //namedWindow("效果图", CV_WINDOW_AUTOSIZE);
   // createTrackbar("方法", "原始图", &g_nMatchMethod, g_nMaxTrackbarNum, on_matching);
    QString point=on_matching();

    //qDebug()<<"end";
    cout<<point.toStdString();
#ifdef SHOW_DEBUG_IMAGE
    waitKey();
#endif
    return 0;
}
