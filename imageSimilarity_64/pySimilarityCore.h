#pragma once
#include<iostream>
#include"Python_wrapper.h"
#include<QDebug>

using namespace std;

class pySimilarityCore {
	
public:
	pySimilarityCore(string);
	~pySimilarityCore();

	void getPath2(string);
	float doSimilarity_classify_gray_hist();
	float doSimilarity_classify_hist_with_split();
	int doSimilarity_classify_aHash();
	int doSimilarity_classify_pHash();
	int doSift();
	int doSurf();

private:
	string image1Path;
	string image2Path;
	PyObject* pModule;
};