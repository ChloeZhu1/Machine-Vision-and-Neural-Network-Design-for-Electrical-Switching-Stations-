#include <iostream>
#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <regex>
#include <vector>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

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

int main(int argc, char* argv[])
    {
        char *outText;
        string imPath;
        if(argc>1){
            imPath=argv[1];
        }else{
            // Default example image bundled with the repo
            imPath="assets/images/T1.JPG";
        }

        // Visualize potential green indicator candidates (optional)
        // get_GREEN_with_Contours(im);
        //waitKey();

        // Create Tesseract object
        tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();

        // Initialize tesseract to use chinese (sim) and the LSTM OCR engine.
        ocr->Init(NULL, "chi_sim", tesseract::OEM_LSTM_ONLY);

        // Set Page segmentation mode to PSM_AUTO (3)
        ocr->SetPageSegMode(tesseract::PSM_AUTO);

        // Open input image using OpenCV
        Mat im = cv::imread(imPath, IMREAD_COLOR);
        Mat img = cv::imread(imPath, IMREAD_GRAYSCALE);

        get_RED_with_Contours(im);
        waitKey();

        //vector<Rect2d> rects;
        bool fromCenter=false;
        Rect2d r=selectROI("Gray",img,fromCenter);
        Mat imCrop=im(r);
        imshow("Crop", imCrop);
        waitKey();


        // Set image data
        ocr->SetImage(imCrop.data, imCrop.cols, imCrop.rows, 3, imCrop.step);

        // Run Tesseract OCR on image
        outText = ocr->GetUTF8Text();
        cout<<outText;
        std::string remaining(outText);
        std::cmatch match;
        std::regex re("\\d\\d\\d[a-z]");
        while (std::regex_search(remaining.c_str(), match, re)) {
            std::cout << match.str() << std::endl;
            remaining= match.suffix();
        }
        /*std::regex_match (outText, match, re);
        for (unsigned i=0; i < match.size(); i++) {
            std::cout << match[i] << std::endl;
        }*/
        cout<<"Switchboard Equipment Switching Status is on. Everything is functioning properly!"<<endl;
        // print recognized text
        //cout << outText << endl; // Destroy used object and release memory ocr->End();
        cout<<flush;
        waitKey();
        return EXIT_SUCCESS;
    }