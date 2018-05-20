/********************************************************************************
** Form generated from reading UI file 'imageSimilarity.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGESIMILARITY_H
#define UI_IMAGESIMILARITY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_imageSimilarityClass
{
public:
    QAction *actionOpen;
    QAction *action2;
    QAction *actionSimilar;
    QAction *actionImportLib;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *SrcImgView;
    QFrame *frame;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *comboBox;
    QCheckBox *checkBox_sift;
    QCheckBox *checkBox_surf;
    QSpacerItem *horizontalSpacer_3;
    QTextEdit *textEdit;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *lastPageBtn;
    QPushButton *nextPageBtn;
    QSpacerItem *horizontalSpacer_2;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *imageSimilarityClass)
    {
        if (imageSimilarityClass->objectName().isEmpty())
            imageSimilarityClass->setObjectName(QStringLiteral("imageSimilarityClass"));
        imageSimilarityClass->resize(1127, 791);
        actionOpen = new QAction(imageSimilarityClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        action2 = new QAction(imageSimilarityClass);
        action2->setObjectName(QStringLiteral("action2"));
        actionSimilar = new QAction(imageSimilarityClass);
        actionSimilar->setObjectName(QStringLiteral("actionSimilar"));
        actionImportLib = new QAction(imageSimilarityClass);
        actionImportLib->setObjectName(QStringLiteral("actionImportLib"));
        centralWidget = new QWidget(imageSimilarityClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_5 = new QVBoxLayout(centralWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        SrcImgView = new QLabel(groupBox_2);
        SrcImgView->setObjectName(QStringLiteral("SrcImgView"));
        SrcImgView->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 255)"));

        verticalLayout_2->addWidget(SrcImgView);


        horizontalLayout_3->addWidget(groupBox_2);

        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Plain);
        verticalLayout_3 = new QVBoxLayout(frame);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        comboBox = new QComboBox(frame);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout_4->addWidget(comboBox);

        checkBox_sift = new QCheckBox(frame);
        checkBox_sift->setObjectName(QStringLiteral("checkBox_sift"));

        horizontalLayout_4->addWidget(checkBox_sift);

        checkBox_surf = new QCheckBox(frame);
        checkBox_surf->setObjectName(QStringLiteral("checkBox_surf"));

        horizontalLayout_4->addWidget(checkBox_surf);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(3, 2);

        verticalLayout_3->addLayout(horizontalLayout_4);

        textEdit = new QTextEdit(frame);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        verticalLayout_3->addWidget(textEdit);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 5);

        horizontalLayout_3->addWidget(frame);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 1);

        verticalLayout_4->addLayout(horizontalLayout_3);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 255)"));

        horizontalLayout->addWidget(label);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 255)"));

        horizontalLayout->addWidget(label_2);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 255)"));

        horizontalLayout->addWidget(label_3);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 255)"));

        horizontalLayout->addWidget(label_4);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(2, 1);
        horizontalLayout->setStretch(3, 1);

        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_4->addWidget(groupBox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        lastPageBtn = new QPushButton(centralWidget);
        lastPageBtn->setObjectName(QStringLiteral("lastPageBtn"));

        horizontalLayout_2->addWidget(lastPageBtn);

        nextPageBtn = new QPushButton(centralWidget);
        nextPageBtn->setObjectName(QStringLiteral("nextPageBtn"));

        horizontalLayout_2->addWidget(nextPageBtn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout_2);

        verticalLayout_4->setStretch(0, 4);
        verticalLayout_4->setStretch(1, 4);
        verticalLayout_4->setStretch(2, 1);

        verticalLayout_5->addLayout(verticalLayout_4);

        imageSimilarityClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(imageSimilarityClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1127, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        imageSimilarityClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(imageSimilarityClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        imageSimilarityClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(imageSimilarityClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        imageSimilarityClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(actionOpen);
        menu->addAction(actionImportLib);
        menu_2->addAction(actionSimilar);

        retranslateUi(imageSimilarityClass);

        QMetaObject::connectSlotsByName(imageSimilarityClass);
    } // setupUi

    void retranslateUi(QMainWindow *imageSimilarityClass)
    {
        imageSimilarityClass->setWindowTitle(QApplication::translate("imageSimilarityClass", "imageSimilarity", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("imageSimilarityClass", "\346\211\223\345\274\200\345\276\205\346\257\224\350\276\203\345\233\276\347\211\207", Q_NULLPTR));
        action2->setText(QApplication::translate("imageSimilarityClass", "2", Q_NULLPTR));
        actionSimilar->setText(QApplication::translate("imageSimilarityClass", "\345\233\276\347\211\207\345\272\223\345\244\204\347\220\206", Q_NULLPTR));
        actionImportLib->setText(QApplication::translate("imageSimilarityClass", "\345\257\274\345\205\245\345\233\276\345\203\217\345\272\223", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("imageSimilarityClass", "\345\216\237\345\233\276", Q_NULLPTR));
        SrcImgView->setText(QString());
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("imageSimilarityClass", "\347\201\260\345\272\246\347\233\264\346\226\271\345\233\276\347\232\204\347\233\270\344\274\274\345\272\246", Q_NULLPTR)
         << QApplication::translate("imageSimilarityClass", "\345\210\206\347\246\273\344\270\211\351\200\232\351\201\223\347\233\264\346\226\271\345\233\276\347\232\204\347\233\270\344\274\274\345\272\246", Q_NULLPTR)
         << QApplication::translate("imageSimilarityClass", "\345\271\263\345\235\207\345\223\210\345\270\214", Q_NULLPTR)
         << QApplication::translate("imageSimilarityClass", "\346\204\237\347\237\245\345\223\210\345\270\214", Q_NULLPTR)
         << QApplication::translate("imageSimilarityClass", "sift\347\211\271\345\276\201\345\214\271\351\205\215", Q_NULLPTR)
         << QApplication::translate("imageSimilarityClass", "surf\347\211\271\345\276\201\345\214\271\351\205\215", Q_NULLPTR)
        );
        checkBox_sift->setText(QApplication::translate("imageSimilarityClass", "\346\230\257\345\220\246\345\214\271\351\205\215sift\347\211\271\345\276\201", Q_NULLPTR));
        checkBox_surf->setText(QApplication::translate("imageSimilarityClass", "\346\230\257\345\220\246\345\214\271\351\205\215surf\347\211\271\345\276\201", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("imageSimilarityClass", "\347\233\270\344\274\274\345\233\276", Q_NULLPTR));
        label->setText(QString());
        label_2->setText(QString());
        label_3->setText(QString());
        label_4->setText(QString());
        lastPageBtn->setText(QApplication::translate("imageSimilarityClass", "\344\270\212\344\270\200\351\241\265", Q_NULLPTR));
        nextPageBtn->setText(QApplication::translate("imageSimilarityClass", "\344\270\213\344\270\200\351\241\265", Q_NULLPTR));
        menu->setTitle(QApplication::translate("imageSimilarityClass", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("imageSimilarityClass", "\345\212\237\350\203\275", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class imageSimilarityClass: public Ui_imageSimilarityClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGESIMILARITY_H
