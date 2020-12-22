
#include "motion_detector.h"
#include "../global_constants.h"


// utility function used in SearchForMovement(...)
std::string intToString(int number) {
	std::stringstream ss;
	ss << number;
	return ss.str();
}



MotionDetector::MotionDetector()
	: m_Camera(0)
{
	if (!m_Camera.isOpened()) { std::cout << "Opening camera failed: " << 0 << "\n"; }
	Init();
}

MotionDetector::MotionDetector(int device_id)
	: m_Camera(device_id)
{
	if (!m_Camera.isOpened()) { std::cout << "Opening camera failed: " << device_id << "\n"; }
	Init();
}

MotionDetector::~MotionDetector()
{
}

void MotionDetector::Update()
{
	// move current frame to previous, then read the current again
	m_Current_Frame.copyTo(m_Prev_Frame);
	m_Camera.read(m_Current_Frame);

	// convert them to grayscale
	cv::cvtColor(m_Current_Frame, m_Current_Frame_gscl, cv::COLOR_BGR2GRAY);
	cv::cvtColor(m_Prev_Frame, m_Prev_Frame_gscl, cv::COLOR_BGR2GRAY);

	// take the diff and thresholds
	cv::absdiff(m_Current_Frame_gscl, m_Prev_Frame_gscl, m_Diff); // this function works with colorful images as well
	cv::threshold(m_Diff, m_Threshold_img, g_sensitivity_value, 255, cv::THRESH_BINARY);
	cv::blur(m_Threshold_img, m_Threshold_img, cv::Size(g_blur_size, g_blur_size));
	cv::threshold(m_Threshold_img, m_Threshold_img, g_sensitivity_value, 255, cv::THRESH_BINARY);
}


void MotionDetector::SearchForMovement()
{
	cv::Mat temp;
	m_Threshold_img.copyTo(temp);

	cv::Rect object_bounding_rectangle = cv::Rect(0, 0, 0, 0);

	// these vectors are needed for the output of findContours
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	// find countours of filtered image using OpenCV findCountours function
	cv::findContours(temp, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	if (contours.size() > 0)
	{
		// the largest contour is found at the end of the contours vector
		// we will simply assume that the biggest contour is the object we are looking for
		std::vector<std::vector<cv::Point>> largestContourVec;
		largestContourVec.push_back(contours.at(contours.size() - 1));

		// make a bounding rectangle around the largest contour then find its centroid
		// this will be the object's final position
		object_bounding_rectangle = cv::boundingRect(largestContourVec.at(0));
		int xpos = object_bounding_rectangle.x + object_bounding_rectangle.width / 2;
		int ypos = object_bounding_rectangle.y + object_bounding_rectangle.width / 2;

		// update the objects position by changing the member variables
		m_X = xpos, m_Y = ypos;
	}

	// draw some crosshairs on the object:
	cv::circle(m_Prev_Frame, cv::Point(m_X, m_Y), 20, cv::Scalar(0, 255, 0), 2);
	cv::line(m_Prev_Frame, cv::Point(m_X - 25, m_Y), cv::Point(m_X + 25, m_Y), cv::Scalar(0, 255, 0), 2);
	cv::line(m_Prev_Frame, cv::Point(m_X, m_Y - 25), cv::Point(m_X, m_Y + 25), cv::Scalar(0, 255, 0), 2);
	cv::putText(m_Prev_Frame, "Tracking object at (" + intToString(m_X) + ", " + intToString(m_Y) + ")", cv::Point(m_X, m_Y), 1, 1, cv::Scalar(255, 0, 0));
}



void MotionDetector::DrawCurrent(const std::string& window_name)
{
	cv::imshow(window_name, m_Current_Frame);
}

void MotionDetector::DrawPrevious(const std::string& window_name)
{
	cv::imshow(window_name, m_Prev_Frame);
}

void MotionDetector::GetCurrent()
{
}

void MotionDetector::GetPrevious()
{
}


void MotionDetector::Init()
{
//	read first image from the camera stream
	m_Camera.read(m_Current_Frame);
	m_Current_Frame.copyTo(m_Prev_Frame);

//	convert them to grayscale
	cv::cvtColor(m_Current_Frame, m_Current_Frame_gscl, cv::COLOR_BGR2GRAY);
	cv::cvtColor(m_Prev_Frame, m_Prev_Frame_gscl, cv::COLOR_BGR2GRAY);

//	Do the rest of the object tracking steps, just to fill out the so far blank Mat objects
	cv::absdiff(m_Current_Frame_gscl, m_Prev_Frame_gscl, m_Diff); // this function works with colorful images as well
	cv::threshold(m_Diff, m_Threshold_img, g_sensitivity_value, 255, cv::THRESH_BINARY);
//	cv::blur(m_Threshold_img, m_Threshold_img, cv::Size(g_blur_size, g_blur_size));
//	cv::threshold(m_Threshold_img, m_Threshold_img, g_sensitivity_value, 255, cv::THRESH_BINARY);
}






