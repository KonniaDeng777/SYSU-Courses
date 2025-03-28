#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <iostream>
#include<conio.h>
#include<Windows.h>

using namespace cv;
using namespace std;

void resize()
{
	string path;
	cout << "Please enter the filename of the image:" << endl;
	cin >> path;
	Mat img = imread(path);
	Mat src = imread(path);
	if (src.empty())
	{
		printf("this image cannot load");
		return;
	}
	int length, height;
	cout << "Please enter the length and height to resize:" << endl;
	cin >> length >> height;
	Size srcSize = Size(length, height);  //填入任意指定尺寸
	resize(src, src, srcSize, 0, 0, INTER_LINEAR);
	string pathwrite;
	cout << "Please enter the filename to store:" << endl;
	cin >> pathwrite;
	imwrite(pathwrite, src);  //保存图片
	imshow("original", img);
	imshow("resize", src);
	waitKey(0);
	return;
}

//更改图像大小函数
Mat my_resize(Mat& img, int width, int height)
{
	// 1, 定义缩放后的图像大小，行列缩放比例
	Mat output = Mat::zeros(Size(width, height), CV_8UC1);
	float width_scale = (float)img.cols / width;     // 列缩放比例，相对于算法前面讲的k1
	float height_scale = (float)img.rows / height;   // 行缩放比例，即k2

	// 2, 采样
	for (int i = 0; i < height; i++)  // 注意i,j的范围, i < height * img.rows / height;
	{
		for (int j = 0; j < width; j++)
		{
			output.at<uchar>(i, j) = img.at<uchar>(round(i * height_scale), round(j * width_scale));
		}
	}
	return output;
}

//压缩 解压缩
void compress_and_decompress()
{
	Mat img;
	cout << "Please enter the filename of the image:" << endl;
	string path;
	cin >> path;
	img = imread(path);
	int quality = 50; //压缩比率0～100
	vector<uint8_t> imageData;
	vector<int> compress_params;
	compress_params.push_back(IMWRITE_PNG_COMPRESSION);
	compress_params.push_back(quality);
	Mat imgup(img.size() * 2, img.type());
	for (int row = 0, rowd = 0; row < img.rows && rowd < img.rows * 2; rowd += 2, row++)
	{
		for (int col = 0, cold = 0; col < img.cols && cold < img.cols * 2; cold += 2, col++)
		{
			imgup.at<Vec3b>(rowd, cold)[0] = img.at<Vec3b>(row, col)[0];
			imgup.at<Vec3b>(rowd, cold)[1] = img.at<Vec3b>(row, col)[1];
			imgup.at<Vec3b>(rowd, cold)[2] = img.at<Vec3b>(row, col)[2];

			imgup.at<Vec3b>(rowd + 1, cold)[0] = img.at<Vec3b>(row, col)[0];
			imgup.at<Vec3b>(rowd + 1, cold)[1] = img.at<Vec3b>(row, col)[1];
			imgup.at<Vec3b>(rowd + 1, cold)[2] = img.at<Vec3b>(row, col)[2];

			imgup.at<Vec3b>(rowd, cold + 1)[0] = img.at<Vec3b>(row, col)[0];
			imgup.at<Vec3b>(rowd, cold + 1)[1] = img.at<Vec3b>(row, col)[1];
			imgup.at<Vec3b>(rowd, cold + 1)[2] = img.at<Vec3b>(row, col)[2];

			imgup.at<Vec3b>(rowd + 1, cold + 1)[0] = img.at<Vec3b>(row, col)[0];
			imgup.at<Vec3b>(rowd + 1, cold + 1)[1] = img.at<Vec3b>(row, col)[1];
			imgup.at<Vec3b>(rowd + 1, cold + 1)[2] = img.at<Vec3b>(row, col)[2];
		}
	}
	Mat imgdown(img.size() / 2, img.type());
	for (int row = 0, rowd = 0; row < img.rows && rowd < img.rows / 2; row += 2, rowd++)
	{
		for (int col = 0, cold = 0; col < img.cols && cold < img.cols / 2; col += 2, cold++)
		{
			imgdown.at<Vec3b>(rowd, cold)[0] = img.at<Vec3b>(row, col)[0];
			imgdown.at<Vec3b>(rowd, cold)[1] = img.at<Vec3b>(row, col)[1];
			imgdown.at<Vec3b>(rowd, cold)[2] = img.at<Vec3b>(row, col)[2];
		}
	}
	imencode(".png", img, imageData, compress_params);
	vector<uint8_t> p_data = imageData;
	Mat image = imdecode(p_data, -1);
	imshow("original", img);
	imshow("decompress", image);
	waitKey(0);
}

//显示图片
void display()
{
	cout << "Please enter the filename of the image:" << endl;
	string path;
	cin >> path;
	Mat img = imread(path);
	imshow("Image", img);
	waitKey(0);
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
	imshow("original", image);
	imshow("gray", imageGray);
	waitKey(0);
}



//输出界面
int show()
{
	cout << "Enter 1:彩色图像转换为灰度图像" << endl;
	cout << "Enter 2:图像尺寸缩放" << endl;
	cout << "Enter 3:显示指定图像" << endl;
	cout << "Enter 4:压缩和解压缩" << endl;
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
			cout << "press any key to exit!" << endl;
			getchar();
			exit(0);
			break;
		case 1:
			ConvertRGB2GRAY();
			break;
		case 2:
			resize();
			break;
		case 3:
			display();
			break;
		case 4:
			compress_and_decompress();
			break;
		default:
			cout << "error" << endl;
			break;
		}
		system("cls");
	}

	return 0;
}
