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

	//�򿪵�һ��ͼ���źŲ�
	QObject::connect(ui->actionOpen, &QAction::triggered, this, &imageSimilarity::open);
	//ͼ��⴦���źŲ�
	QObject::connect(ui->actionSimilar, &QAction::triggered, this, &imageSimilarity::similar);
	//ѡ��ͼ����źŲ�
	QObject::connect(ui->actionImportLib, &QAction::triggered, this, &imageSimilarity::chooseImageLib);

	//չʾ��һҳͼ��
	QObject::connect(ui->lastPageBtn, &QPushButton::clicked , this, &imageSimilarity::showLastPage);
	//��һҳ
	QObject::connect(ui->nextPageBtn, &QPushButton::clicked, this, &imageSimilarity::showNextPage);
	//comboBox���ı�
	QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(chooseSortMethod(int)));

	ui->actionSimilar->setEnabled(false);
	ui->textEdit->setReadOnly(true);
}
imageSimilarity::~imageSimilarity()
{
	delete ui;
}
void imageSimilarity::open() {
	//���ô��ڴ��ļ�
	filename = QFileDialog::getOpenFileName(this,
		tr("��ͼƬ"),
		".",
		tr("Image file(*.png *.jpg *.bmp)"));
	p1 = filename.toStdString();

	if (!filename.isEmpty()) {
		ui->SrcImgView->clear();
		//QImage���سɹ���չʾ
		if (srcQimage.load(filename)) {
			ui->actionSimilar->setEnabled(true);

			int width = ui->SrcImgView->width();
			int height = ui->SrcImgView->height();
			QPixmap pixmap = QPixmap::fromImage(srcQimage);
			QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			//Qt::IgnoreAspectRatio, Qt::SmoothTransformation ��� Qt::KeepAspectRatio, Qt::SmoothTransformation ������
			ui->SrcImgView->setPixmap(fitpixmap);
			nowPage = -1;//��ǰҳΪ-1
		}
		else {
			ui->actionSimilar->setEnabled(false);
			QMessageBox::information(this,
				tr("����ʧ��"),
				tr("ͼƬ��ʧ��!"));
			return;
		}
	}
}
void imageSimilarity::chooseImageLib() {
	path = QFileDialog::getExistingDirectory(NULL, tr("ѡ��ͼ����ļ���"), "", QFileDialog::ShowDirsOnly);
	//qDebug() << path;
	QDir *dir = new QDir(path);//����E:/Item/C++/imageSimilarity/imageSimilarity/imageLib
	QStringList filter;
	filter << "*.jpg";
	dir->setNameFilters(filter);
	list = dir->entryInfoList();
	imageCount = list.size();
	pageNum = ceil(imageCount / 4.0);//��ҳ��,����ȡ��
	nowPage = -1;////ͼƬ�⴦��ʱ��ͼƬҳ��λ
}
void imageSimilarity::similar() {
	if (path.isEmpty() || path.isNull()) {//���·��Ϊ�ջ��߲�����
		chooseImageLib();
	}
	if (imagelibs.size() != 0) {
		imagelibs.clear();

	}
	ui->textEdit->clear();

	
	if (list.length() != 0) {
		//����������ͼ��
		for (int i = 0; i < list.size(); i++) {
			//ʵ����pyͼ�����ƶȴ�����󣬽����Ƚϵĵ�һ��ͼ��·������
			pySimilarityCore *core = new pySimilarityCore(p1);
			//�ڶ���ͼ���·������ѭ���ı�
			QString pathJpg = path + "/" + list.at(i).fileName();
			//qDebug() << pathJpg;

			QFileInfo fileInfo(pathJpg);

			QScrollBar *scrollbar = ui->textEdit->verticalScrollBar();
			if (scrollbar) {
				scrollbar->setSliderPosition(scrollbar->maximum());
			}
			ui->textEdit->insertPlainText("ͼƬ:" + pathJpg + "\n���ڽ������ƶȷ���...\n");
			statusBar()->showMessage(tr("���ڶ�ͼ�����з���..."));

			//���ô�������еڶ���ͼ��·��
			core->getPath2(pathJpg.toStdString());

			//����ͼ��������Ϣ����
			imageInfo *info = new imageInfo();
			//�Ҷ�ͼ��ֱ��ͼ�Ƚ�
			info->setClassify_gray_hist(core->doSimilarity_classify_gray_hist());
			//������ͨ����ֱ��ͼ�Ƚ�
			info->setClassify_hist_with_split(core->doSimilarity_classify_hist_with_split());
			//ƽ��hash
			info->setClassify_aHash(core->doSimilarity_classify_aHash());
			//��֪hash
			info->setClassify_pHash(core->doSimilarity_classify_pHash());
			//sift����ƥ��
			//info->setSift_point(core->doSift());
			////surf����ƥ��
			//info->setSurf_point(core->doSurf());
			//ÿ��ͼ���·�����浽ͼ����Ϣ������
			info->setPath(pathJpg.toStdString());


			Py_Initialize();//�ر�py����
			imagelibs.push_back(info);//����洢ͼ�����ƶ�����Ķ�̬����

			ui->textEdit->insertPlainText("������ϣ�\n");
			QApplication::processEvents();
			delete core;
		}
	}
	else{
		qDebug() << "no file";
	}
	statusBar()->showMessage(tr("ͼ��⴦����ϣ�"));
	int choose = ui->comboBox->currentIndex();
	//��ѡ������ƶȴ����㷨����
	chooseSortMethod(choose);
	
}

//����
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

	//�ı����ƶȷ���ʱ��ͼƬҳ��λ
	nowPage = -1;
	showNextPage();
}

//void imageSimilarity::showLabel() {
//	nowPage = 0;//��ǰҳΪ0
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
//	//Qt::IgnoreAspectRatio, Qt::SmoothTransformation ��� Qt::KeepAspectRatio, Qt::SmoothTransformation ������
//	ui->label->setPixmap(fitpixmap);
//
//	ui->label_2->clear();
//	QString filep2 = QString::fromStdString((imagelibs.begin() + 1)->getPath());
//	showImageInfo(*(imagelibs.begin()+1));
//	QImage image2(filep2);
//	pixmap = QPixmap::fromImage(image2);
//	fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//	//Qt::IgnoreAspectRatio, Qt::SmoothTransformation ��� Qt::KeepAspectRatio, Qt::SmoothTransformation ������
//	ui->label_2->setPixmap(fitpixmap);
//
//	ui->label_3->clear();
//	//qDebug() << QString::fromStdString((imagelibs.begin() + 2)->getPath());
//	QString filep3 = QString::fromStdString((imagelibs.begin() + 2)->getPath());
//	showImageInfo(*(imagelibs.begin()+2));
//	QImage image3(filep3);
//	pixmap = QPixmap::fromImage(image3);
//	fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//	//Qt::IgnoreAspectRatio, Qt::SmoothTransformation ��� Qt::KeepAspectRatio, Qt::SmoothTransformation ������
//	ui->label_3->setPixmap(fitpixmap);
//
//	ui->label_4->clear();
//	//qDebug() << QString::fromStdString((imagelibs.begin() + 3)->getPath());
//	QString filep4 = QString::fromStdString((imagelibs.begin() + 3)->getPath());
//	showImageInfo(*(imagelibs.begin() + 3));
//	QImage image4(filep4);
//	pixmap = QPixmap::fromImage(image4);
//	fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//	//Qt::IgnoreAspectRatio, Qt::SmoothTransformation ��� Qt::KeepAspectRatio, Qt::SmoothTransformation ������
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
	ui->textEdit->insertPlainText("ͼƬ·��:" + QString::fromStdString(img->getPath()) + "\n");
	ui->textEdit->insertPlainText("�Ҷ�ֱ��ͼ�Ƚ�:" + QString::number(img->getClassify_gray_hist(), 'f') + "\n");
	ui->textEdit->insertPlainText("��ͨ������Ƚ�:" + QString::number(img->getClassify_hist_with_split(), 'f') + "\n");
	ui->textEdit->insertPlainText("aHash:" + QString::number(img->getClassify_aHash(), 10) + "\n");
	ui->textEdit->insertPlainText("pHash:" + QString::number(img->getClassify_pHash(), 10) + "\n");
	ui->textEdit->insertPlainText("sift����ƥ�����:" + QString::number(img->getSift_point(), 10) + "\n");
	ui->textEdit->insertPlainText("surf����ƥ�����:" + QString::number(img->getSurf_point(), 10) + "\n\n");
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