#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <regex>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    char *outText;
    string imPath;
    if(argc>1){
        imPath=argv[1];
    }else{
        imPath="/Users/aaple/Desktop/TextT/T1.jpg";
    }

    // Create Tesseract object
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();

    // Initialize tesseract to use chinese (sim) and the LSTM OCR engine.
    ocr->Init(NULL, "chi_sim", tesseract::OEM_LSTM_ONLY);

    // Set Page segmentation mode to PSM_AUTO (3)
    ocr->SetPageSegMode(tesseract::PSM_AUTO);

    // Open input image using OpenCV
    Mat im = cv::imread(imPath, IMREAD_COLOR);
    Mat img = cv::imread(imPath, IMREAD_GRAYSCALE);

    //vector<Rect2d> rects;
    bool fromCenter=false;
    Rect2d r=selectROI("Gray",img,fromCenter);
    Mat imCrop=im(r);
    imshow("Crop", imCrop);
    waitKey();
    //cv::resize(im,im,Size(720,540),INTER_AREA);
    //imshow("scaled gray",im);
    //waitKey();
    //Mat dst;
    //threshold(imCrop,dst,100,255,THRESH_BINARY);
    //adaptiveThreshold(im,dst,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,11,2);
    //imshow("BinIm",dst);
    //waitKey();
    //GaussianBlur(dst,dst,Size(9,9),2,2);
    //imshow("G",dst);
    //waitKey();
    //erode(dst,dst,Mat());
    //imshow("e",dst);
    //waitKey();


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

    // print recognized text
    //cout << outText << endl; // Destroy used object and release memory ocr->End();
    cout<<flush;
    waitKey();
    return EXIT_SUCCESS;
}
