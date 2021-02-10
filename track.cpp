//#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include<opencv2/opencv.hpp>
#include<iostream>
#include <fstream>
#include <windows.h>
#include <string>

using namespace cv;
using namespace std;

/*
1：事件宏定义
enum
{
EVENT_MOUSEMOVE      =0,
EVENT_LBUTTONDOWN    =1,
EVENT_RBUTTONDOWN    =2,
EVENT_MBUTTONDOWN    =3,
EVENT_LBUTTONUP      =4,
EVENT_RBUTTONUP      =5,
EVENT_MBUTTONUP      =6,
EVENT_LBUTTONDBLCLK  =7,
EVENT_RBUTTONDBLCLK  =8,
EVENT_MBUTTONDBLCLK  =9
};
2：回调函数的定义方式（函数指针）
typedef void (*MouseCallback)(int event, int x, int y, int flags, void* userdata);
//! assigns callback for mouse events
CV_EXPORTS void setMouseCallback(const string& winname, MouseCallback onMouse, void* userdata = 0);
3：画线函数
line
Draws a line segment connecting two points.
C++: void line(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
4：画矩形函数
rectangle
Draws a simple, thick, or filled up-right rectangle.
C++: void rectangle(Mat& img, Rect rec, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
*/



vector<Point>  mousePoints;
vector<Point> ans;
vector<Point2f> gap;
vector<Point2f>ans2;
Point2f points;
int flag = 1;//用于切换状态的标志，0表示未进行跟踪，1表示手动跟踪，2表示自动跟踪，3表示已有数据
int key;//用于暂停的指针
int zhenshu = 0;//读取了第多少帧
int chuli = 1;//处理了第多少帧
Rect2d roi;
//int flag = 0;

int speed;
string videopath;
string outputfilename;
Mat Trans;
//读取txt文件
void readTxt(string file)
{
	ifstream infile;
	//infile.open(file.data());   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

	string s;
	//getline(infile, s);
	//speed=stoi(s);
	//getline(infile, videopath);
	//cout << videopath;
	//getline(infile, outputfilename);
	infile.open(file);
	infile >> s >> videopath >> outputfilename;
	speed = stoi(s);
	cout << "读取完三个" << endl;
	/*vector<float>points;
	for (int i = 0; i < 16; i++)
	{
	cout << "i" << "正读取"<<endl;
	string help;
	getline(infile, help);
	int mid = stoi(help);
	points.push_back((float)mid);
	}*/
	//Point2f AffinePoints0[4] = { Point2f(points[0], points[1]), Point2f(points[2], points[3]), Point2f(points[4], points[5]), Point2f(points[6], points[7]) };//视频
	//Point2f AffinePoints1[4] = { Point2f(points[8],points[9]), Point2f(points[10], points[11]), Point2f(points[12], points[13]), Point2f(points[14], points[15]) };//鸟瞰
	Point2f AffinePoints0[4] = { Point2f(932, 341), Point2f(69, 378), Point2f(936, 738), Point2f(1797, 385) };//视频
	Point2f AffinePoints1[4] = { Point2f(959,38), Point2f(419, 238), Point2f(960, 1060), Point2f(1500, 247) };//鸟瞰
	Trans = getPerspectiveTransform(AffinePoints0, AffinePoints1);
	infile.close();             //关闭文件输入流 
}

static void onMouse(int event, int x, int y, int, void*)
{
	/*if (event == EVENT_LBUTTONDOWN || event == EVENT_MOUSEMOVE || event == EVENT_RBUTTONDOWN)
	{
	return;
	}*/
	if (key != 32 && ans.size()<zhenshu)
	{
		points.x = x;
		points.y = y;
		mousePoints.push_back(points);
		ans.push_back(points);
		cout << "用户点击图像： 位置 " << ans.size() << ": " << x << ": " << y << endl;
	}
}

Rect2d change(Point p)
{
	double a = 150 + p.y, b = p.x - 35, c = 70, d = 150;
	Rect2d help(a, b, c, d);
	return help;
}
Point change2(Rect2d r)
{
	Point p;
	p.x = r.x + r.width / 2;
	p.y = r.y + r.height;
	return p;
}
////输出单击点的GRB值
//void on_mouse(int EVENT, int x, int y, int flags, void* userdata)
//{
//	Mat hh;
//	hh = *(Mat*)userdata;
//	Point p(x, y);
//	switch (EVENT)
//	{
//	case EVENT_LBUTTONDOWN:
//	{
//		printf("b=%d\t", hh.at<Vec3b>(p)[0]);
//		printf("g=%d\t", hh.at<Vec3b>(p)[1]);
//		printf("r=%d\n", hh.at<Vec3b>(p)[2]);
//		circle(hh, p, 2, Scalar(255), 3);
//	}
//	break;
//	}
//}
//
//
//void mouseHandler(int event, int x, int y, int flags, void* param)
//{
//	Point p(x, y);
//	//p.x = x, p.y = y;
//	IplImage* img0, *img1;
//	Mat image;
//	image = *(Mat*)param;
//	//img0 = (IplImage*)param;
//	img0 = &IplImage(image);
//	img1 = cvCloneImage(img0);
//
//
//	CvFont font;
//	uchar* ptr;
//	char label[20];
//
//	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, .8, .8, 0, 1, 8);
//	if (event == CV_EVENT_LBUTTONDOWN) //单击左键
//	{
//		/* read pixel */
//		ptr = cvPtr2D(img1, y, x, NULL);//返回指向特殊数组元素的指针 && cvGet2D返回指向特殊数组的元素
//										/*display the BGR value*/
//		sprintf_s(label, "(%d, %d, %d)", ptr[0], ptr[1], ptr[2]);
//		//sprintf_s(labels, "(%d, %d, %d)", ptr[0], ptr[1], ptr[2]);
//		//cvRectangle(img1, cvPoint(x, y - 12), cvPoint(x + 100, y + 4), CV_RGB(255, 0, 0), CV_FILLED, 8, 0);
//		cvPutText(img1, label, cvPoint(x, y), &font, CV_RGB(255, 0, 0));
//		//putText(image, label, p, 1, 2, CV_RGB(255, 0, 0), 2);
//		//putText(image, label, Point(x,y), 1, 2, CV_RGB(255, 0, 0), 2);
//		cout << "用户点击图像： 位置 " << x << ": " << y << endl;
//		cvShowImage("img", img1);
//		//imshow("image", image);
//	}
//}

int main()
{
	readTxt("setting.txt");


	//VideoCapture cap(0);
	//VideoCapture cap("C:\\Users\\28299\\Documents\\Visual Studio 2017\\Projects\\足球项目\\足球项目\\test.mp4");
	//VideoCapture cap("L:\\上海vs辽宁（切）\\3-2-2.mp4");
	//VideoCapture cap("3-2-2 00_00_00-00_03_00.mp4");
	VideoCapture cap;
	cap.open(videopath);
	if (!cap.isOpened()) //检查打开是否成功 
		cout << "打开失败" << endl;
	Mat frame;

	/*if (!cap.isOpened())
	{
	cout << "无法打开视频文件" << endl;
	return -1;
	}*/
	namedWindow("video");
	//namedWindow("pitch");

	//获得并输出点击处的位置
	//setMouseCallback("mouseCallback", onMouse);

	//opencv3 获得点击处的像素值
	//setMouseCallback("mouseCallback", on_mouse, &frame);

	//opencv2 获得点击处的像素值 和点击的位置
	//cvSetMouseCallback("mouseCallback", mouseHandler, &frame);
	Ptr<Tracker> tracker;
	while (1)
	{
		cap >> frame;
		if (frame.rows == 0 || frame.cols == 0)
		{
			cout << "帧为空" << endl;
			//break;
		}
		zhenshu++;
		cout << "帧数" << zhenshu << endl;
		if (zhenshu == 1)
		{
			imshow("video", frame);
			int help = waitKey(0);
			if (help == 32)
			{
				setMouseCallback("video", onMouse);
			}
		}
		else {
			if (frame.empty())
			{
				cout << "视频文件读取完成" << endl;
				break;
			}
			key = waitKey(40);
			if (flag == 1)
			{
				//key = waitKey(40);
				setMouseCallback("video", onMouse);
				//if (mousePoints.size() > 1)
				//{
				//	mousePoints.clear();
				//}
				//for (vector<Point>::iterator it = mousePoints.begin(); it != mousePoints.end(); it++)
				//{
				if (mousePoints.size() > 0)
					circle(frame, mousePoints[mousePoints.size() - 1], 15, CV_RGB(255, 0, 0));
				imshow("video", frame);
				//key = waitKey(30);
				Sleep(speed);
				//}
			}
			else if (flag == 2)
			{
				tracker->update(frame, roi);
				Point2f p = change2(roi);
				cout << "roi:" << roi.x << ' ' << roi.y << ' ' << roi.width << ' ' << roi.height << endl;
				//cout << "p:" << p.x << ' ' << p.y << endl;
				mousePoints.push_back(p);
				ans.push_back(p);
				cout << "自动跟踪图像： 位置 " << ans.size() << ": " << p.x << ": " << p.y << endl;
				rectangle(frame, roi, Scalar(255, 0, 0), 2, 1);
				//circle(frame, mousePoints[mousePoints.size() - 1], 5, CV_RGB(255, 0, 0));
				imshow("video", frame);
				//Sleep(40);
			}
			//暂停和退出
			if (key == 'q' || key == 'Q' || key == 27)
				break;
			else if (key == 32)
			{
				int pause = waitKey(0);
				if (pause == 32)continue;
				else if (pause == 'q' || pause == 'Q' || pause == 27)
					break;
				else if (pause == 't' || pause == 'T')//键盘上t(track)的asci码，表示切换成自动
				{
					cout << "**********************" << endl;
					cout << "切换至自动状态" << endl;
					flag = 2;
					//roi = { 1272, 652, 40, 60 };
					roi = selectROI("video", frame);
					//roi = change(ans[ans.size() - 1]);
					cout << "roi:" << roi.x << ' ' << roi.y << ' ' << roi.width << ' ' << roi.height << endl;
					tracker = TrackerKCF::create();
					tracker->init(frame, roi);
					continue;
				}
				else if (pause == 'h' || pause == 'H')//键盘上h(human)的asci码，表示切换成手动
				{
					cout << "**********************" << endl;
					cout << "切换至手动状态" << endl;
					flag = 1;
					continue;
				}

			}

			//暂停和退出
		}

	}
	for (int i = 0; i < ans.size(); i++)
	{
		Point2f pf(ans[i].x, ans[i].y);
		gap.push_back(pf);
		//file << now[i].x << ' ' << now[i].y << endl;
	}
	perspectiveTransform(gap, ans2, Trans);
	ofstream file(outputfilename);
	for (int i = 0; i < ans.size(); i++)
	{
		file << ans[i].x << ' ' << ans[i].y << endl;
	}
	ofstream file2("refer.txt");
	for (int i = 0; i < ans2.size(); i++)
	{
		file2 << ans2[i].x << ' ' << ans2[i].y << endl;
	}
	file2.close();
	//cap.release();
	destroyAllWindows();
	//system("pause");

}