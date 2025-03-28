#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <iostream>
#include<conio.h>
#include<Windows.h>

using namespace cv;
using namespace std;

void display()
{
	cout << "Please enter the filename of the image:" << endl;
	string path;
	cin >> path;
	Mat img = imread(path);
	imshow("Image", img);
	waitKey(0);
}

void tranGray()
{
	cout << "Please enter the filename of the image:" << endl;
	string path;
	cin >> path;
	Mat img = imread(path);
	if (img.empty())
	{
		printf("this image cannot load");
		return ;
	}
	Mat imgGray;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	string writepath;
	cout << "Please enter the filename to store:" << endl;
	cin >> writepath;
	imwrite(writepath, imgGray);
	imshow("Image", img);
	imshow("ImageGray", imgGray);
	waitKey(0);
}

void resize()
{
	string path;
	cout << "Please enter the filename of the image:" << endl;
	cin >> path;
	Mat src = imread(path);
	if (src.empty())
	{
		printf("this image cannot load");
		return ;
	}
	int length, height;
	cout << "Please enter the length and height to resize:" << endl;
	cin >> length >> height;
	getchar();
	Size srcSize = Size(length,height);  //填入任意指定尺寸
	resize(src, src, srcSize, 0, 0, INTER_LINEAR);
	string pathwrite;
	cout << "Please enter the filename to store:" << endl;
	cin >> pathwrite;
	imwrite(pathwrite, src);  //保存图片
	cout << src.size() << endl;
	imshow("resize", src);
	waitKey(0);
	return ;
}



void ConvertRGB2GRAY()
{
	Mat image, imageGray;
	string path;
	cout << "Please enter the filename of the image:" << endl;
	cin >> path;
	image = imread(path);
	if (!image.data || image.channels() != 3)
	{
		cout << "Error" << endl;
		return;
	}
	//创建一张单通道的灰度图像
	imageGray = Mat::zeros(image.size(), CV_8UC1);
	//取出存储图像像素的数组的指针
	uchar* pointImage = image.data;
	uchar* pointImageGray = imageGray.data;
	//取出图像每行所占的字节数
	size_t stepImage = image.step;
	size_t stepImageGray = imageGray.step;
	for (int i = 0; i < imageGray.rows; i++)
	{
		for (int j = 0; j < imageGray.cols; j++)
		{
			pointImageGray[i * stepImageGray + j] =
				(uchar)(0.114 * pointImage[i * stepImage + 3 * j] +
					0.587 * pointImage[i * stepImage + 3 * j + 1] +
					0.299 * pointImage[i * stepImage + 3 * j + 2]);
		}
	}
	string writepath;
	cout << "Please enter the filename to store:" << endl;
	cin >> writepath;
	imwrite(writepath,imageGray);
	imshow("gray", imageGray);
	waitKey(0);
}

void Resize()
{
	string path;
	cout << "Please enter the filename of the image:" << endl;
	cin >> path;
	Mat src = imread(path);
	int length, height;
	cout << "Please enter the length and height to resize:" << endl;
	cin >> length >> height;
	getchar();
	src.rows = length;
	src.cols = height;
	string pathwrite;
	cout << "Please enter the filename to store:" << endl;
	cin >> pathwrite;
	imwrite(pathwrite, src);  //保存图片
	cout << src.size() << endl;
	imshow("resize", src);
	waitKey(0);
	return;
}


int show()
{
	cout << "Enter 1:彩色图像转换为灰度图像" << endl;
	cout << "Enter 2:图像尺寸缩放" << endl;
	cout << "Enter 3:显示指定图像" << endl;
	cout << "Enter 0:Exit" << endl;
	int n;
	cout << "Please enter your choice:" << endl;
	cin >> n;
	getchar();
	return n;
}

int main()
{
	while (1)
	{
		switch (show())
		{
		case 0:
			cout << "Press any key to exit!" << endl;
			getchar();
			exit(0);
			break;
		case 1:
			tranGray();
			break;
		case 2:
			Resize();
			break;
		case 3:
			display();
			break;
		default:
			cout << "Error" << endl;
			break;
		}
		system("cls");
	}

	return 0;
}
