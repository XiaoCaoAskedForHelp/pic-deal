#define _CRT_SECURE_NO_WARNINGS


#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;

//处理越界：
//
//如果用uchar类型表示一个图像，假设两个图像都是255，加起来就是500，由于uchar是8位表示不了五百，
//图像就有了越界，实际上OpenCV在实际操作中会根据越界来进行一个处理，如果大于255的话就会让它等于255，
//如果它小于0的话，就会让它等于0，0 - 255正好是我们uchar可以表示的范围。

int  main(int argc, char** argv) {
	Mat src1 = imread("img/WindowsLogo.jpg");//读取图片1
	Mat src2 = imread("img/LinuxLogo.jpg");//读取图片2
	if (src1.empty() || src2.empty()) {
		printf("Could not find image file");
		return -1;
	}
	imshow("input1", src1);//显示图片1
	imshow("input2", src2);//显示图片2
	//代码演示
	Mat dst1;
	add(src1, src2, dst1);//加法操作输入图片1，图片2，输出图片3
	imshow("add-demo", dst1);

	Mat dst2;
	subtract(src1, src2, dst2);//减法操作输入图片1，图片2，输出图片3
	imshow("subtract-demo", dst2);

	Mat dst3;
	multiply(src1, src2, dst3);//乘法操作输入图片1，图片2，输出图片3
	imshow("multiply-demo", dst3);

	Mat dst4;
	divide(src1, src2, dst4);//除法操作输入图片1，图片2，输出图片3
	imshow("divide-demo", dst4);

	waitKey(0);
	destroyAllWindows();
	return 0;
}