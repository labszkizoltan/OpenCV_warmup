
#include "functions.h"


cv::Mat DetectAllEdges(const cv::Mat& img)
{
	cv::Mat edge_horizontal_kernel = (cv::Mat_<char>(3, 3) << 1, 1, 1,
		0, 0, 0,
		-1, -1, -1);

	cv::Mat edge_vertical_kernel = (cv::Mat_<char>(3, 3) << -1, 0, 1,
		-1, 0, 1,
		-1, 0, 1);

	cv::Mat edge_horizontal_p_img, edge_horizontal_m_img, edge_vertical_p_img, edge_vertical_m_img;
	cv::filter2D(img, edge_horizontal_p_img, img.depth(), edge_horizontal_kernel);
	cv::filter2D(img, edge_horizontal_m_img, img.depth(), -edge_horizontal_kernel);
	cv::filter2D(img, edge_vertical_p_img, img.depth(), edge_vertical_kernel);
	cv::filter2D(img, edge_vertical_m_img, img.depth(), -edge_vertical_kernel);

	cv::Mat edges_img = edge_horizontal_p_img / 4 + edge_horizontal_m_img / 4 + edge_vertical_p_img / 4 + edge_vertical_m_img / 4;

	return edges_img;
}



