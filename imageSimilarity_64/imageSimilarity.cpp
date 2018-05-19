#include "imageSimilarity.h"
#include<QDebug>
#include<QMessageBox>
#include<math.h>
#include <qscrollbar.h>

imageSimilarity::imageSimilarity(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::imageSimilarityClass;
	ui->setupUi(this);

	//打开第一张图像信号槽
	QObject::connect(ui->actionOpen, &QAction::triggered, this, &imageSimilarity::open);
	//图像库处理信号槽
	QObject::connect(ui->actionSimilar, &QAction::triggered, this, &imageSimilarity::similar);
	//选择图像库信号槽
	QObject::connect(ui->actionImportLib, &QAction::triggered, this, &imageSimilarity::chooseImageLib);

	//展示上一页图像
	QObject::connect(ui->lastPageBtn, &QPushButton::clicked , this, &imageSimilarity::showLastPage);
	//下一页
	QObject::connect(ui->nextPageBtn, &QPushButton::clicked, this, &imageSimilarity::showNextPage);
	//comboBox被改变
	QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(chooseSortMethod(int)));

	ui->actionSimilar->setEnabled(false);
	ui->textEdit->setReadOnly(true);
}
imageSimilarity::~imageSimilarity()
{
	delete ui;
}
void imageSimilarity::open() {
	//调用窗口打开文件
	filename = QFileDialog::getOpenFileName(this,
		tr("打开图片"),
		".",
		tr("Image file(*.png *.jpg *.bmp)"));
	p1 = filename.toStdString();

	if (!filename.isEmpty()) {
		ui->SrcImgView->clear();
		//QImage加载成功则展示
		if (srcQimage.load(filename)) {
			ui->actionSimilar->setEnabled(true);

			int width = ui->SrcImgView->width();
			int height = ui->SrcImgView->height();
			QPixmap pixmap = QPixmap::fromImage(srcQimage);
			QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
			ui->SrcImgView->setPixmap(fitpixmap);
			nowPage = -1;//当前页为-1
		}
		else {
			ui->actionSimilar->setEnabled(false);
			QMessageBox::information(this,
				tr("操作失败"),
				tr("图片打开失败!"));
			return;
		}
	}
}
void imageSimilarity::chooseImageLib() {
	path = QFileDialog::getExistingDirectory(NULL, tr("选择图像库文件夹"), "", QFileDialog::ShowDirsOnly);
	//qDebug() << path;
	QDir *dir = new QDir(path);//本机E:/Item/C++/imageSimilarity/imageSimilarity/imageLib
	QStringList filter;
	filter << "*.jpg";
	dir->setNameFilters(filter);
	list = dir->entryInfoList();
	imageCount = list.size();
	pageNum = ceil(imageCount / 4.0);//总页数,向上取整
	nowPage = -1;////图片库处理时，图片页置位
}
void imageSimilarity::similar() {
	if (path.isEmpty() || path.isNull()) {//如果路径为空或者不存在
		chooseImageLib();
	}
	if (imagelibs.size() != 0) {
		imagelibs.clear();

	}
	ui->textEdit->clear();

	
	if (list.length() != 0) {
		//遍历待处理图库
		for (int i = 0; i < list.size(); i++) {
			//实例化py图像相似度处理对象，将待比较的第一张图像路径导入
			pySimilarityCore *core = new pySimilarityCore(p1);
			//第二张图像的路径，随循环改变
			QString pathJpg = path + "/" + list.at(i).fileName();
			//qDebug() << pathJpg;

			QFileInfo fileInfo(pathJpg);

			QScrollBar *scrollbar = ui->textEdit->verticalScrollBar();
			if (scrollbar) {
				scrollbar->setSliderPosition(scrollbar->maximum());
			}
			ui->textEdit->insertPlainText("图片:" + pathJpg + "\n正在进行相似度分析...\n");
			statusBar()->showMessage(tr("正在对图像库进行分析..."));

			//设置处理对象中第二张图的路径
			core->getPath2(pathJpg.toStdString());

			//以下图像处理后的信息保存
			imageInfo *info = new imageInfo();
			//灰度图的直方图比较
			info->setClassify_gray_hist(core->doSimilarity_classify_gray_hist());
			//分离三通道的直方图比较
			info->setClassify_hist_with_split(core->doSimilarity_classify_hist_with_split());
			//平均hash
			info->setClassify_aHash(core->doSimilarity_classify_aHash());
			//感知hash
			info->setClassify_pHash(core->doSimilarity_classify_pHash());
			//sift特征匹配
			//info->setSift_point(core->doSift());
			////surf特征匹配
			//info->setSurf_point(core->doSurf());
			//每张图像的路径保存到图像信息对象中
			info->setPath(pathJpg.toStdString());


			Py_Initialize();//关闭py环境
			imagelibs.push_back(info);//导入存储图像相似度形象的动态数组

			ui->textEdit->insertPlainText("分析完毕！\n");
			QApplication::processEvents();
			delete core;
		}
	}
	else{
		qDebug() << "no file";
	}
	statusBar()->showMessage(tr("图像库处理完毕！"));
	int choose = ui->comboBox->currentIndex();
	//随选择的相似度处理算法排序
	chooseSortMethod(choose);
	
}

//排序
void imageSimilarity::chooseSortMethod(int choose) {
	if (choose == 0) {
		std::sort(imagelibs.begin(), imagelibs.end(), &imageSimilarity::sortMethod_gray_hist);
	}
	else if (choose == 1) {
		std::sort(imagelibs.begin(), imagelibs.end(), &imageSimilarity::sortMethod_hist_split);
	}
	else if (choose == 2) {
		std::sort(imagelibs.begin(), imagelibs.end(), &imageSimilarity::sortMethod_aHash);
	}
	else if (choose == 3) {
		std::sort(imagelibs.begin(), imagelibs.end(), &imageSimilarity::sortMethod_pHash);
	}
	else if (choose == 4) {
		std::sort(imagelibs.begin(), imagelibs.end(), &imageSimilarity::sortMethod_sift);
	}
	else if (choose == 5) {
		std::sort(imagelibs.begin(), imagelibs.end(), &imageSimilarity::sortMethod_surf);
	}

	//改变相似度方法时，图片页置位
	nowPage = -1;
	showNextPage();
}

//void imageSimilarity::showLabel() {
//	nowPage = 0;//当前页为0
//	ui->textEdit->clear();
//	ui->label->clear();
//	//qDebug() << QString::fromStdString((imagelibs.begin())->getPath());
//	//qDebug() << imagelibs.size();
//	QString filep = QString::fromStdString((imagelibs.begin())->getPath());
//	showImageInfo(*(imagelibs.begin()));
//	int width = ui->label->width();
//	int height = ui->label->height();
//	QImage image1(filep);
//	QPixmap pixmap = QPixmap::fromImage(image1);
//	QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//	//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
//	ui->label->setPixmap(fitpixmap);
//
//	ui->label_2->clear();
//	QString filep2 = QString::fromStdString((imagelibs.begin() + 1)->getPath());
//	showImageInfo(*(imagelibs.begin()+1));
//	QImage image2(filep2);
//	pixmap = QPixmap::fromImage(image2);
//	fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//	//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
//	ui->label_2->setPixmap(fitpixmap);
//
//	ui->label_3->clear();
//	//qDebug() << QString::fromStdString((imagelibs.begin() + 2)->getPath());
//	QString filep3 = QString::fromStdString((imagelibs.begin() + 2)->getPath());
//	showImageInfo(*(imagelibs.begin()+2));
//	QImage image3(filep3);
//	pixmap = QPixmap::fromImage(image3);
//	fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//	//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
//	ui->label_3->setPixmap(fitpixmap);
//
//	ui->label_4->clear();
//	//qDebug() << QString::fromStdString((imagelibs.begin() + 3)->getPath());
//	QString filep4 = QString::fromStdString((imagelibs.begin() + 3)->getPath());
//	showImageInfo(*(imagelibs.begin() + 3));
//	QImage image4(filep4);
//	pixmap = QPixmap::fromImage(image4);
//	fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//	//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
//	ui->label_4->setPixmap(fitpixmap);
//}

void imageSimilarity::showLastPage() {
	if (nowPage > 0) {
		nowPage--;
		ui->textEdit->clear();
		int width = ui->label->width();
		int height = ui->label->height();
		//qDebug() << QString::fromStdString((imagelibs.begin())->getPath());
		//qDebug() << imagelibs.size();
		QList<QLabel *> showLabel = ui->groupBox->findChildren<QLabel *>();
		for (int i = 0; i < showLabel.count(); i++) {
			showLabel.at(i)->clear();
			if ((nowPage * 4 + i)<list.size()) {
				imageInfo *nowInfo = imagelibs.at(nowPage * 4 + i);
				showImageInfo(nowInfo);
				QImage image1(QString::fromStdString(nowInfo->getPath()));
				QPixmap pixmap = QPixmap::fromImage(image1);
				QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				showLabel.at(i)->setPixmap(fitpixmap);
			}
			else {
				qDebug() << "no file";
			}
		}
	}
}

void imageSimilarity::showNextPage() {
	if (nowPage < pageNum-1) {
		nowPage++;
		ui->textEdit->clear();
		int width = ui->label->width();
		int height = ui->label->height();
		//qDebug() << QString::fromStdString((imagelibs.begin())->getPath());
		//qDebug() << imagelibs.size();
		QList<QLabel *> showLabel = ui->groupBox->findChildren<QLabel *>();
		for (int i = 0; i < showLabel.count(); i++) {
			showLabel.at(i)->clear();
			if ((nowPage * 4 + i)<list.size()) {
				imageInfo *nowInfo=imagelibs.at(nowPage * 4 + i);
				showImageInfo(nowInfo);
				QImage image1(QString::fromStdString(nowInfo->getPath()));
				QPixmap pixmap = QPixmap::fromImage(image1);
				QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				showLabel.at(i)->setPixmap(fitpixmap);
			}
			else {
				qDebug() << "no file";
			}
		}		
	}
}

void imageSimilarity::showImageInfo(imageInfo *img) {
	ui->textEdit->insertPlainText("图片路径:" + QString::fromStdString(img->getPath()) + "\n");
	ui->textEdit->insertPlainText("灰度直方图比较:" + QString::number(img->getClassify_gray_hist(), 'f') + "\n");
	ui->textEdit->insertPlainText("三通道分离比较:" + QString::number(img->getClassify_hist_with_split(), 'f') + "\n");
	ui->textEdit->insertPlainText("aHash:" + QString::number(img->getClassify_aHash(), 10) + "\n");
	ui->textEdit->insertPlainText("pHash:" + QString::number(img->getClassify_pHash(), 10) + "\n");
	ui->textEdit->insertPlainText("sift特征匹配点数:" + QString::number(img->getSift_point(), 10) + "\n");
	ui->textEdit->insertPlainText("surf特征匹配点数:" + QString::number(img->getSurf_point(), 10) + "\n\n");
	QApplication::processEvents();
}

bool imageSimilarity::sortMethod_gray_hist(imageInfo *aaa, imageInfo *bbb) {
	return ((aaa->getClassify_gray_hist())>(bbb->getClassify_gray_hist()));
}
bool imageSimilarity::sortMethod_hist_split(imageInfo *aaa, imageInfo *bbb) {
	return ((aaa->getClassify_hist_with_split())>(bbb->getClassify_hist_with_split()));
}
bool imageSimilarity::sortMethod_aHash(imageInfo *aaa, imageInfo *bbb) {
	return ((aaa->getClassify_aHash())<(bbb->getClassify_aHash()));
}
bool imageSimilarity::sortMethod_pHash(imageInfo *aaa, imageInfo *bbb) {
	return ((aaa->getClassify_pHash())<(bbb->getClassify_pHash()));
}
bool imageSimilarity::sortMethod_sift(imageInfo *aaa, imageInfo *bbb) {
	return ((aaa->getSift_point())>(bbb->getSift_point()));
}
bool imageSimilarity::sortMethod_surf(imageInfo *aaa, imageInfo *bbb) {
	return ((aaa->getSurf_point())>(bbb->getSurf_point()));
}