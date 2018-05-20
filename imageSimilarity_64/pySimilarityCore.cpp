#include "pySimilarityCore.h"
pySimilarityCore::pySimilarityCore(string Path1) {
	image1Path = Path1;
	//Py_Initialize();    // 初始化

	// 将Python工作路径切换到待调用模块所在目录，一定要保证路径名的正确性
	string path = "E:/Item/C++/imageSimilarity_64/imageSimilarity_64";  //python文件路径  

	//初始化Python环境  
	Py_Initialize();
	PyRun_SimpleString("import sys");
	string chdir_cmd = string("sys.path.append(\"");
	chdir_cmd += path;
	chdir_cmd += "\")";

	//PyRun_SimpleString("")可以简单的直接运行字符串内的Python语句  
	const char* cstr_cmd = chdir_cmd.c_str();
	//添加Insert模块路径  
	PyRun_SimpleString(cstr_cmd);

	//PyObject* fname = PyUnicode_FromString("pyadd");
	pModule = PyImport_ImportModule("seemImg_py3_64");//GameState:Python文件名
	if (!pModule) // 加载模块失败
	{
		qDebug() << "[ERROR] Python get module failed." ;
	}
	qDebug() << "[INFO] Python get module succeed." ;
}
pySimilarityCore::~pySimilarityCore() {
	//delete pModule;
	//Py_Finalize();
}

void pySimilarityCore::getPath2(string Path2) {
	image2Path = Path2;
}

float pySimilarityCore::doSimilarity_classify_gray_hist() {
	// 加载函数
	PyObject* pv = PyObject_GetAttrString(pModule, "doSimilarity_classify_gray_hist");
	if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
	{
		qDebug() << "[ERROR] Can't find funftion (doSimilarity_classify_gray_hist)" ;
	}
	qDebug() << "[INFO] Get function (doSimilarity_classify_gray_hist) succeed." ;

	// 设置参数
	PyObject* args = PyTuple_New(2);   // 2个参数
	PyObject* arg1 = Py_BuildValue("s", image1Path.c_str());    // 参数一
	PyObject* arg2 = Py_BuildValue("s", image2Path.c_str());    // 参数二

	PyTuple_SetItem(args, 0, arg1);
	PyTuple_SetItem(args, 1, arg2);

	// 调用函数
	PyObject* pRet = PyObject_CallObject(pv, args);

	// 获取参数
	float result;
	if (pRet) {
		PyArg_Parse(pRet, "f", &result);//转换成float型变量
		qDebug() << "基于灰度化直方图相似度:" << result ;
	}
	else {
		qDebug() << "获取失败" ;
	}
	return result;
}

float pySimilarityCore::doSimilarity_classify_hist_with_split(){
	PyObject* pv = PyObject_GetAttrString(pModule, "doSimilarity_classify_hist_with_split");
	if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
	{
		qDebug() << "[ERROR] Can't find funftion (doSimilarity_classify_hist_with_split)" ;
	}
	qDebug() << "[INFO] Get function (doSimilarity_classify_hist_with_split) succeed." ;

	// 设置参数
	PyObject* args = PyTuple_New(2);   // 2个参数
	PyObject* arg1 = Py_BuildValue("s", image1Path.c_str());    // 参数一设为4
	PyObject* arg2 = Py_BuildValue("s", image2Path.c_str());    // 参数二设为3
	PyTuple_SetItem(args, 0, arg1);
	PyTuple_SetItem(args, 1, arg2);

	// 调用函数
	PyObject* pRet = PyObject_CallObject(pv, args);

	float result;
	if (pRet) {
		PyArg_Parse(pRet, "f", &result);//float型变量
		qDebug() << "分离各通道直方图后的相似度:" << result ;
	}
	else {
		qDebug() << "获取失败" ;
	}
	return result;
}

int pySimilarityCore::doSimilarity_classify_aHash() {
	PyObject* pv = PyObject_GetAttrString(pModule, "doSimilarity_classify_aHash");
	if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
	{
		qDebug() << "[ERROR] Can't find funftion (doSimilarity_classify_aHash)" ;
	}
	qDebug() << "[INFO] Get function (doSimilarity_classify_aHash) succeed." ;

	// 设置参数
	PyObject* args = PyTuple_New(2);   // 2个参数
	PyObject* arg1 = Py_BuildValue("s", image1Path.c_str());    // 参数一
	PyObject* arg2 = Py_BuildValue("s", image2Path.c_str());    // 参数二
	PyTuple_SetItem(args, 0, arg1);
	PyTuple_SetItem(args, 1, arg2);

	// 调用函数
	PyObject* pRet = PyObject_CallObject(pv, args);

	int result;
	if (pRet) {
		PyArg_Parse(pRet, "i", &result);//int型变量
		qDebug() << "aHash:" << result ;
	}
	else {
		qDebug() << "获取失败" ;
	}
	return result;
}

int pySimilarityCore::doSimilarity_classify_pHash() {
	PyObject* pv = PyObject_GetAttrString(pModule, "doSimilarity_classify_pHash");
	if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
	{
		qDebug() << "[ERROR] Can't find funftion (doSimilarity_classify_pHash)" ;
	}
	qDebug() << "[INFO] Get function (doSimilarity_classify_pHash) succeed." ;

	// 设置参数
	PyObject* args = PyTuple_New(2);   // 2个参数
	PyObject* arg1 = Py_BuildValue("s", image1Path.c_str());    // 参数一
	PyObject* arg2 = Py_BuildValue("s", image2Path.c_str());    // 参数二
	PyTuple_SetItem(args, 0, arg1);
	PyTuple_SetItem(args, 1, arg2);

	// 调用函数
	PyObject* pRet = PyObject_CallObject(pv, args);

	int result;
	if (pRet) {
		PyArg_Parse(pRet, "i", &result);//int型变量
		qDebug() << "pHash:" << result ;
	}
	else {
		qDebug() << "获取失败" ;
	}
	return result;
}

int pySimilarityCore::doSift() {
	PyObject* pv = PyObject_GetAttrString(pModule, "doSift");
	if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
	{
		qDebug() << "[ERROR] Can't find funftion (doSift)";
	}
	qDebug() << "[INFO] Get function (doSift) succeed.";

	// 设置参数
	PyObject* args = PyTuple_New(2);   // 2个参数
	PyObject* arg1 = Py_BuildValue("s", image1Path.c_str());    // 参数一
	PyObject* arg2 = Py_BuildValue("s", image2Path.c_str());    // 参数二
	PyTuple_SetItem(args, 0, arg1);
	PyTuple_SetItem(args, 1, arg2);

	// 调用函数
	PyObject* pRet = PyObject_CallObject(pv, args);

	int result;
	if (pRet) {
		PyArg_Parse(pRet, "i", &result);//int型变量
		qDebug() << "Sift特征匹配点数:" << result;
	}
	else {
		qDebug() << "获取失败";
	}
	return result;
}

int pySimilarityCore::doSurf() {
	PyObject* pv = PyObject_GetAttrString(pModule, "doSurf");
	if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
	{
		qDebug() << "[ERROR] Can't find funftion (doSurf)";
	}
	qDebug() << "[INFO] Get function (doSurf) succeed.";

	// 设置参数
	PyObject* args = PyTuple_New(2);   // 2个参数
	PyObject* arg1 = Py_BuildValue("s", image1Path.c_str());    // 参数一
	PyObject* arg2 = Py_BuildValue("s", image2Path.c_str());    // 参数二
	PyTuple_SetItem(args, 0, arg1);
	PyTuple_SetItem(args, 1, arg2);

	// 调用函数
	PyObject* pRet = PyObject_CallObject(pv, args);

	int result;
	if (pRet) {
		PyArg_Parse(pRet, "i", &result);//int型变量
		qDebug() << "Surf特征匹配点数:" << result;
	}
	else {
		qDebug() << "获取失败";
	}
	return result;
}