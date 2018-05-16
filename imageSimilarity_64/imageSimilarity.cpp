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

	QObject::connect(ui->actionOpen, &QAction::triggered, this, &imageSimilarity::open);
	QObject::connect(ui->actionSimilar, &QAction::triggered, this, &imageSimilarity::similar);
	QObject::connect(ui->actionImportLib, &QAction::triggered, this, &imageSimilarity::chooseImageLib);

	QObject::connect(ui->lastPageBtn, &QPushButton::clicked , this, &imageSimilarity::showLastPage);
	QObject::connect(ui->nextPageBtn, &QPushButton::clicked, this, &imageSimilarity::showNextPage);
	QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(chooseSortMethod(int)));

	ui->actionSimilar->setEnabled(false);
	ui->textEdit->setReadOnly(true);
	nowPage = 0;//当前页为0
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
			nowPage = 0;//当前页为0
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
	QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
	imageCount = fileInfo->count();
	pageNum = ceil(imageCount / 4.0);//总页数,向上取整
	//qDebug() << fileInfo->count();
	//qDebug() << pageNum;
	nowPage = 0;//当前页为0
}
void imageSimilarity::similar() {
	if (path.isEmpty() || path.isNull()) {//如果路径为空或者不存在
		chooseImageLib();
	}
	if (imagelibs.size() != 0) {
		imagelibs.clear();
	}
	ui->textEdit->clear();

	imageInfo info;
	for (int pic = 0; pic < imageCount + 1; pic++) {
		pySimilarityCore *a = new pySimilarityCore(p1);
		QString pathJpg = path +"/" + QString::number(pic,10) + ".jpg";

		QFileInfo fileInfo(pathJpg);
		if (fileInfo.exists()) {
		//qDebug() << "路径:"+QString::fromStdString(p2);

			QScrollBar *scrollbar = ui->textEdit->verticalScrollBar();
			if (scrollbar) {
				scrollbar->setSliderPosition(scrollbar->maximum());
			}
			ui->textEdit->insertPlainText("图片:" + pathJpg+"\n正在进行相似度分析...\n");
			statusBar()->showMessage(tr("正在对图像库进行分析..."));

			//图像处理后信息保存
			a->getPath2(pathJpg.toStdString());
			info.setClassify_gray_hist(a->doSimilarity_classify_gray_hist());
			info.setClassify_hist_with_split(a->doSimilarity_classify_hist_with_split());
			info.setClassify_aHash(a->doSimilarity_classify_aHash());
			info.setClassify_pHash(a->doSimilarity_classify_pHash());
			info.setPath(pathJpg.toStdString());

			Py_Initialize();//关闭py环境
			imagelibs.push_back(info);//导入存储图像相似度形象的动态数组

			ui->textEdit->insertPlainText("分析完毕！\n");
			QApplication::processEvents();
			delete a;
		}
		else {
			delete a;
			continue;
		}
	}
	statusBar()->showMessage(tr("图像库处理完毕！"));
	int choose = ui->comboBox->currentIndex();
	chooseSortMethod(choose);
	
}

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

	showLabel();
}

void imageSimilarity::showLabel() {
	nowPage = 0;//当前页为0
	ui->textEdit->clear();
	ui->label->clear();
	//qDebug() << QString::fromStdString((imagelibs.begin())->getPath());
	//qDebug() << imagelibs.size();
	QString filep = QString::fromStdString((imagelibs.begin())->getPath());
	showImageInfo(*(imagelibs.begin()));
	int width = ui->label->width();
	int height = ui->label->height();
	QImage image1(filep);
	QPixmap pixmap = QPixmap::fromImage(image1);
	QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
	ui->label->setPixmap(fitpixmap);

	ui->label_2->clear();
	QString filep2 = QString::fromStdString((imagelibs.begin() + 1)->getPath());
	showImageInfo(*(imagelibs.begin()+1));
	QImage image2(filep2);
	pixmap = QPixmap::fromImage(image2);
	fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
	ui->label_2->setPixmap(fitpixmap);

	ui->label_3->clear();
	//qDebug() << QString::fromStdString((imagelibs.begin() + 2)->getPath());
	QString filep3 = QString::fromStdString((imagelibs.begin() + 2)->getPath());
	showImageInfo(*(imagelibs.begin()+2));
	QImage image3(filep3);
	pixmap = QPixmap::fromImage(image3);
	fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
	ui->label_3->setPixmap(fitpixmap);

	ui->label_4->clear();
	//qDebug() << QString::fromStdString((imagelibs.begin() + 3)->getPath());
	QString filep4 = QString::fromStdString((imagelibs.begin() + 3)->getPath());
	showImageInfo(*(imagelibs.begin() + 3));
	QImage image4(filep4);
	pixmap = QPixmap::fromImage(image4);
	fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
	ui->label_4->setPixmap(fitpixmap);
}

void imageSimilarity::showLastPage() {
	if (nowPage > 0) {
		nowPage--;
		ui->textEdit->clear();
		ui->label->clear();
		//qDebug() << QString::fromStdString((imagelibs.begin())->getPath());
		//qDebug() << imagelibs.size();
		QString filep = QString::fromStdString((imagelibs.begin() +nowPage * 4)->getPath());
		showImageInfo(*(imagelibs.begin() + nowPage * 4));
		int width = ui->label->width();
		int height = ui->label->height();
		QImage image1(filep);
		QPixmap pixmap = QPixmap::fromImage(image1);
		QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
		ui->label->setPixmap(fitpixmap);

		ui->label_2->clear();
		QString filep2 = QString::fromStdString((imagelibs.begin() + nowPage  * 4 + 1)->getPath());
		showImageInfo(*(imagelibs.begin() + nowPage * 4+1));
		QImage image2(filep2);
		pixmap = QPixmap::fromImage(image2);
		fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
		ui->label_2->setPixmap(fitpixmap);

		ui->label_3->clear();
		//qDebug() << QString::fromStdString((imagelibs.begin() + 2)->getPath());
		QString filep3 = QString::fromStdString((imagelibs.begin() + nowPage  * 4 + 2)->getPath());
		showImageInfo(*(imagelibs.begin() + nowPage * 4+2));
		QImage image3(filep3);
		pixmap = QPixmap::fromImage(image3);
		fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
		ui->label_3->setPixmap(fitpixmap);

		ui->label_4->clear();
		//qDebug() << QString::fromStdString((imagelibs.begin() + 3)->getPath());
		QString filep4 = QString::fromStdString((imagelibs.begin()+nowPage  * 4 + 3)->getPath());
		showImageInfo(*(imagelibs.begin() + nowPage * 4+3));
		QImage image4(filep4);
		pixmap = QPixmap::fromImage(image4);
		fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
		ui->label_4->setPixmap(fitpixmap);
		
	}
}

void imageSimilarity::showNextPage() {
	if (nowPage < pageNum) {
		nowPage++;
		ui->textEdit->clear();
		int width = ui->label->width();
		int height = ui->label->height();
		ui->label->clear();
		ui->label_2->clear();
		ui->label_3->clear();
		ui->label_4->clear();
		//qDebug() << QString::fromStdString((imagelibs.begin())->getPath());
		//qDebug() << imagelibs.size();
		QString filep = QString::fromStdString((imagelibs.begin() + nowPage * 4)->getPath());
		showImageInfo(*(imagelibs.begin() + nowPage * 4));
		QImage image1(filep);
		QPixmap pixmap = QPixmap::fromImage(image1);
		QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
		ui->label->setPixmap(fitpixmap);

		
		QString filep2 = QString::fromStdString((imagelibs.begin() + nowPage * 4 + 1)->getPath());
		showImageInfo(*(imagelibs.begin() + nowPage * 4+1));
		QImage image2(filep2);
		pixmap = QPixmap::fromImage(image2);
		fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
		ui->label_2->setPixmap(fitpixmap);

		
		//qDebug() << QString::fromStdString((imagelibs.begin() + 2)->getPath());
		QString filep3 = QString::fromStdString((imagelibs.begin() + nowPage* 4 + 2)->getPath());
		showImageInfo(*(imagelibs.begin() + nowPage * 4 +2));
		QImage image3(filep3);
		pixmap = QPixmap::fromImage(image3);
		fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
		ui->label_3->setPixmap(fitpixmap);

		
		//qDebug() << QString::fromStdString((imagelibs.begin() + 3)->getPath());
		QString filep4 = QString::fromStdString((imagelibs.begin() + nowPage * 4 + 3)->getPath());
		showImageInfo(*(imagelibs.begin() + nowPage * 4+3));
		QImage image4(filep4);
		pixmap = QPixmap::fromImage(image4);
		fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		//Qt::IgnoreAspectRatio, Qt::SmoothTransformation 填充 Qt::KeepAspectRatio, Qt::SmoothTransformation 按比例
		ui->label_4->setPixmap(fitpixmap);
		
	}
}

void imageSimilarity::showImageInfo(imageInfo img) {
	ui->textEdit->insertPlainText("图片路径:" + QString::fromStdString(img.getPath()) + "\n");
	ui->textEdit->insertPlainText("灰度直方图比较:" + QString::number(img.getClassify_gray_hist(), 'f') + "\n");
	ui->textEdit->insertPlainText("三通道分离比较:" + QString::number(img.getClassify_hist_with_split(), 'f') + "\n");
	ui->textEdit->insertPlainText("aHash:" + QString::number(img.getClassify_aHash(), 10) + "\n");
	ui->textEdit->insertPlainText("pHash:" + QString::number(img.getClassify_pHash(), 10) + "\n\n");
	QApplication::processEvents();
}

bool imageSimilarity::sortMethod_gray_hist(imageInfo aaa, imageInfo bbb) {
	return ((aaa.getClassify_gray_hist())>(bbb.getClassify_gray_hist()));
}
bool imageSimilarity::sortMethod_hist_split(imageInfo aaa, imageInfo bbb) {
	return ((aaa.getClassify_hist_with_split())>(bbb.getClassify_hist_with_split()));
}
bool imageSimilarity::sortMethod_aHash(imageInfo aaa, imageInfo bbb) {
	return ((aaa.getClassify_aHash())<(bbb.getClassify_aHash()));
}
bool imageSimilarity::sortMethod_pHash(imageInfo aaa, imageInfo bbb) {
	return ((aaa.getClassify_pHash())<(bbb.getClassify_pHash()));
}