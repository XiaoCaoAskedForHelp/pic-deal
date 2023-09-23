#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// 增强效率
// 循环中的变换，完全可以提前构造映射表，然后进行查找，这样就可以避免进行大规模的计算。

// 灰度翻转
void grayConvert(Mat image1_gray) {
	Mat output_image;
	output_image = image1_gray.clone();
	for (int i = 0; i < image1_gray.rows; i++)
	{
		for (int j = 0; j < image1_gray.cols; j++)
		{
			output_image.at<uchar>(i, j) = 255 - image1_gray.at<uchar>(i, j);  //灰度反转
		}
	}
	imshow(" output_image1", output_image);  //显示反转图像
}


// 对数变换
void logTrans(Mat image1_gray) {
	Mat output_image;
	output_image = image1_gray.clone();
	for (int i = 0; i < image1_gray.rows; i++)
	{
		for (int j = 0; j < image1_gray.cols; j++)
		{
			output_image.at<uchar>(i, j) = 6 * log((double)(image1_gray.at<uchar>(i, j)) + 1);  //对数变换 s=6*log(r+1)
		}
	}
	normalize(output_image, output_image, 0, 255, NORM_MINMAX);  //图像归一化，转到0~255范围内
	convertScaleAbs(output_image, output_image);  //数据类型转换到CV_8U
	imshow(" output_image2", output_image);  //显示变换图像
}

//冥律（伽马）变换
void jiamaTrans(Mat image1_gray) {
	Mat output_image;
	output_image = image1_gray.clone();
	for (int i = 0; i < image1_gray.rows; i++)
	{
		for (int j = 0; j < image1_gray.cols; j++)
		{
			output_image.at<uchar>(i, j) = 6 * pow((double)image1_gray.at<uchar>(i, j), 0.5);  //冥律变换 s=6*r^0.5
		}
	}
	normalize(output_image, output_image, 0, 255, NORM_MINMAX);  //图像归一化，转到0~255范围内
	convertScaleAbs(output_image, output_image);  //数据类型转换到CV_8U
	imshow(" output_image4", output_image);  //显示变换图像
}


//二值化
void nValueTrans(Mat image1_gray) {
	Mat output_image;
	output_image = image1_gray.clone();
	for (int i = 0; i < image1_gray.rows; i++)
	{
		for (int j = 0; j < image1_gray.cols; j++)
		{
			output_image.at<uchar>(i, j) = image1_gray.at<uchar>(i, j) > 125 ? 255 : 0;
		}
	}
	convertScaleAbs(output_image, output_image);  //数据类型转换到CV_8U
	imshow(" output_image3", output_image);  //显示变换图像
}

int main()
{
	Mat image1, output_image, image1_gray;   //定义输入图像，输出图像，灰度图像
	image1 = imread("img/test-lena.png");  //读取图像；
	if (image1.empty())
	{
		cout << "读取错误" << endl;
		return -1;
	}

	cvtColor(image1, image1_gray, COLOR_BGR2GRAY);  //灰度化
	imshow(" image1_gray", image1_gray);   //显示灰度图像

	grayConvert(image1_gray);
	logTrans(image1_gray);
	jiamaTrans(image1_gray);
	nValueTrans(image1_gray);


	waitKey(0);  //暂停，保持图像显示，等待按键结束
	return 0;
}
