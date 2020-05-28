#pragma once
#include "PicInfo.h"
struct Query {
	const PicInfo* picInfo = NULL;
	double precision = 0;
	const PicInfo* results[30] = { 0 };
};