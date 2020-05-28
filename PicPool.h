#pragma once
#include "PicInfo.h"
#include <vector>
#include <fstream>
typedef cv::Point3_<uint8_t> Pixel;

class PicPool {
public:
	PicPool(const char* dataset_rela_dir) {
		DATASET_RELA_DIR = dataset_rela_dir;
	}
	
	/*
	Load PicInfo from the given file directory
	@param allImages: The relative path to "AllImages.txt"
	@return The number of lines of picture information, -1 means error in opening AllImage.txt. ,-2 means error in loading image
	*/
	int loadAllImageInfo(std::string allImages) {
		std::ifstream fin(allImages);
		if (!fin.is_open()) {
			std::cerr << "Fail to open " << allImages << std::endl;
			return -1;
		}
		PicInfo* tmpData = NULL;
		std::cout << "Loading images from " << allImages << "..." << std::endl;
		while (!fin.eof()) {
			// read lines
			tmpData = new PicInfo();
			fin >> tmpData->fileName >> tmpData->width >> tmpData->height;
			if (tmpData->fileName.size() == 0) {
				delete tmpData;
				break;
			}
			// load image
			tmpData->image = cv::imread(DATASET_RELA_DIR + tmpData->fileName);
			if (tmpData->image.empty()) {
				std::cerr << "Fail to load image from " << DATASET_RELA_DIR + tmpData->fileName << std::endl;
				fin.close();
				return -2;
			}
			// calculate hist
			calcImgHists(tmpData);
			// store data
			pool.push_back(tmpData);
		}
		fin.close();
		std::cout << "Loading finished." << std::endl;
		return pool.size();
	}

	PicInfo* findPic(std::string fileName) {
		// TODO: Try to change the data structure to increase speed of searching
		// Normal iteration with time complexity O(n*m) where n is |pool|, m is |filename|
		for (auto iter = pool.begin(); iter != pool.end(); ++iter) {
			if ((*iter)->fileName == fileName) {
				return (*iter);
			}
		}
		return NULL;
	}

	bool isLoaded() {
		return pool.size() > 0;
	}

	~PicPool(){
		// release image info resource
		while (pool.size() > 0) {
			delete pool.back();
			pool.pop_back();
		}
	}
private:
	void calcImgHists(PicInfo* info) {
		/// This is slow but it works
		// TODO: Try to find a faster way to do the same thing
		int b = 0, g = 0, r = 0;
		for (int i = 0; i < info->height; ++i) {
			for (int j = 0; j < info->width; ++j) {
				Pixel& pixel = info->image.at<Pixel>(i, j);
				// bins = 16
				b = pixel.x / 256.0 * 2;
				g = pixel.y / 256.0 * 4;
				r = pixel.z / 256.0 * 2;
				info->hist16[b * 8 + g * 2 + r]++;
				//bins = 128
				b = pixel.x / 256.0 * 4;
				g = pixel.y / 256.0 * 8;
				r = pixel.z / 256.0 * 4;
				info->hist128[b * 32 + g * 4 + r]++;
			}
		}
	}
	std::vector<PicInfo*> pool;
	const char *DATASET_RELA_DIR;
};