
/*
Some instructions can be found here:
https://www.deciphertechnic.com/install-opencv-with-visual-studio/

Additionally copy the necessary .dll file to the source directory (where the exe file is created)
*/

/*
This is the official opencv page, and the tutorial paragraphs are looking promising, it even mentions GPU acceleration with cuda.
https://docs.opencv.org/master/d9/df8/tutorial_root.html
*/

#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/hal/hal.hpp> // needed for filter2D
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include<iostream>

#include "functions/functions.h"

//using namespace std;
//using namespace cv;

int main()
{
//	cv::Mat img = cv::imread("D:/R_codes/2019/Clicker/screenshot_test/generated_by_R.png");
//	cv::namedWindow("image", cv::WINDOW_NORMAL);
//	cv::imshow("image", img);




//	// cv::Mat::Mat Constructor
//	cv::Mat M(2, 2, CV_8UC3, cv::Scalar(0, 0, 255));
//	std::cout << "M = " << std::endl << " " << M << std::endl << std::endl;
//
//	// For small matrices you may use comma separated initializers or initializer lists (C++11 support is required in the last case):
//	cv::Mat C = (cv::Mat_<double>(3, 3) << 0, 0, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f);
//	std::cout << "C = " << std::endl << " " << C << std::endl << std::endl;
//
//	// You can fill out a matrix with random values using the cv::randu() function. You need to give a lower and upper limit for the random values:
//	cv::Mat R = cv::Mat(50, 50, CV_8UC3);
//	cv::randu(R, cv::Scalar::all(0), cv::Scalar::all(255));
//
//	cv::namedWindow("2nd_window", cv::WINDOW_NORMAL);
//	cv::imshow("2nd_window", R);




//	// Applying a matrix on the image
//	cv::Mat img = cv::imread("D:/cpp_codes/30_OpenCV_revisit/00_Image_Samples/sample_image.png");
//	cv::namedWindow("image", cv::WINDOW_NORMAL);
//	cv::imshow("image", img);

//	cv::Mat blur_kernel = (cv::Mat_<float>(5, 5) << 0.04f, 0.04f, 0.04f, 0.04f, 0.04f,
//		0.04f, 0.04f, 0.04f, 0.04f, 0.04f,
//		0.04f, 0.04f, 0.04f, 0.04f, 0.04f,
//		0.04f, 0.04f, 0.04f, 0.04f, 0.04f,
//		0.04f, 0.04f, 0.04f, 0.04f, 0.04f);
//	cv::Mat blur_kernel = (cv::Mat_<float>(3, 3) << 0.11f, 0.11f, 0.11f,
//		0.11f, 0.11f, 0.11f,
//		0.11f, 0.11f, 0.11f);
//
//	cv::Mat blurred_img;
//	cv::filter2D(img, blurred_img, img.depth(), blur_kernel);
//	cv::namedWindow("blurred_image", cv::WINDOW_NORMAL);
//	cv::imshow("blurred_image", blurred_img);
//
//
//	cv::Mat sharpen_kernel = (cv::Mat_<char>(3, 3) << 0, -1, 0,
//											-1, 5, -1,
//											0, -1, 0);
//
//	cv::Mat sharpened_img;
//	cv::filter2D(img, sharpened_img, img.depth(), sharpen_kernel); // try to sharpen the original image
////	cv::filter2D(blurred_img, sharpened_img, blurred_img.depth(), sharpen_kernel); // try to sharpen back the blurred image - doesnt work very well
//	cv::namedWindow("sharpened_image", cv::WINDOW_NORMAL);
//	cv::imshow("sharpened_image", sharpened_img);

//	cv::Mat edges_img = DetectAllEdges(img);
//	cv::namedWindow("edge_detected_img", cv::WINDOW_NORMAL);
//	cv::imshow("edge_detected_img", edges_img);


	cv::Mat cookieSample = cv::imread("D:/cpp_codes/30_OpenCV_revisit/00_Image_Samples/wrath_cookie.png", cv::IMREAD_GRAYSCALE); //
	cv::Mat noCookie = cv::imread("D:/cpp_codes/30_OpenCV_revisit/00_Image_Samples/no_cookie.png", cv::IMREAD_GRAYSCALE);
	cv::Mat yesCookie = cv::imread("D:/cpp_codes/30_OpenCV_revisit/00_Image_Samples/wrath_cookie_present.png", cv::IMREAD_GRAYSCALE);

	cv::Mat_<float> cookieSample_f, noCookie_f, yesCookie_f;
	cookieSample.convertTo(cookieSample_f, CV_32F);
	noCookie.convertTo(noCookie_f, CV_32F);
	yesCookie.convertTo(yesCookie_f, CV_32F);

	float normalization = 1.0f/cv::sum(cookieSample_f)[0];

	cv::Mat noCookieSearch, yesCookieSearch;
	cv::filter2D(noCookie_f, noCookieSearch, noCookie_f.depth(), normalization*cookieSample_f);
	cv::filter2D(yesCookie_f, yesCookieSearch, yesCookie_f.depth(), normalization*cookieSample_f);

	cv::namedWindow("noCookieSearched", cv::WINDOW_NORMAL);
	cv::imshow("noCookieSearched", noCookieSearch);
	cv::namedWindow("noCookie", cv::WINDOW_NORMAL);
	cv::imshow("noCookie", noCookie);

	cv::namedWindow("yesCookieSearched", cv::WINDOW_NORMAL);
	cv::imshow("yesCookieSearched", yesCookieSearch);
	cv::namedWindow("yesCookie", cv::WINDOW_NORMAL);
	cv::imshow("yesCookie", yesCookie);


	//***** Blending *****//

	float alpha = 0.6f;
	cv::Mat blended;
	cv::addWeighted(noCookie, alpha, yesCookie, 1.0f-alpha, 0.0, blended); // only works with same sized images

	cv::namedWindow("blend", cv::WINDOW_NORMAL);
	cv::imshow("blend", blended);




	cv::waitKey(0);
	return 0;
}

