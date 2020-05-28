#include "PicRetriver.h"
#include <exception>
#include <string>
using namespace std;
#define WIDTH (378) 
#define HEIGHT (252)
#define TOTALNUM ((double)(WIDTH * HEIGHT))
void PicRetriver::getPrecision(Query* query, DistanceMethod method, int bins, std::string storeDirName){
	double (*preFunction) (const int*,const int*, int) = NULL;
	switch (method) {
	case LTWO:
		preFunction = PicRetriver::m_L2;
		break;
	case HI:
		preFunction = PicRetriver::m_HI;
		break;
	case BH:
		preFunction = [](const int* histP,const int* histQ, int bins)->double {
			double histPd[BIN_128] = { 0 }, histQd[BIN_128] = { 0 };
			for (int i = 0; i < bins; ++i) {
				histPd[i] = histP[i] / TOTALNUM;
				histQd[i] = histQ[i] / TOTALNUM;
			}
			return PicRetriver::m_Bh(histPd, histQd, bins);
		};
		break;
	default:
		cerr << "method error: " << method << endl;
		return;
	}
	// insert sort
	double tmpPreci = 0;
	query->clear();
	auto iter = query->results.begin();
	for (int i = 0; i < m_pool.size(); ++i) {
		tmpPreci = preFunction((bins == 16) ? query->picInfo->hist16 : query->picInfo->hist128, 
			(bins == 16) ? m_pool[i]->hist16 : m_pool[i]->hist128, bins);
		for (iter = query->results.begin(); iter != query->results.end(); ++iter) {
			if (iter->second > tmpPreci) {
				break;
			}
		}
		query->results.insert(iter, std::pair<const PicInfo*, double>(m_pool[i], tmpPreci));
		if (query->results.size() > 30) {
			query->results.pop_back();
		}
	}
	// output
	tmpPreci = 0;
	string outputPath = "res_" + query->picInfo->fileName;
	outputPath.replace(outputPath.find("/"), 1, "_");
	outputPath.replace(outputPath.find("jpg"), 3, "txt");
	outputPath = storeDirName + outputPath;
	ofstream fout(outputPath);
	if (!fout.is_open()) {
		cerr << "In getPrecision: Fail to open " << outputPath << endl;
		return;
	}
	for (int i = 0; i < 30; ++i) {
		fout << query->results[i].first->fileName << " " << query->results[i].second << endl;
		cout << query->results[i].first->fileName << " " << query->results[i].second << endl;
		if (isCorrect(query->results[i].first->fileName, query->picInfo->fileName)) {
			tmpPreci++;
		}
	}
	query->precision = tmpPreci / 30.0;
	cout << "precision: " << query->precision << endl << endl; // debug
	fout.close();
}
bool PicRetriver::isCorrect(std::string p, std::string q)
{
	return p.substr(0, p.find('/')) == q.substr(0, q.find('/'));
}
double PicRetriver::m_L2(const int* histP, const int* histQ, int bins){
	if (bins != 16 && bins != 128) {
		cerr << "L2 Error: bins = " << bins << ",but should be 16 or 128" << endl;
		return -1;
	}
	else {
		try {
			double sum = 0;
			for (int i = 0; i < bins; ++i) {
				sum += ((double)histP[i] - histQ[i]) * ((double)histP[i] - histQ[i]);
			}
			return sqrt(sum);
		}
		catch (range_error) {
			cerr << "L2 Error: Try to read element out of range" << endl;
			return -1;
		}
		catch (exception e) {
			cerr << "L2 Error: Error occurs in L2: " << e.what() << endl;
			return -2;
		}
	}
}

double PicRetriver::m_HI(const int* histP,const int* histQ, int bins){
	if (bins != 16 && bins != 128) {
		cerr << "HI Error: bins = " << bins << ",but should be 16 or 128" << endl;
		return -1;
	}
	else {
		try {
			double sumMin = 0, sumQ = 0;
			for (int i = 0; i < bins; ++i) {
				sumMin += min(histP[i], histQ[i]);
				sumQ += histQ[i];
			}
			return sumMin / sumQ;
		}
		catch (range_error e) {
			cerr << "HI Error: Try to read element out of range." << endl;
			return -1;
		}
		catch (exception e) {
			cerr << "HI Error: Error occurs in HI: " << e.what() << endl;
			return -2;
		}
	}
}
/*
input percentile histogram
*/
double PicRetriver::m_Bh(const double* histPd,const double* histQd, int bins){
	if (bins != 16 && bins != 128) {
		cerr << "Bh Error: bins = " << bins << ",but should be 16 or 128" << endl;
		return -1;
	}
	else {
		try {
			double sum = 0;
			for (int i = 0; i < bins; ++i) {
				sum += sqrt(histPd[i] * histQd[i]);
			}
			return sqrt(1 - sum);
		}
		catch (range_error e) {
			cerr << "Bh Error: Try to read element out of range." << endl;
			return -1;
		}
		catch (exception e) {
			cerr << "Bh Error: Error occurs in Bh: " << e.what() << endl;
			return -2;
		}
	}
}
