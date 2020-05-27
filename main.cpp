#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include <cmath>
#include "PicInfo.h"
#include "PicPool.h"
using namespace std;
using namespace cv;

enum DistanceMethod {
	LTWO,
	HI,
	BH
};

#define BIN_16 (16)
#define BIN_128 (128)

double m_L2(int *histP, int *histQ, int bins){
	if(bins != 16 && bins != 128){
		cerr << "L2 Error: bins = " << bins << ",but should be 16 or 128" << endl;
		return -1;
	} else{
		try{
			double sum = 0;
			for(int i = 0;i < bins; ++i){
				sum += (histP[i] - histQ[i]) * (histP[i] - histQ[i]);
			}
			return sqrt(sum);
		} catch(range_error){
			cerr << "L2 Error: Try to read element out of range" << endl;
			return -1;
		} catch(exception e){
			cerr << "L2 Error: Error occurs in L2: " << e.what() << endl;
			return -2;
		}
	}
}

double m_HI(int *histP, int *histQ, int bins){
	if(bins != 16 && bins != 128){
		cerr << "HI Error: bins = " << bins << ",but should be 16 or 128" << endl;
		return -1;
	} else{
		try{
			double sumMin = 0, sumQ = 0;
			for(int i = 0;i < bins; ++i){
				sumMin += min(histP[i], histQ[i]);
				sumQ += histQ[i];
			}
			return sumMin / sumQ;
		} catch(range_error e){
			cerr << "HI Error: Try to read element out of range." << endl;
			return -1;
		} catch(exception e){
			cerr << "HI Error: Error occurs in HI: " << e.what() << endl;
			return -2;
		}
	}
}
/*
input percentile histogram
*/
double m_Bh(double *histPd, double *histQd, int bins){
	if(bins != 16 && bins != 128){
		cerr << "Bh Error: bins = " << bins << ",but should be 16 or 128" << endl;
		return -1;
	} else{
		try{
			double sum = 0;
			for(int i = 0;i < bins; ++i){
				sum += sqrt(histPd[i] * histQd[i]);
			}
			return sqrt(1 - sum);
		} catch(range_error e){
			cerr << "Bh Error: Try to read element out of range." << endl;
			return -1;
		} catch(exception e){
			cerr << "Bh Error: Error occurs in Bh: " << e.what() << endl;
			return -2;
		}
	}
}
const char DATASET_RELA_DIR[] = "DataSet/";
int main(){
	// Read images info from AllImage.txt
	PicPool picPool(DATASET_RELA_DIR);
	picPool.loadAllImageInfo("AllImages.txt");
	cv::imshow("Find", picPool.findPic("arborgreens/Image01.jpg")->image);
	//// Read query info from QueryImages.txt
	//fin.open("QueryImages.txt");
	//vector<string> querys;
	//int tmpNum;
	//while (!fin.eof()) {
	//	string queryName;
	//	fin >> queryName >> tmpNum >> tmpNum;
	//	querys.push_back(queryName);
	//}

	//// test to calculate Histogram of an BGR image
	//Mat testImg = imread(DATASET_RELA_DIR + datas[0]->fileName);
	//if (testImg.empty()) {
	//	cerr << "fail to load image from " << datas[0]->fileName << endl;
	//	return -1;
	//}
	//
	//int histP[16] = { 0 }, histQ[16] = { 0 };
	//int bins = 16;

	//// test of three different distance functions
	//Mat Q = imread(DATASET_RELA_DIR + datas[1]->fileName);
	//if(Q.empty()){
	//	cerr << "fail to load image from " << datas[1]->fileName << endl;
	//}
	//for (int i = 0; i < Q.rows; ++i) {
	//	for (int j = 0; j < Q.cols; ++j) {
	//		Pixel& pixel = Q.at<Pixel>(i, j);
	//		int b = 0, g = 0, r = 0;
	//		b = pixel.x / 256.0 * 2;
	//		g = pixel.y / 256.0 * 4;
	//		r = pixel.z / 256.0 * 2;
	//		histQ[b * 8 + g * 2 + r]++;
	//	}
	//}
	//
	//double l2 = m_L2(histP, histQ, bins);
	//double hi = m_HI(histP, histQ, bins);
	//double histPd[16] = {0}, histQd[16] = {0};
	//double totalNum = testImg.rows * testImg.cols;
	//for(int i = 0;i < bins; ++i){
	//	histPd[i] = histP[i] / totalNum;
	//	histQd[i] = histQ[i] / totalNum;
	//}
	//double bh = m_Bh(histPd, histQd, bins);
	//printf("bins = 16\n");
	//printf("l2: %lf, hi: %lf, bn: %lf\n", l2, hi, bh);
	//
	//

	//for (int i = 0; i < Q.rows; ++i) {
	//	for (int j = 0; j < Q.cols; ++j) {
	//		Pixel& pixel = Q.at<Pixel>(i, j);
	//		int b = 0, g = 0, r = 0;
	//		b = pixel.x / 256.0 * 4;
	//		g = pixel.y / 256.0 * 8;
	//		r = pixel.z / 256.0 * 4;
	//		histQl[b * 32 + g * 4 + r]++;
	//	}
	//}
	//
	//l2 = m_L2(histPl, histQl, bins);
	//hi = m_HI(histPl, histQl, bins);
	//double histPdd[128] = {0}, histQdd[128] = {0};
	//for(int i = 0;i < bins; ++i){
	//	histPdd[i] = histPl[i] / totalNum;
	//	histQdd[i] = histQl[i] / totalNum;
	//}
	//bh = m_Bh(histPdd, histQdd, bins);

	//printf("bins = 128\n");
	//printf("l2: %lf, hi: %lf, bn: %lf\n", l2, hi, bh);
	
	// wait for user
	waitKey(0);
	return 0;
}