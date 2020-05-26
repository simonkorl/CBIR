#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
using namespace cv;
struct PicData {
	string fileName;
	int width = 0;
	int height = 0;
};
int main(){
	// Read images info in AllImage.txt
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
	for (auto data = datas.begin(); data != datas.end(); ++data) {
		cout << (*data)->fileName << endl;
	}
	// Read query info in QueryImages.txt
	fin.close();
	fin.open("QueryImages.txt");
	vector<string> querys;
	int tmpNum;
	while (!fin.eof()) {
		string queryName;
		fin >> queryName >> tmpNum >> tmpNum;
		querys.push_back(queryName);
	}
	cout << "Querys:\n";
	for (auto query = querys.begin(); query != querys.end(); ++query) {
		cout << *query << endl;
	}
	// release image info resource
	while (datas.size() > 0) {
		delete datas.back();
		datas.pop_back();
	}
	////Load a local picture and display
	//Mat img = imread("DataSet/arborgreens/Image01.jpg");
	//imshow("Test", img);
	//cout << "size: " << img.size << endl;
	////wait for user
	//waitKey(0);
	return 0;
}