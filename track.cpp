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
1���¼��궨��
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
2���ص������Ķ��巽ʽ������ָ�룩
typedef void (*MouseCallback)(int event, int x, int y, int flags, void* userdata);
//! assigns callback for mouse events
CV_EXPORTS void setMouseCallback(const string& winname, MouseCallback onMouse, void* userdata = 0);
3�����ߺ���
line
Draws a line segment connecting two points.
C++: void line(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
4�������κ���
rectangle
Draws a simple, thick, or filled up-right rectangle.
C++: void rectangle(Mat& img, Rect rec, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
*/



vector<Point>  mousePoints;
vector<Point> ans;
vector<Point2f> gap;
vector<Point2f>ans2;
Point2f points;
int flag = 1;//�����л�״̬�ı�־��0��ʾδ���и��٣�1��ʾ�ֶ����٣�2��ʾ�Զ����٣�3��ʾ��������
int key;//������ͣ��ָ��
int zhenshu = 0;//��ȡ�˵ڶ���֡
int chuli = 1;//�����˵ڶ���֡
Rect2d roi;
//int flag = 0;

int speed;
string videopath;
string outputfilename;
Mat Trans;
//��ȡtxt�ļ�
void readTxt(string file)
{
	ifstream infile;
	//infile.open(file.data());   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 

	string s;
	//getline(infile, s);
	//speed=stoi(s);
	//getline(infile, videopath);
	//cout << videopath;
	//getline(infile, outputfilename);
	infile.open(file);
	infile >> s >> videopath >> outputfilename;
	speed = stoi(s);
	cout << "��ȡ������" << endl;
	/*vector<float>points;
	for (int i = 0; i < 16; i++)
	{
	cout << "i" << "����ȡ"<<endl;
	string help;
	getline(infile, help);
	int mid = stoi(help);
	points.push_back((float)mid);
	}*/
	//Point2f AffinePoints0[4] = { Point2f(points[0], points[1]), Point2f(points[2], points[3]), Point2f(points[4], points[5]), Point2f(points[6], points[7]) };//��Ƶ
	//Point2f AffinePoints1[4] = { Point2f(points[8],points[9]), Point2f(points[10], points[11]), Point2f(points[12], points[13]), Point2f(points[14], points[15]) };//���
	Point2f AffinePoints0[4] = { Point2f(932, 341), Point2f(69, 378), Point2f(936, 738), Point2f(1797, 385) };//��Ƶ
	Point2f AffinePoints1[4] = { Point2f(959,38), Point2f(419, 238), Point2f(960, 1060), Point2f(1500, 247) };//���
	Trans = getPerspectiveTransform(AffinePoints0, AffinePoints1);
	infile.close();             //�ر��ļ������� 
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
		cout << "�û����ͼ�� λ�� " << ans.size() << ": " << x << ": " << y << endl;
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
////����������GRBֵ
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
//	if (event == CV_EVENT_LBUTTONDOWN) //�������
//	{
//		/* read pixel */
//		ptr = cvPtr2D(img1, y, x, NULL);//����ָ����������Ԫ�ص�ָ�� && cvGet2D����ָ�����������Ԫ��
//										/*display the BGR value*/
//		sprintf_s(label, "(%d, %d, %d)", ptr[0], ptr[1], ptr[2]);
//		//sprintf_s(labels, "(%d, %d, %d)", ptr[0], ptr[1], ptr[2]);
//		//cvRectangle(img1, cvPoint(x, y - 12), cvPoint(x + 100, y + 4), CV_RGB(255, 0, 0), CV_FILLED, 8, 0);
//		cvPutText(img1, label, cvPoint(x, y), &font, CV_RGB(255, 0, 0));
//		//putText(image, label, p, 1, 2, CV_RGB(255, 0, 0), 2);
//		//putText(image, label, Point(x,y), 1, 2, CV_RGB(255, 0, 0), 2);
//		cout << "�û����ͼ�� λ�� " << x << ": " << y << endl;
//		cvShowImage("img", img1);
//		//imshow("image", image);
//	}
//}

int main()
{
	readTxt("setting.txt");


	//VideoCapture cap(0);
	//VideoCapture cap("C:\\Users\\28299\\Documents\\Visual Studio 2017\\Projects\\������Ŀ\\������Ŀ\\test.mp4");
	//VideoCapture cap("L:\\�Ϻ�vs�������У�\\3-2-2.mp4");
	//VideoCapture cap("3-2-2 00_00_00-00_03_00.mp4");
	VideoCapture cap;
	cap.open(videopath);
	if (!cap.isOpened()) //�����Ƿ�ɹ� 
		cout << "��ʧ��" << endl;
	Mat frame;

	/*if (!cap.isOpened())
	{
	cout << "�޷�����Ƶ�ļ�" << endl;
	return -1;
	}*/
	namedWindow("video");
	//namedWindow("pitch");

	//��ò�����������λ��
	//setMouseCallback("mouseCallback", onMouse);

	//opencv3 ��õ����������ֵ
	//setMouseCallback("mouseCallback", on_mouse, &frame);

	//opencv2 ��õ����������ֵ �͵����λ��
	//cvSetMouseCallback("mouseCallback", mouseHandler, &frame);
	Ptr<Tracker> tracker;
	while (1)
	{
		cap >> frame;
		if (frame.rows == 0 || frame.cols == 0)
		{
			cout << "֡Ϊ��" << endl;
			//break;
		}
		zhenshu++;
		cout << "֡��" << zhenshu << endl;
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
				cout << "��Ƶ�ļ���ȡ���" << endl;
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
				cout << "�Զ�����ͼ�� λ�� " << ans.size() << ": " << p.x << ": " << p.y << endl;
				rectangle(frame, roi, Scalar(255, 0, 0), 2, 1);
				//circle(frame, mousePoints[mousePoints.size() - 1], 5, CV_RGB(255, 0, 0));
				imshow("video", frame);
				//Sleep(40);
			}
			//��ͣ���˳�
			if (key == 'q' || key == 'Q' || key == 27)
				break;
			else if (key == 32)
			{
				int pause = waitKey(0);
				if (pause == 32)continue;
				else if (pause == 'q' || pause == 'Q' || pause == 27)
					break;
				else if (pause == 't' || pause == 'T')//������t(track)��asci�룬��ʾ�л����Զ�
				{
					cout << "**********************" << endl;
					cout << "�л����Զ�״̬" << endl;
					flag = 2;
					//roi = { 1272, 652, 40, 60 };
					roi = selectROI("video", frame);
					//roi = change(ans[ans.size() - 1]);
					cout << "roi:" << roi.x << ' ' << roi.y << ' ' << roi.width << ' ' << roi.height << endl;
					tracker = TrackerKCF::create();
					tracker->init(frame, roi);
					continue;
				}
				else if (pause == 'h' || pause == 'H')//������h(human)��asci�룬��ʾ�л����ֶ�
				{
					cout << "**********************" << endl;
					cout << "�л����ֶ�״̬" << endl;
					flag = 1;
					continue;
				}

			}

			//��ͣ���˳�
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