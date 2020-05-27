#pragma once
#include "PicInfo.h"
#include "PicPool.h"
#include "Query.h"
class PicRetriver {
public:
	PicRetriver(const char* dataset_rela_dir): m_pool(dataset_rela_dir){}
	int loadPicPool(std::string allImages) {
		return m_pool.loadAllImageInfo(allImages);
	}
	int loadQueries(std::string queryImages) {
		if (!m_pool.isLoaded()) return -1;
		std::ifstream fin(queryImages);
		fin.open("QueryImages.txt");
		PicInfo* tmpInfo = NULL;
		Query* tmpQuery = NULL;
		int junk = 0;
		std::cout << "Loading queries from " << queryImages << std::endl;
		while (!fin.eof()) {
			std::string queryName;
			fin >> queryName >> junk >> junk;
			if (queryName.size() == 0) break;
			if ((tmpInfo = m_pool.findPic(queryName)) != NULL) {
				tmpQuery = new Query();
				tmpQuery->picInfo = tmpInfo;
			}
			else {
				std::cerr << "Query: " << queryName << " is not valid (should be picture in the dataset" << std::endl;
				return -1;
			}
		}
		std::cout << "Loading queries finished" << std::endl;
		return queries.size();
	}
	~PicRetriver(){
		while (queries.size() > 0) {
			delete queries.back();
			queries.pop_back();
		}
	}
	//void getAllPrecision();
	//void getPrecision(Query* query, DistanceMethod method, int bins, std::string storeDirName);
private:
	enum DistanceMethod {
		LTWO,
		HI,
		BH
	};
	//double m_L2(int* histP, int* histQ, int bins);
	//double m_HI(int *histP, int *histQ, int bins);
	//double m_Bh(double *histPd, double *histQd, int bins);
	//bool isCorrect(PicInfo* p, PicInfo* q);
	PicPool m_pool;
	std::vector<Query*> queries;
};