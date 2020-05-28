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
		if (!fin.is_open()) {
			std::cerr << "Fail to open " << queryImages << std::endl;
			return -1;
		}
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
				queries.push_back(tmpQuery);
			}
			else {
				std::cerr << "Query: " << queryName << " is not valid (should be picture in the dataset" << std::endl;
				fin.close();
				return -1;
			}
		}
		std::cout << "Loading queries finished" << std::endl;
		fin.close();
		return queries.size();
	}
	~PicRetriver(){
		while (queries.size() > 0) {
			delete queries.back();
			queries.pop_back();
		}
	}
	//void getAllPrecision();
	enum DistanceMethod {
		LTWO,
		HI,
		BH
	};
	void getPrecision(Query* query,DistanceMethod method, int bins, std::string storeDirName);
	std::vector<Query*> queries; // test only, should be private
	PicPool m_pool; // debug only, should be private
private:
	static double m_L2(const int* histP,const int* histQ, int bins);
	static double m_HI(const int *histP,const int *histQ, int bins);
	static double m_Bh(const double* histPd,const double* histQd, int bins);
	bool isCorrect(std::string p, std::string q);
};