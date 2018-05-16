#include"imageInfo.h"
imageInfo::imageInfo()
{

}

imageInfo::~imageInfo()
{
}

void imageInfo::setPath(string p) {
	path = p;
}

void imageInfo::setClassify_gray_hist(float g) {
	gray_hist = g;
}
void imageInfo::setClassify_hist_with_split(float h) {
	hist_with_split = h;
}
void imageInfo::setClassify_aHash(int a) {
	aHash = a;
}
void imageInfo::setClassify_pHash(int p) {
	pHash = p;
}

string imageInfo::getPath() {
	return path;
}
float imageInfo::getClassify_gray_hist() {
	return gray_hist;
}
float imageInfo::getClassify_hist_with_split() {
	return hist_with_split;
}
int imageInfo::getClassify_aHash() {
	return aHash;
}
int imageInfo::getClassify_pHash() {
	return pHash;
}