#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include <cmath>
#include "PicInfo.h"
#include "PicPool.h"
#include "PicRetriver.h"
using namespace std;
using namespace cv;

const char DATASET_RELA_DIR[] = "DataSet/";
int main(){
	
	PicRetriver retriver(DATASET_RELA_DIR);
	// Read images info from AllImage.txt
	retriver.loadPicPool("AllImages.txt");
	// Read query info from QueryImages.txt
	retriver.loadQueries("QueryImages.txt");
	/*for (int i = 0; i < 3; ++i) {
		retriver.getPrecision(retriver.queries[0], (PicRetriver::DistanceMethod)i, 16, "Test/");
	}

	for (int i = 0; i < 3; ++i) {
		retriver.getPrecision(retriver.queries[0], (PicRetriver::DistanceMethod)i, 128, "Test/");
	}*/
	Mat hist;
	Mat testImg = retriver.m_pool[0]->image.clone();
	const int* hist16 = retriver.m_pool[0]->hist16;
	int channels[] = { 0,1,2 };
	float range[] = { 0.0f, 256.0f };
	const float* ranges[] = { range, range, range };
	const int histSize[] = { 2,4,2 };

	calcHist(&testImg, 1, channels,Mat(), hist, 3, histSize, ranges);

	cout << hist.step[0] << " " << hist.step[1] << " " << hist.step[2] << endl;
	for (int i = 0; i < hist.size[0]; ++i) {
		for (int j = 0; j < hist.size[1]; ++j) {
			for (int k = 0; k < hist.size[2]; ++k) {
				cout << hist.at<float>(i, j, k) << " ";
			}
		}
	}
	cout << endl;
	for (int i = 0; i < 16; ++i) {
		cout << hist16[i] << " ";
	}
	// wait for user
	// waitKey(0);
	return 0;
}