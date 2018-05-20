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
	ui->comboBox->setEnabled(false);
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
	QDir *dir = new QDir(path);
	QStringList filter;
	filter << "*.jpg";
	dir->setNameFilters(filter);
	list = dir->entryInfoList();
	imageCount = list.size();
	pageNum = ceil(imageCount / 4.0);//总页数,向上取整
	nowPage = -1;////图片库处理时，图片页置位
	ui->comboBox->setEnabled(true);
}
void imageSimilarity::similar() {
	if (path.isEmpty() || path.isNull()) {//判断是否导入图像库
		chooseImageLib();
	}

	if (imagelibs.size() != 0) {
		std::vector<imageInfo*>::iterator iter = imagelibs.begin();
		//释放掉动态数组所保存的指针指向区域的对象
		for (; iter != imagelibs.end(); ++iter)
		{
			delete *iter; 
		}
		imagelibs.clear();
	}
	ui->textEdit->clear();

	//如果该图像库中有jpg图像
	if (list.length() != 0) {
		//遍历待处理图库
		for (int i = 0; i < list.size(); i++) {
			//实例化py图像相似度处理对象，将待比较的第一张图像路径导入
			pySimilarityCore *core = new pySimilarityCore(p1);
			//第二张图像的路径，随循环改变
			QString pathJpg = path + "/" + list.at(i).fileName();
			//文本框一直向下滚动
			QScrollBar *scrollbar = ui->textEdit->verticalScrollBar();
			if (scrollbar) {
				scrollbar->setSliderPosition(scrollbar->maximum());
			}
			QApplication::processEvents();

			ui->textEdit->insertPlainText("图片:" + pathJpg + "\n正在进行相似度分析...\n");
			statusBar()->showMessage(tr("正在对图像库进行分析..."));

			//设置处理对象中第二张图的路径
			core->getPath2(pathJpg.toStdString());

			//以下进行图像处理并信息保存
			imageInfo *info = new imageInfo();
			//灰度图的直方图比较
			ui->textEdit->insertPlainText("灰度图直方图比较...\n");
			info->setClassify_gray_hist(core->doSimilarity_classify_gray_hist());
			//分离三通道的直方图比较
			ui->textEdit->insertPlainText("分离三通道的直方图比较...\n");
			info->setClassify_hist_with_split(core->doSimilarity_classify_hist_with_split());
			//平均hash
			ui->textEdit->insertPlainText("平均hash...\n");
			info->setClassify_aHash(core->doSimilarity_classify_aHash());
			//感知hash
			ui->textEdit->insertPlainText("感知hash...\n");
			info->setClassify_pHash(core->doSimilarity_classify_pHash());
			
			if (ui->checkBox_sift->isChecked() == true) {
				//sift特征匹配
				ui->textEdit->insertPlainText("sift特征匹配中...\n");
				info->setSift_point(core->doSift());
			}
			if (ui->checkBox_surf->isChecked() == true) {
				//surf特征匹配
				ui->textEdit->insertPlainText("surf特征匹配中...\n");
				info->setSurf_point(core->doSurf());
			}
			//每张图像的路径保存到图像信息对象中
			info->setPath(pathJpg.toStdString());

			Py_Initialize();//关闭py环境
			imagelibs.push_back(info);//导入存储图像相似度形象的动态数组
			ui->textEdit->insertPlainText("分析完毕！\n\n");			
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
	if (imagelibs.size()>0) {
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
	}else{
		qDebug() << "还未处理图像";
	}
}

void imageSimilarity::showLastPage() {
	if (nowPage > 0) {
		nowPage--;
		statusBar()->showMessage(tr("总页数:") + QString::number(pageNum, 10) + tr("/") + tr("当前页:") + QString::number(nowPage + 1, 10));
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

//展示下一页
void imageSimilarity::showNextPage() {
	if (nowPage < pageNum-1) {
		nowPage++;
		statusBar()->showMessage(tr("总页数:")+ QString::number(pageNum,10)+tr("/")+tr("当前页:") + QString::number(nowPage+1, 10));
		ui->textEdit->clear();//清空信息框
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