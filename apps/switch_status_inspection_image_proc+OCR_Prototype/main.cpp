#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;


void get_setImagePixel(const char* imagePath, int x, int y)
{
    Mat image = imread(imagePath, 0);
    //  Mat image = imread(imagePath , 0);
    //Mat image = imread(imagePath, 1)
    int w = image.cols;
    int h = image.rows;

    int channels = image.channels();
    if (x < w && y < h)
    {
        if (channels == 1)
        {
            int pixel = image.at<uchar>(x, y);
        }
        else
        {
            int b = image.at<Vec3b>(x, y)[0]; // b = image.at<uchar>(x,y*3);
            cout << b << endl;
            int g = image.at<Vec3b>(x, y)[1]; //g = image.at<uchar>(x, y * 3 + 1);
            cout << g << endl;
            int r = image.at<Vec3b>(x, y)[2]; //r = image.at<uchar>(x, y * 3 + 1);
            cout << r << endl;

            image.at<Vec3b>(x, y)[0] = 255;
            image.at<Vec3b>(x, y)[1] = 255;
            image.at<Vec3b>(x, y)[2] = 255;
        }
    }
    imshow("cc", image);
}


void get_color_RED(Mat img)
{

    Mat srcImage, dstImage;
    Mat channel[3];
    srcImage = img;
    resize(srcImage, srcImage, Size(720, 540));
    //imshow(SRC_WINDOW_NAME, srcImage);

    int g_nHm = 9;
    split(srcImage, channel);

    channel[0] = channel[0].mul(.1 * g_nHm); // B    (mul: per-element matrix multiplication)
    channel[1] = channel[1].mul(.1 * g_nHm); // G
    channel[2] = channel[2] - channel[0] - channel[1]; // R
    channel[2] = 3 * channel[2];

    GaussianBlur(channel[2], channel[2], Size(9, 9), 2, 2);
    imshow("blur", channel[2]);

    vector<Vec3f> circles;
    HoughCircles(channel[2], circles, HOUGH_GRADIENT, 2, channel[2].rows / 4, 220, 45);

    for (size_t i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);

        circle(srcImage, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        circle(srcImage, center, radius, Scalar(255, 0, 0), 3, 8, 0);
        cout << "Pixel=" << (int)channel[1].at<uchar>(cvRound(circles[i][1]), cvRound(circles[i][0])) << "\t" << endl;
        cout << "radius_line=" << (int)channel[1].at<uchar>(cvRound(circles[i][1]) + radius + 3, cvRound(circles[i][0])) << "\t" << endl;
        cout << circles[i][0] << "\t" << circles[i][1] << "\t" << circles[i][2] << endl;
    }
    imshow("Red", srcImage);
    waitKey();
}

void get_RED_with_Contours(Mat img)
{
    Mat srcImage, dstImage;
    Mat channel[3];
    srcImage = img;
    resize(srcImage, srcImage, Size(720, 540));
    //imshow(SRC_WINDOW_NAME, srcImage);

    int g_nHm = 9;
    split(srcImage, channel);

    channel[0] = channel[0].mul(.1 * g_nHm); // B    (mul: per-element matrix multiplication)
    channel[1] = channel[1].mul(.1 * g_nHm); // G
    channel[2] = channel[2] - channel[0] - channel[1]; // R
    channel[2] = 3 * channel[2];

    GaussianBlur(channel[2], channel[2], Size(9, 9), 2, 2);


    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    int thresh = 100;
    /// Detect edges using canny
    Canny(channel[2], canny_output, thresh, thresh * 2, 3);
    /// Find contours
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    RNG rng(12345);
    /// Draw contours
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    vector<Rect> bbs;
    float mean_height = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        auto bb = boundingRect(contours.at(i));
        if (abs(bb.width - bb.height) < 80)
        {
            bbs.push_back(bb);
            mean_height += bb.height;
        }

        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
    }
    if(!bbs.empty())
        mean_height /= bbs.size();
    for (auto bb : bbs)
    {
        if (bb.height > mean_height)
            rectangle(srcImage, bb, Scalar(0, 255, 0), 1, 8, 0);
            //circle(srcImage, Point(bb.x + bb.width * .5, bb.y + bb.height * .5), bb.height * 0.5, Scalar(0, 0, 0), -1, 8, 0);
    }

    /// Show in a window
    /*namedWindow("Contours", WINDOW_AUTOSIZE);
    imshow("Contours", drawing);*/
    imshow("Red", srcImage);
    waitKey();
}


void get_GREEN_with_Contours(Mat img)
{
    Mat srcImage, dstImage;
    Mat channel[3];
    srcImage = img;
    resize(srcImage, srcImage, Size(720, 540));
    //imshow(SRC_WINDOW_NAME, srcImage);

    int g_nHm = 9;
    split(srcImage, channel);

    channel[0] = channel[0].mul(.1 * g_nHm); // B    (mul: per-element matrix multiplication)
    channel[2] = channel[2].mul(.1 * g_nHm); // R
    channel[1] = channel[1] - channel[0] - channel[2]; // G
    channel[1] = 3 * channel[1];

    //Mat ch_green;
    //inRange(channel[1], Scalar(0, 110, 80), Scalar(20, 255, 180), ch_green);

    GaussianBlur(channel[1], channel[1], Size(9, 9), 2, 2);


    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    int thresh = 100;
    /// Detect edges using canny
    Canny(channel[1], canny_output, thresh, thresh * 2, 3);
    /// Find contours
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    RNG rng(12345);
    /// Draw contours
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    vector<Rect> bbs;
    float mean_height = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        auto bb = boundingRect(contours.at(i));
        if (abs(bb.width - bb.height) < 30)
        {
            bbs.push_back(bb);
            mean_height += bb.height;
        }

        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
    }
    if (!bbs.empty())
        mean_height /= bbs.size();
    for (auto bb : bbs)
    {
        if (bb.height > mean_height)
            circle(srcImage, Point(bb.x + bb.width * .5, bb.y + bb.height * .5), bb.height * 0.5, Scalar(255, 255, 255), 1, 8, 0);
    }

    /// Show in a window
    /*namedWindow("Contours", WINDOW_AUTOSIZE);
    imshow("Contours", drawing);*/
    imshow("Green", srcImage);
    waitKey();
}


void get_color_GREEN(Mat img)
{
    Mat srcImage, dstImage1, dstImage2;
    Mat channel[3];
    srcImage = img;
    resize(srcImage, srcImage, Size(720, 540));

    //imshow(SRC_WINDOW_NAME, srcImage);

    float g_nHm = 9.5;
    split(srcImage, channel);

    channel[0] = channel[0].mul(.1 * g_nHm); // B    (mul: per-element matrix multiplication)
    channel[2] = channel[2].mul(.1 * g_nHm); // R
    channel[1] = channel[1] - channel[0] - channel[2]; // G
    channel[1] = 3 * channel[1];
    //imshow("non-blur", channel[2]);

    //medianBlur(channel[1], channel[1], 15);
    GaussianBlur(channel[1], channel[1], Size(9, 9), 2, 2);
    imshow("blur-g", channel[1]);

    vector<Vec3f> circles;
    HoughCircles(channel[1], circles, HOUGH_GRADIENT, 2, channel[1].rows / 4, 60, 45);

    for (size_t i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);

        circle(srcImage, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        circle(srcImage, center, radius, Scalar(255, 0, 0), 3, 8, 0);
        cout << "Pixel=" << (int)channel[1].at<uchar>(cvRound(circles[i][1]), cvRound(circles[i][0])) << "\t" << endl;
        cout << "radius_line=" << (int)channel[1].at<uchar>(cvRound(circles[i][1]) + radius + 3, cvRound(circles[i][0])) << "\t" << endl;
        cout << circles[i][0] << "\t" << circles[i][1] << "\t" << circles[i][2] << endl;
    }
    imshow("Green", srcImage);
    waitKey();


}


/*
int main()
{
	get_color_RED();
	get_color_GREEN();

	waitKey();
	return 0;
}
*/
int main(int argc, char** argv)
{
    std::string imPath = (argc > 1) ? argv[1] : "assets/images/IMG_5861.JPG";
    Mat src = imread(imPath);
    if (src.empty())
    {
        std::cerr << "Failed to read image: " << imPath << std::endl;
        return 1;
    }
    //get_color_RED(src);
    //get_color_GREEN(src);
    //return 0;
    /*Mat frame = imread("sample.jpg", 1);
    get_RED_with_Contours(frame);
    get_GREEN_with_Contours(frame);
    get_color_RED(frame);
    get_color_GREEN(frame);*/
        get_RED_with_Contours(src);
        get_GREEN_with_Contours(src);
        waitKey();
    return 0;
}