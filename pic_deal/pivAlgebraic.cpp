#define _CRT_SECURE_NO_WARNINGS


#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;

//����Խ�磺
//
//�����uchar���ͱ�ʾһ��ͼ�񣬼�������ͼ����255������������500������uchar��8λ��ʾ������٣�
//ͼ�������Խ�磬ʵ����OpenCV��ʵ�ʲ����л����Խ��������һ�������������255�Ļ��ͻ���������255��
//�����С��0�Ļ����ͻ���������0��0 - 255����������uchar���Ա�ʾ�ķ�Χ��

int  main(int argc, char** argv) {
	Mat src1 = imread("img/WindowsLogo.jpg");//��ȡͼƬ1
	Mat src2 = imread("img/LinuxLogo.jpg");//��ȡͼƬ2
	if (src1.empty() || src2.empty()) {
		printf("Could not find image file");
		return -1;
	}
	imshow("input1", src1);//��ʾͼƬ1
	imshow("input2", src2);//��ʾͼƬ2
	//������ʾ
	Mat dst1;
	add(src1, src2, dst1);//�ӷ���������ͼƬ1��ͼƬ2�����ͼƬ3
	imshow("add-demo", dst1);

	Mat dst2;
	subtract(src1, src2, dst2);//������������ͼƬ1��ͼƬ2�����ͼƬ3
	imshow("subtract-demo", dst2);

	Mat dst3;
	multiply(src1, src2, dst3);//�˷���������ͼƬ1��ͼƬ2�����ͼƬ3
	imshow("multiply-demo", dst3);

	Mat dst4;
	divide(src1, src2, dst4);//������������ͼƬ1��ͼƬ2�����ͼƬ3
	imshow("divide-demo", dst4);

	waitKey(0);
	destroyAllWindows();
	return 0;
}