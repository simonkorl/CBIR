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
		BH,
		COS,
		JM
	};
	/*
		@param storeDirName : should end with "/"
	*/
	double retrieve(Query* query, DistanceMethod method, int bins);
	/*
		@param storeDirName : should end with "/"
	*/
	double retrieveAll(DistanceMethod method, int bins);
	/*
		Output a query result in {dirName}
	*/
	int dumpQuery(Query* query, std::string dirName);
	/*
		Output all query results and "res_overall.txt" in {dirName}
		@param dirName: should end with "/"
		@return the number of dumped queries or error code
	*/
	int dumpAllQueries(std::string queryDirName, std::string overallDirName);
	// debug only
	Query* getTestQuery() {
		return queries[0];
	}
private:
	static double m_L2(const int* histP,const int* histQ, int bins);
	static double m_HI(const int *histP,const int *histQ, int bins);
	static double m_Bh(const double* histPd,const double* histQd, int bins);
	static double m_cosine(const int* histP, const int* histQ, int bins);
	// Jffreys & Matusita distance
	static double m_JM(const int* histP, const int* histQ, int bins);
	bool isCorrect(std::string p, std::string q);
	PicPool m_pool;
	std::vector<Query*> queries;
};