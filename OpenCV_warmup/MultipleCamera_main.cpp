
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

#include "functions/motion_detector.h"

#include "global_constants.h"


int main()
{
	bool is_running = true; // variable indicating if the main loop should be running or not

	MotionDetector detector_1(0), detector_2(1);

	while (is_running)
	{

		detector_1.Update();
		detector_1.SearchForMovement();
		detector_1.DrawPrevious("Webcam");

		detector_2.Update();
		detector_2.SearchForMovement();
		detector_2.DrawPrevious("CellphoneCamera");

		if (cv::waitKey(1000 / g_fps) == 27)
		{
			is_running = false;
//			cv::destroyWindow("diff");
		} // if esc key is pressed, exit loop
	}

	return 0;
}

