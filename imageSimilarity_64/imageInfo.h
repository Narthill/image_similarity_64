#pragma once
#include<iostream>
using namespace std;
class imageInfo
{
public:
	imageInfo();
	imageInfo(const imageInfo& info);
	~imageInfo();

	//imageInfo operator =(const imageInfo& info);//重载等号，用以string深拷贝

	void setClassify_gray_hist(float);
	void setClassify_hist_with_split(float);
	void setClassify_aHash(int);
	void setClassify_pHash(int);
	void setSift_point(int);
	void setSurf_point(int);
	void setPath(string);

	float getClassify_gray_hist();
	float getClassify_hist_with_split();
	int getClassify_aHash();
	int getClassify_pHash();
	int getSift_point();
	int getSurf_point();
	string getPath();


private:
	string path="";
	float gray_hist=0;
	float hist_with_split=0;
	int aHash=0;
	int pHash=0;
	int siftPoint = 0;
	int surfPoint = 0;
};

