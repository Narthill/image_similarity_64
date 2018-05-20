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
	ui->comboBox->setEnabled(false);
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
	QDir *dir = new QDir(path);
	QStringList filter;
	filter << "*.jpg";
	dir->setNameFilters(filter);
	list = dir->entryInfoList();
	imageCount = list.size();
	pageNum = ceil(imageCount / 4.0);//��ҳ��,����ȡ��
	nowPage = -1;////ͼƬ�⴦��ʱ��ͼƬҳ��λ
	ui->comboBox->setEnabled(true);
}
void imageSimilarity::similar() {
	if (path.isEmpty() || path.isNull()) {//�ж��Ƿ���ͼ���
		chooseImageLib();
	}

	if (imagelibs.size() != 0) {
		std::vector<imageInfo*>::iterator iter = imagelibs.begin();
		//�ͷŵ���̬�����������ָ��ָ������Ķ���
		for (; iter != imagelibs.end(); ++iter)
		{
			delete *iter; 
		}
		imagelibs.clear();
	}
	ui->textEdit->clear();

	//�����ͼ�������jpgͼ��
	if (list.length() != 0) {
		//����������ͼ��
		for (int i = 0; i < list.size(); i++) {
			//ʵ����pyͼ�����ƶȴ�����󣬽����Ƚϵĵ�һ��ͼ��·������
			pySimilarityCore *core = new pySimilarityCore(p1);
			//�ڶ���ͼ���·������ѭ���ı�
			QString pathJpg = path + "/" + list.at(i).fileName();
			//�ı���һֱ���¹���
			QScrollBar *scrollbar = ui->textEdit->verticalScrollBar();
			if (scrollbar) {
				scrollbar->setSliderPosition(scrollbar->maximum());
			}
			QApplication::processEvents();

			ui->textEdit->insertPlainText("ͼƬ:" + pathJpg + "\n���ڽ������ƶȷ���...\n");
			statusBar()->showMessage(tr("���ڶ�ͼ�����з���..."));

			//���ô�������еڶ���ͼ��·��
			core->getPath2(pathJpg.toStdString());

			//���½���ͼ������Ϣ����
			imageInfo *info = new imageInfo();
			//�Ҷ�ͼ��ֱ��ͼ�Ƚ�
			ui->textEdit->insertPlainText("�Ҷ�ͼֱ��ͼ�Ƚ�...\n");
			info->setClassify_gray_hist(core->doSimilarity_classify_gray_hist());
			//������ͨ����ֱ��ͼ�Ƚ�
			ui->textEdit->insertPlainText("������ͨ����ֱ��ͼ�Ƚ�...\n");
			info->setClassify_hist_with_split(core->doSimilarity_classify_hist_with_split());
			//ƽ��hash
			ui->textEdit->insertPlainText("ƽ��hash...\n");
			info->setClassify_aHash(core->doSimilarity_classify_aHash());
			//��֪hash
			ui->textEdit->insertPlainText("��֪hash...\n");
			info->setClassify_pHash(core->doSimilarity_classify_pHash());
			
			if (ui->checkBox_sift->isChecked() == true) {
				//sift����ƥ��
				ui->textEdit->insertPlainText("sift����ƥ����...\n");
				info->setSift_point(core->doSift());
			}
			if (ui->checkBox_surf->isChecked() == true) {
				//surf����ƥ��
				ui->textEdit->insertPlainText("surf����ƥ����...\n");
				info->setSurf_point(core->doSurf());
			}
			//ÿ��ͼ���·�����浽ͼ����Ϣ������
			info->setPath(pathJpg.toStdString());

			Py_Initialize();//�ر�py����
			imagelibs.push_back(info);//����洢ͼ�����ƶ�����Ķ�̬����
			ui->textEdit->insertPlainText("������ϣ�\n\n");			
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

		//�ı����ƶȷ���ʱ��ͼƬҳ��λ
		nowPage = -1;
		showNextPage();
	}else{
		qDebug() << "��δ����ͼ��";
	}
}

void imageSimilarity::showLastPage() {
	if (nowPage > 0) {
		nowPage--;
		statusBar()->showMessage(tr("��ҳ��:") + QString::number(pageNum, 10) + tr("/") + tr("��ǰҳ:") + QString::number(nowPage + 1, 10));
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

//չʾ��һҳ
void imageSimilarity::showNextPage() {
	if (nowPage < pageNum-1) {
		nowPage++;
		statusBar()->showMessage(tr("��ҳ��:")+ QString::number(pageNum,10)+tr("/")+tr("��ǰҳ:") + QString::number(nowPage+1, 10));
		ui->textEdit->clear();//�����Ϣ��
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