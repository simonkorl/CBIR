#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(){
	//��ȡ���ص�һ��ͼƬ����ʾ����
	Mat img = imread("DataSet/arborgreens/Image01.jpg");
	imshow("Test", img);
	cout << "size: " << img.size << endl;
	//�ȴ��û�����
	waitKey(0);
	return 0;
}