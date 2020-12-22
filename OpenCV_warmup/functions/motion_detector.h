#pragma once

#include<opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/hal/hal.hpp> // needed for filter2D
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


class MotionDetector
{
public:
	MotionDetector();
	MotionDetector(int device_id);
	~MotionDetector();

	void Update();
	void SearchForMovement();

	void DrawCurrent(const std::string& window_name);
	void DrawPrevious(const std::string& window_name);
	void GetCurrent();
	void GetPrevious();


private:
	void Init();


private:
	cv::VideoCapture m_Camera;

	cv::Mat m_Current_Frame, m_Prev_Frame;
	cv::Mat m_Current_Frame_gscl, m_Prev_Frame_gscl; // grayscale images
	cv::Mat m_Diff, m_Threshold_img;

	int m_X, m_Y; // coordinates of the detected object

};
















