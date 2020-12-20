
/*

code was started by this tutorial:
https://www.youtube.com/watch?v=y6n3rQbMYmc&list=PLAp0ZhYvW6XbEveYeefGSuLhaPlFML9gP&index=13

and was continued by this:
https://www.youtube.com/watch?v=X6rPdRZzgjg

*/



#include<opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/hal/hal.hpp> // needed for filter2D
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include<iostream>
#include<vector>
#include<cstdint>

const int g_fps = 20;
const int g_sensitivity_value = 20;
const int g_blur_size = 10;
cv::Rect g_object_bounding_rectangle = cv::Rect(0,0,0,0);
int g_the_object[2] = {0,0};
bool g_obj_detected = true;

std::string intToString(int number) {
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void SearchForMovement(bool& objDetected, cv::Mat thresholdImg, cv::Mat& cameraFeed)
{
	cv::Mat temp;
	thresholdImg.copyTo(temp);

	// these vectors are needed for the output of findContours
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	// find countours of filtered image using OpenCV findCountours function
	cv::findContours(temp, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

//	if (contours.size() > 0) { objDetected = true; }
	objDetected = (contours.size() > 0);

	if (objDetected)
	{
		// the largest contour is found at the end of teh contours vector
		// we will simply assume that the biggest contour is the object we are looking for
		std::vector<std::vector<cv::Point>> largestContourVec;
		largestContourVec.push_back(contours.at(contours.size()-1));

		// make a bounding rectangle around the largest contour then find its centroid
		// this will be the object's final position
		g_object_bounding_rectangle = cv::boundingRect(largestContourVec.at(0));
		int xpos = g_object_bounding_rectangle.x + g_object_bounding_rectangle.width / 2;
		int ypos = g_object_bounding_rectangle.y + g_object_bounding_rectangle.width / 2;

		// update the objects position by changing the values in g_the_object
		g_the_object[0] = xpos, g_the_object[1] = ypos;
	}

	// make some temporary variables so dont have to type as much
	int x = g_the_object[0];
	int y = g_the_object[1];

	// draw some crosshairs on the object:
	cv::circle(cameraFeed, cv::Point(x,y), 20, cv::Scalar(0, 255, 0), 2);
	cv::line(cameraFeed, cv::Point(x-25, y), cv::Point(x+25, y), cv::Scalar(0, 255, 0), 2);
	cv::line(cameraFeed, cv::Point(x, y-25), cv::Point(x, y+25), cv::Scalar(0, 255, 0), 2);
	cv::putText(cameraFeed, "Tracking object at (" + intToString(x) + ", " + intToString(y) + ")", cv::Point(x,y), 1, 1, cv::Scalar(255, 0, 0));
}


int main()
{
	bool is_running = true;
	cv::Mat current_frame, prev_frame;
	cv::Mat current_frame_gscl, prev_frame_gscl; // grayscale images
	cv::Mat diff, threshold_img, smooth_threshold_img;

	cv::VideoCapture vid(0);
	if (!vid.isOpened()) { return -1; }

	vid.read(current_frame);
	current_frame.copyTo(prev_frame);

	while (is_running)
	{
		current_frame.copyTo(prev_frame);
		vid.read(current_frame);

		cv::cvtColor(current_frame, current_frame_gscl, cv::COLOR_BGR2GRAY);
		cv::cvtColor(prev_frame, prev_frame_gscl, cv::COLOR_BGR2GRAY);

		cv::absdiff(current_frame_gscl, prev_frame_gscl, diff); // this function works with colorful images as well
		cv::threshold(diff, threshold_img, g_sensitivity_value, 255, cv::THRESH_BINARY);

//		// Smooth the threshold image, to reduce noise, and threshold it again (seems to work, just very slow in debug mode, however it is still okay in release mode)
//		cv::filter2D(threshold_img, smooth_threshold_img, threshold_img.depth(), blur_kernel);
//		cv::threshold(smooth_threshold_img, threshold_img, g_sensitivity_value, 255, cv::THRESH_BINARY);

		cv::blur(threshold_img, smooth_threshold_img, cv::Size(g_blur_size, g_blur_size));
		cv::threshold(smooth_threshold_img, threshold_img, g_sensitivity_value, 255, cv::THRESH_BINARY);

		// when I used current_frame, to put the markers on, the marker was moving, and generated a detectable movement of the marker circle
		SearchForMovement(g_obj_detected, threshold_img, prev_frame);



		cv::imshow("diff", diff);
		cv::imshow("threshold_img", threshold_img);
//		cv::imshow("smooth_threshold_img", smooth_threshold_img);
		cv::imshow("current_img", current_frame);
		cv::imshow("prev_img", prev_frame);

//		if (cv::waitKey(1000 / g_fps) >= 0) break;


		if (cv::waitKey(1000/g_fps) == 27)
		{
			is_running = false;
			cv::destroyWindow("diff");
			cv::destroyWindow("threshold_img");
			cv::destroyWindow("current_img");
			cv::destroyWindow("prev_img");
		} // if esc key is pressed, exit loop

	}

	return 0;
}

