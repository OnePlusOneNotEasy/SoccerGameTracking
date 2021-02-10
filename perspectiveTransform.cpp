#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
 
struct userdata
{
    Mat img;
    vector<Point2f> points;
};
 
void mouseHandler(int event, int x, int y, int flags, void* data_ptr)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        userdata* data = ((userdata*)data_ptr);
        circle(data->img, Point(x, y), 3, Scalar(0, 255, 255), 5);
        namedWindow("Image_window", 0);
        setWindowProperty("Image_window", WND_PROP_AUTOSIZE, WINDOW_AUTOSIZE);
        imshow("Image_window", data->img);
 
        if (data->points.size() < 4)
        {
            data->points.push_back(Point2f(x, y));
        }
    }
}
 
int main(int argc, char** argv)
{
    Mat im_c = imread("/Users/jialeyang/Desktop/3.jpg");
    userdata data_c;
    data_c.img = im_c;
    namedWindow("Image_window", 0);
    setWindowProperty("Image_window", WND_PROP_AUTOSIZE, WINDOW_AUTOSIZE);
    imshow("Image_window", im_c);
    setMouseCallback("Image_window", mouseHandler, &data_c);
    waitKey(0);
 
    Mat im_e = imread("/Users/jialeyang/Desktop/2.jpg");
    userdata data_e;
    data_e.img = im_e;
    namedWindow("Image_window", 0);
    setWindowProperty("Image_window", WND_PROP_AUTOSIZE, WINDOW_AUTOSIZE);
    imshow("Image_window", im_e);
    setMouseCallback("Image_window", mouseHandler, &data_e);
    waitKey(0);
    
 
    Mat warpMatrix = getPerspectiveTransform(data_e.points, data_c.points);
 
    cout << "warpMatrix=  " << warpMatrix << endl;
    
    Mat warpPerspective_dst;
    warpPerspective_dst = Mat::zeros( im_c.rows, im_c.cols, im_c.type() );

 
    warpPerspective(im_e, warpPerspective_dst, warpMatrix, im_e.size());
    namedWindow("Effect", WINDOW_AUTOSIZE);
    imshow("Effect", warpPerspective_dst);
    waitKey(0);
    
    return 0;
}