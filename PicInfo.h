#pragma once
#include <iostream>
#include <opencv.hpp>
#define BIN_16 (16)
#define BIN_128 (128)
struct PicInfo {
	std::string fileName;
	int width = 0;
	int height = 0;
	cv::Mat image;
	int hist16[BIN_16] = { 0 };
	int hist128[BIN_128] = { 0 };
};