#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include<opencv2/xfeatures2d/nonfree.hpp>
#include<iostream>
#include<math.h>
using namespace std;
using namespace cv;
vector<array<float, 2>> points;
void mouseCallbackTrans(int event, int x, int y, int flags, void* userdata)
{
	static int x0 = 0;
	static int y0 = 0;
	static int count = 0;
	Mat img = *(Mat*)userdata;
	if (count >= 6)
		return;
	if (event == EVENT_LBUTTONDOWN)
	{
		switch (x0 == 0 && y0 == 0)
		{
		case true:
			x0 = x;
			y0 = y;
			circle(img, cv::Point(x, y), 5, cv::Scalar::all(255), -1, cv::LINE_AA);
			cout << "original point: X=" << x << "Y=" << y << endl;
			points.push_back({ (float)x, (float)y });
			break;
		case false:
			circle(img, cv::Point(x, y), 5, (30, 30, 200, 0), -1, cv::LINE_AA);
			cout << "new point: X=" << x << "Y=" << y << endl;
			points.push_back({ (float)x, (float)y });
			line(img, cv::Point(x0, y0), cv::Point(x, y), cv::Scalar::all(255), 1, cv::LINE_AA);
			x0 = 0;
			y0 = 0;
			break;
		}
		count++;
	}
	imshow("original image", img);
}
class Transformer
{
private:
	Mat img;
	Mat res;
	Mat oriImg;
	Size size;
public:
	Transformer(Mat img)
	{
		this->img = img;
		img.copyTo(oriImg);
		size = img.size().width >= img.size().height ?
			Size(img.size().width, img.size().width) :
			Size(img.size().height, img.size().height);
	}
	void start()
	{
		imshow("original image", img);
		setMouseCallback("original image", mouseCallbackTrans, &img);
		cout << "press any key to continue" << endl;
		waitKey(0);

		Point2f srcPoint[3] = { { points[0][0], points[0][1] },{ points[2][0], points[2][1]},{  points[4][0], points[4][1] } };
		Point2f dstPoint[3] = { { points[1][0], points[1][1] },{ points[3][0], points[3][1] },{  points[5][0], points[5][1]} };

		Mat affine = getAffineTransform(srcPoint, dstPoint);
		cout << "the transform matrix is : " << endl << affine << endl;
		cv::warpAffine(oriImg, res, affine, size, cv::INTER_LINEAR);
		imshow("Result", res);
		waitKey(0);
	}
};
int main(void)
{
	string imgPath;
	cout << "input image path:" << endl;
	cin >> imgPath;
	Mat img = imread(imgPath);
	if (img.empty())
		return -1;
	Transformer trans = Transformer(img);
	trans.start();
	return 0;
}