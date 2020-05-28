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
	for (int i = 0; i < 3; ++i) {
		retriver.getPrecision(retriver.queries[0], (PicRetriver::DistanceMethod)i, 16, "Test/");
	}

	for (int i = 0; i < 3; ++i) {
		retriver.getPrecision(retriver.queries[0], (PicRetriver::DistanceMethod)i, 128, "Test/");
	}
	
	// wait for user
	// waitKey(0);
	return 0;
}