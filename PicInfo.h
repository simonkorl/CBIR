#pragma once
#include <iostream>
#include <opencv.hpp>
struct PicInfo {
	std::string fileName;
	int width = 0;
	int height = 0;
	cv::Mat image;
	int hist16[16] = { 0 };
	int hist128[128] = { 0 };
};