#pragma once
#include "PicInfo.h"
#include "PicPool.h"
#include "Query.h"
class PicRetriever {
public:
	PicRetriever(const char* dataset_rela_dir): m_pool(dataset_rela_dir){}
	int loadPicPool(std::string allImages) {
		return m_pool.loadAllImageInfo(allImages);
	}
	int loadQueries(std::string queryImages);
	~PicRetriever(){
		while (queries.size() > 0) {
			delete queries.back();
			queries.pop_back();
		}
	}

	enum DistanceMethod {
		LTWO,
		HI,
		BH
	};
	double retrieve(Query* query, DistanceMethod method, int bins, std::string storeDirName);
	double retriveAll(DistanceMethod method, int bins, std::string storeDirName);
	int dumpQueries(std::string dirName);
private:
	static double m_L2(const int* histP,const int* histQ, int bins);
	static double m_HI(const int *histP,const int *histQ, int bins);
	static double m_Bh(const double* histPd,const double* histQd, int bins);
	//static double m_cosine(const int* histP, const int* histQ, int bins);
	//static double 
	bool isCorrect(std::string p, std::string q);
	PicPool m_pool;
	std::vector<Query*> queries;
};