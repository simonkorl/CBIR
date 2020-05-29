#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include <cmath>
#include "PicInfo.h"
#include "PicPool.h"
#include "PicRetriever.h"
using namespace std;
using namespace cv;

const char DATASET_RELA_DIR[] = "DataSet/";
int main(){
	// Create PicRetriver with DataSet directory
	PicRetriever retriever(DATASET_RELA_DIR);
	// Read images info from AllImage.txt
	retriever.loadPicPool("AllImages.txt");
	// Read query info from QueryImages.txt
	retriever.loadQueries("QueryImages.txt");
	vector<double> preciResults;
	// Do all combinations queries
	for (int i = 0; i < 10; ++i) {
		string dirName = "Result/" + to_string(i) + "/";
		if (i < 5) {
			// use BIN16
			preciResults.push_back(retriever.retrieveAll((PicRetriever::DistanceMethod)i, BIN_16));
			retriever.dumpAllQueries(dirName, dirName);
		}
		else {
			// use BIN128
			preciResults.push_back(retriever.retrieveAll((PicRetriever::DistanceMethod)(i - 5), BIN_128));
			retriever.dumpAllQueries(dirName, dirName);
		}
	}
	return 0;
}