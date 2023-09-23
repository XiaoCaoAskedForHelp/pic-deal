#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// ��ǿЧ��
// ѭ���еı任����ȫ������ǰ����ӳ���Ȼ����в��ң������Ϳ��Ա�����д��ģ�ļ��㡣

// �Ҷȷ�ת
void grayConvert(Mat image1_gray) {
	Mat output_image;
	output_image = image1_gray.clone();
	for (int i = 0; i < image1_gray.rows; i++)
	{
		for (int j = 0; j < image1_gray.cols; j++)
		{
			output_image.at<uchar>(i, j) = 255 - image1_gray.at<uchar>(i, j);  //�Ҷȷ�ת
		}
	}
	imshow(" output_image1", output_image);  //��ʾ��תͼ��
}


// �����任
void logTrans(Mat image1_gray) {
	Mat output_image;
	output_image = image1_gray.clone();
	for (int i = 0; i < image1_gray.rows; i++)
	{
		for (int j = 0; j < image1_gray.cols; j++)
		{
			output_image.at<uchar>(i, j) = 6 * log((double)(image1_gray.at<uchar>(i, j)) + 1);  //�����任 s=6*log(r+1)
		}
	}
	normalize(output_image, output_image, 0, 255, NORM_MINMAX);  //ͼ���һ����ת��0~255��Χ��
	convertScaleAbs(output_image, output_image);  //��������ת����CV_8U
	imshow(" output_image2", output_image);  //��ʾ�任ͼ��
}

//ڤ�ɣ�٤���任
void jiamaTrans(Mat image1_gray) {
	Mat output_image;
	output_image = image1_gray.clone();
	for (int i = 0; i < image1_gray.rows; i++)
	{
		for (int j = 0; j < image1_gray.cols; j++)
		{
			output_image.at<uchar>(i, j) = 6 * pow((double)image1_gray.at<uchar>(i, j), 0.5);  //ڤ�ɱ任 s=6*r^0.5
		}
	}
	normalize(output_image, output_image, 0, 255, NORM_MINMAX);  //ͼ���һ����ת��0~255��Χ��
	convertScaleAbs(output_image, output_image);  //��������ת����CV_8U
	imshow(" output_image4", output_image);  //��ʾ�任ͼ��
}


//��ֵ��
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
	convertScaleAbs(output_image, output_image);  //��������ת����CV_8U
	imshow(" output_image3", output_image);  //��ʾ�任ͼ��
}

int main()
{
	Mat image1, output_image, image1_gray;   //��������ͼ�����ͼ�񣬻Ҷ�ͼ��
	image1 = imread("img/test-lena.png");  //��ȡͼ��
	if (image1.empty())
	{
		cout << "��ȡ����" << endl;
		return -1;
	}

	cvtColor(image1, image1_gray, COLOR_BGR2GRAY);  //�ҶȻ�
	imshow(" image1_gray", image1_gray);   //��ʾ�Ҷ�ͼ��

	grayConvert(image1_gray);
	logTrans(image1_gray);
	jiamaTrans(image1_gray);
	nValueTrans(image1_gray);


	waitKey(0);  //��ͣ������ͼ����ʾ���ȴ���������
	return 0;
}
