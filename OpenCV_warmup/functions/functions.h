#pragma once

#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/hal/hal.hpp> // needed for filter2D
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


cv::Mat DetectAllEdges(const cv::Mat& img);

