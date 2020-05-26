#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
using namespace cv;
typedef Point3_<uint8_t> Pixel;
struct PicData {
	string fileName;
	int width = 0;
	int height = 0;
};
const char DATASET_RELA_DIR[] = "DataSet/";
int main(){
	// Read images info from AllImage.txt
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
		cout << "testImg has not been loaded" << endl;
		return -1;
	}
	
	int bin[16] = { 0 }, bin2[16] = { 0 };
	int tmpCount = 0;
	// This iteration seemed to be slower, but it guarantees 
	// the total count of pixels
	for (int i = 0; i < testImg.rows; ++i) {
		for (int j = 0; j < testImg.cols; ++j) {
			Pixel& pixel = testImg.at<Pixel>(i, j);
			int b = 0, g = 0, r = 0;
			b = pixel.x / 256.0 * 2;
			g = pixel.y / 256.0 * 4;
			r = pixel.z / 256.0 * 2;
			bin[b * 8 + g * 2 + r]++;
		}
	}
	// It is weird that forEach cannot iterate the whole image
	// The total count in bin2 is not equal to that in bin
	testImg.forEach<Pixel>(
		[&bin2, &tmpCount](Pixel & pixel, const int* position) -> void{
			int b = 0, g = 0, r = 0;
			b = pixel.x / 256.0 * 2;
			g = pixel.y / 256.0 * 4;
			r = pixel.z / 256.0 * 2;
			bin2[b * 8 + g * 2 + r]++;
		}
	);
	int tmpSum = 0, tmpSum2 = 0;
	for (int i = 0; i < 16; ++i) {
		printf("bin[%d]: %d \t bin2[%d]: %d\n", i, bin[i],i, bin2[i]);
		tmpSum += bin[i];
		tmpSum2 += bin2[i];
	}
	printf("tmpSum: %d, tmpSum2: %d, width * height: %d", tmpSum , tmpSum2, (datas[0]->width * datas[0]->height));
	// wait for user
	// waitKey(0);

	// release image info resource
	while (datas.size() > 0) {
		delete datas.back();
		datas.pop_back();
	}
	return 0;
}