// pic_deal.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

//OpenCV中使用数组来表示直方图，这里的数组和用来表示其他数据额数组没有任何区别。
//这意味着对于一维或二维数组，你可以使用cv::Mat(NX1或者1XN来表示一维数组），
//也可以使用vector<>或稀疏矩阵类型。

// 获取直方图Mat数组Hist的函数

//@para gray:需要统计的图   Hist：用于存放统计数据
void GetHist(Mat gray, Mat& Hist)    //统计8Bit量化图像的灰度直方图
{
	const int channels[1] = { 0 }; //通道索引
	float inRanges[2] = { 0,255 };  //像素范围
	const float* ranges[1] = { inRanges };//像素灰度级范围
	const int bins[1] = { 256 };   //直方图的维度
	//images:带统计直方图的图像数组
	//nimages:输入图像的数目
	//channels:需要统计的通道索引数组 一般灰度图就是0通道
	//mask:掩码 一般填Mat()
	//hist:输出的统计直方图结果，是一个dims维度的数组。不过用OpenCV中Mat类型的变量存储
	//dims:hist的列数、需要计算直方图的维度
	//histSize:存放每个维度直方图数组的尺寸
	//ranges:每个通道灰度值的取值范围
	calcHist(&gray, 1, channels, Mat(), Hist, 1, bins, ranges);
}

// 申请一块背景为纯黑的图像，再根据Hist的值转化为点坐标从而画出一个个白色的矩形以完成直方图的绘制
void ShowHist(Mat& Hist, cv::String name)
{
	//准备绘制直方图
	int hist_w = 512;
	int hist_h = 400;
	int width = 2; // 一个矩形的横坐标长度应该为一个像素级所占的长度，这里我们假定一个像素级占长度为width=2
	Mat histImage = Mat::zeros(hist_h, hist_w, CV_8UC3);   //准备histImage为全黑背景色，大小为512*400
	// Hist就是一个256行1列Mat类型的变量
	for (int i = 0; i < Hist.rows; i++)
	{
		// pt1为矩形的一个顶点，pt2为矩形对角线上的另一个顶点
		// pt1的横坐标值为width*(i)，pt2的横坐标值为width*(i+1)；
		// Hist.at<float>(i)取出在该像素级上像素点的个数，这个数字可能远远大于hist_h（512），因为直方图我们只希望看到图像像素级分布的大致，所以我们除以一个20保证不会超出图像的边界。
		// （图片，长方形框左上角坐标, 长方形框右下角坐标， 字体颜色，字体粗细）
		rectangle(histImage, Point(width * (i), hist_h - 1), Point(width * (i + 1), hist_h - cvRound(Hist.at<float>(i) / 10)),
			Scalar(255, 255, 255), -1);
	}
	namedWindow(name, WINDOW_AUTOSIZE);
	imshow(name, histImage);
	//waitKey(0);
}

int gray_sum = 0;  //像素总数

// 均衡化函数
Mat equalize_hist(Mat& image)
{
	Mat output = image.clone();
	Mat hist;
	gray_sum = image.cols * image.rows;

	GetHist(image, hist);

	double gray_prob[256] = { 0 };  //记录灰度分布密度
	//统计灰度频率
	for (int i = 0; i < 256; i++)
	{
		gray_prob[i] = ((double)hist.at<float>(i) / gray_sum);
	}

	double gray_distribution[256] = { 0 };  //记录累计密度
	//计算累计密度
	gray_distribution[0] = gray_prob[0];
	for (int i = 1; i < 256; i++)
	{
		gray_distribution[i] = gray_distribution[i - 1] + gray_prob[i];
	}

	int gray_equal[256] = { 0 };  //均衡化后的灰度值
	//重新计算均衡化后的灰度值，四舍五入。参考公式：(N-1)*T+0.5
	for (int i = 0; i < 256; i++)
	{
		gray_equal[i] = (uchar)(255 * gray_distribution[i] + 0.5);
	}

	//直方图均衡化,更新原图每个点的像素值
	for (int i = 0; i < output.rows; i++)
	{
		uchar* p = output.ptr<uchar>(i);
		for (int j = 0; j < output.cols; j++)
		{
			p[j] = gray_equal[p[j]];
		}
	}

	return output;
}

// 规范化函数
void HistogramMatching(Mat gray) {
	Mat image1, image1_gray, hist, hist1, hist2, image_matched, hist_matched;
	image1 = imread("img/test-lena.png", IMREAD_COLOR);  //读取图像；

	if (image1.empty())
	{
		cout << "读取错误" << endl;
		return;
	}

	cvtColor(image1, image1_gray, COLOR_BGR2GRAY);  //灰度化
	imshow(" image1_gray", image1_gray);   //显示灰度图像
	GetHist(image1_gray, hist);
	ShowHist(hist, "image1_hist");

	//均衡化处理
	equalizeHist(image1_gray, image1_gray);
	equalizeHist(gray, gray);

	GetHist(image1_gray, hist1);
	GetHist(gray, hist2);

	//ShowHist(hist1, "hist1");
	//ShowHist(hist2, "hist2");

	//计算两个均衡化图像直方图的累积概率
	float hist1_cdf[256] = { hist1.at<float>(0) };
	float hist2_cdf[256] = { hist2.at<float>(0) };

	for (int i = 1; i < 256; i++)
	{
		hist1_cdf[i] = (hist1_cdf[i - 1] + hist1.at<float>(i));
		hist2_cdf[i] = (hist2_cdf[i - 1] + hist2.at<float>(i));
	}

	for (int i = 0; i < 256; i++)
	{
		hist1_cdf[i] = hist1_cdf[i] / (image1_gray.rows * image1_gray.cols);
		hist2_cdf[i] = hist2_cdf[i] / (gray.rows * gray.cols);
	}

	float diff_cdf[256][256];
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++)
		{
			diff_cdf[i][j] = fabs(hist1_cdf[i] - hist2_cdf[j]);
		}
	}

	// LUT(image1_gray, lut, image_enhanced);  //图像中进行映射
	Mat lut(1, 256, CV_8U);
	for (int i = 0; i < 256; i++)
	{
		//查找源灰度级为i的映射灰度和i的累积概率差最小(灰度接近)的规定化灰度
		float min = diff_cdf[i][0];
		int index = 0;
		for (int j = 0; j < 256; j++) {
			if (min > diff_cdf[i][j]) {
				min = diff_cdf[i][j];
				index = j;
			}
		}
		lut.at<uchar>(i) = index;
	}

	LUT(image1_gray, lut, image_matched);  //图像中进行映射
	imshow("image_matched", image_matched);

	GetHist(image_matched, hist_matched);
	ShowHist(hist_matched, "hist_matched");
}


int main()
{
	Mat image, gray, hist, image_enhanced, hist_enhanced, hist_enhance;   //定义输入图像，灰度图像, 直方图
	//cv2.IMREAD_COLOR：默认参数，读入一副彩色图片，忽略alpha通道，可以直接写1
	//cv2.IMREAD_GRAYSCALE：读入灰度图片，可以直接写0
	//cv2.IMREAD_UNCHANGED：顾名思义，读入完整图片，包括alpha通道，可以直接写 - 1
	image = imread("img/test.jpg", IMREAD_COLOR);  //读取图像；

	if (image.empty())
	{
		cout << "读取错误" << endl;
		return -1;
	}
	else {
		cout << "读取成功" << endl;
		cout << "通道数：" << image.channels() << endl;
	}
	// OpenCV里的颜色空间转换函数，可以实现RGB颜色向HSV、HSI等颜色空间的转换，也可以转换为灰度图像。
	// Gray = 0.1140 * B + 0.5870 * G + 0.2989 * R
	cvtColor(image, gray, COLOR_BGR2GRAY);
	GetHist(gray, hist);
	ShowHist(hist, "image_hist");
	imshow("image_gray", gray);   //显示灰度图像

	// -----------------直方图均衡化----------------------------------

	//Mat image_enhance = equalize_hist(gray);
	//GetHist(image_enhance, hist_enhanced);
	//ShowHist(hist_enhanced, "hist_enhanced_mine");
	//imshow("enhanced_mine", image_enhance);   //显示增强图像

	//equalizeHist(gray, image_enhanced);//直方图均衡化
	//GetHist(image_enhanced, hist_enhance);
	//ShowHist(hist_enhance, "hist_enhanced_offical");
	//imshow("enhanced_offical", image_enhanced);   //显示增强图像

	// ---------------------直方图规范化-----------------------------------

	HistogramMatching(gray);

	// --------------------------------------------------------
	waitKey(0);  //暂停，保持图像显示，等待按键结束
	return 0;
}
