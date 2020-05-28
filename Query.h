#pragma once
#include "PicInfo.h"
struct Query {
	const PicInfo* picInfo = NULL;
	double precision = 0;
	std::vector<std::pair<const PicInfo*, double>> results;
	void clear() {
		precision = 0;
		results.clear();
	}
};