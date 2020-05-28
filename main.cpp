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
	// Create PicRetriver with DataSet directory
	PicRetriver retriver(DATASET_RELA_DIR);
	// Read images info from AllImage.txt
	retriver.loadPicPool("AllImages.txt");
	// Read query info from QueryImages.txt
	retriver.loadQueries("QueryImages.txt");
	vector<double> preciResults;
	// Do all combinations queries
	for (int i = 0; i < 6; ++i) {
		string dirName = "Result/" + to_string(i) + "/";
		if (i < 3) {
			// use BIN16
			preciResults.push_back(retriver.retriveAll((PicRetriver::DistanceMethod)i, BIN_16, dirName));
			retriver.dumpQueries(dirName);
		}
		else {
			// use BIN128
			preciResults.push_back(retriver.retriveAll((PicRetriver::DistanceMethod)(i - 3), BIN_128, dirName));
			retriver.dumpQueries(dirName);
		}
	}
	
	return 0;
}