#pragma once
#include<iostream>
using namespace std;
class imageInfo
{
public:
	imageInfo();
	~imageInfo();
	void setClassify_gray_hist(float);
	void setClassify_hist_with_split(float);
	void setClassify_aHash(int);
	void setClassify_pHash(int);
	void setPath(string);

	float getClassify_gray_hist();
	float getClassify_hist_with_split();
	int getClassify_aHash();
	int getClassify_pHash();
	string getPath();

private:
	string path=NULL;
	float gray_hist=0;
	float hist_with_split=0;
	int aHash=0;
	int pHash=0;
};

