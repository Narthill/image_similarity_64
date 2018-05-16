#include "imageSimilarity.h"
#include <QtWidgets/QApplication>
#include<QTextCodec>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	imageSimilarity w;
	w.show();
	return a.exec();
}
