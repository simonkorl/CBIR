#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include <cmath>
using namespace std;
using namespace cv;
typedef Point3_<uint8_t> Pixel;
struct PicData {
	string fileName;
	int width = 0;
	int height = 0;
};
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
	// TODO: Try to change the data structure to increase speed of searching
	ifstream fin("AllImages.txt");
	if (!fin.is_open()) {
		cout << "Fail to open AllImages.txt" << endl;
		return -1;
	}
	string input;
	vector<PicData*> datas;
	PicData* tmpData = NULL;
	while (!fin.eof()) {
		tmpData = new PicData();
		fin >> tmpData->fileName >> tmpData->width >> tmpData->height;
		datas.push_back(tmpData);
	}

	// Read query info from QueryImages.txt
	fin.close();
	fin.open("QueryImages.txt");
	vector<string> querys;
	int tmpNum;
	while (!fin.eof()) {
		string queryName;
		fin >> queryName >> tmpNum >> tmpNum;
		querys.push_back(queryName);
	}

	// test to calculate Histogram of an BGR image
	Mat testImg = imread(DATASET_RELA_DIR + datas[0]->fileName);
	if (testImg.empty()) {
		cerr << "fail to load image from " << datas[0]->fileName << endl;
		return -1;
	}
	
	int histP[16] = { 0 }, histQ[16] = { 0 };
	int bins = 16;

	/// This is slow but it works
	// TODO: Try to find a faster way to do the same thing
	for (int i = 0; i < testImg.rows; ++i) {
		for (int j = 0; j < testImg.cols; ++j) {
			Pixel& pixel = testImg.at<Pixel>(i, j);
			int b = 0, g = 0, r = 0;
			b = pixel.x / 256.0 * 2;
			g = pixel.y / 256.0 * 4;
			r = pixel.z / 256.0 * 2;
			histP[b * 8 + g * 2 + r]++;
		}
	}

	// test of three different distance functions
	Mat Q = imread(DATASET_RELA_DIR + datas[1]->fileName);
	if(Q.empty()){
		cerr << "fail to load image from " << datas[1]->fileName << endl;
	}
	for (int i = 0; i < Q.rows; ++i) {
		for (int j = 0; j < Q.cols; ++j) {
			Pixel& pixel = Q.at<Pixel>(i, j);
			int b = 0, g = 0, r = 0;
			b = pixel.x / 256.0 * 2;
			g = pixel.y / 256.0 * 4;
			r = pixel.z / 256.0 * 2;
			histQ[b * 8 + g * 2 + r]++;
		}
	}
	
	double l2 = m_L2(histP, histQ, bins);
	double hi = m_HI(histP, histQ, bins);
	double histPd[16] = {0}, histQd[16] = {0};
	double totalNum = testImg.rows * testImg.cols;
	for(int i = 0;i < bins; ++i){
		histPd[i] = histP[i] / totalNum;
		histQd[i] = histQ[i] / totalNum;
	}
	double bh = m_Bh(histPd, histQd, bins);
	printf("bins = 16\n");
	printf("l2: %lf, hi: %lf, bn: %lf\n", l2, hi, bh);
	
	//test for bins = 128
	bins = 128;
	int histPl[128] = { 0 }, histQl[128] = { 0 };
	for (int i = 0; i < testImg.rows; ++i) {
		for (int j = 0; j < testImg.cols; ++j) {
			Pixel& pixel = testImg.at<Pixel>(i, j);
			int b = 0, g = 0, r = 0;
			b = pixel.x / 256.0 * 4;
			g = pixel.y / 256.0 * 8;
			r = pixel.z / 256.0 * 4;
			histPl[b * 32 + g * 4 + r]++;
		}
	}

	for (int i = 0; i < Q.rows; ++i) {
		for (int j = 0; j < Q.cols; ++j) {
			Pixel& pixel = Q.at<Pixel>(i, j);
			int b = 0, g = 0, r = 0;
			b = pixel.x / 256.0 * 4;
			g = pixel.y / 256.0 * 8;
			r = pixel.z / 256.0 * 4;
			histQl[b * 32 + g * 4 + r]++;
		}
	}
	
	l2 = m_L2(histPl, histQl, bins);
	hi = m_HI(histPl, histQl, bins);
	double histPdd[128] = {0}, histQdd[128] = {0};
	for(int i = 0;i < bins; ++i){
		histPdd[i] = histPl[i] / totalNum;
		histQdd[i] = histQl[i] / totalNum;
	}
	bh = m_Bh(histPdd, histQdd, bins);

	printf("bins = 128\n");
	printf("l2: %lf, hi: %lf, bn: %lf\n", l2, hi, bh);
	
	// wait for user
	// waitKey(0);

	// release image info resource
	while (datas.size() > 0) {
		delete datas.back();
		datas.pop_back();
	}
	return 0;
}