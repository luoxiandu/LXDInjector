/********************************************************************************
** Form generated from reading UI file 'LXDInjector.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LXDINJECTOR_H
#define UI_LXDINJECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LXDInjectorClass
{
public:
    QAction *actiontuichu;
    QAction *actionFile;
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionRefresh;
    QAction *actionHelp;
    QWidget *centralWidget;
    QComboBox *lstCheats;
    QLabel *lbAvaliableCheats;
    QPushButton *btnInject;
    QLabel *lbMemo;
    QPushButton *btnTranslate;
    QMenuBar *menuBar;
    QMenu *menuService;
    QMenu *menuHelp;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *LXDInjectorClass)
    {
        if (LXDInjectorClass->objectName().isEmpty())
            LXDInjectorClass->setObjectName(QString::fromUtf8("LXDInjectorClass"));
        LXDInjectorClass->resize(500, 200);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LXDInjectorClass->sizePolicy().hasHeightForWidth());
        LXDInjectorClass->setSizePolicy(sizePolicy);
        LXDInjectorClass->setMinimumSize(QSize(500, 200));
        LXDInjectorClass->setMaximumSize(QSize(500, 200));
        LXDInjectorClass->setStyleSheet(QString::fromUtf8(""));
        actiontuichu = new QAction(LXDInjectorClass);
        actiontuichu->setObjectName(QString::fromUtf8("actiontuichu"));
        actionFile = new QAction(LXDInjectorClass);
        actionFile->setObjectName(QString::fromUtf8("actionFile"));
        actionExit = new QAction(LXDInjectorClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAbout = new QAction(LXDInjectorClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionRefresh = new QAction(LXDInjectorClass);
        actionRefresh->setObjectName(QString::fromUtf8("actionRefresh"));
        actionHelp = new QAction(LXDInjectorClass);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        centralWidget = new QWidget(LXDInjectorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        lstCheats = new QComboBox(centralWidget);
        lstCheats->setObjectName(QString::fromUtf8("lstCheats"));
        lstCheats->setGeometry(QRect(100, 11, 381, 21));
        lstCheats->setInsertPolicy(QComboBox::InsertAtTop);
        lbAvaliableCheats = new QLabel(centralWidget);
        lbAvaliableCheats->setObjectName(QString::fromUtf8("lbAvaliableCheats"));
        lbAvaliableCheats->setGeometry(QRect(10, 10, 81, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("\346\226\260\345\256\213\344\275\223"));
        font.setPointSize(9);
        lbAvaliableCheats->setFont(font);
        btnInject = new QPushButton(centralWidget);
        btnInject->setObjectName(QString::fromUtf8("btnInject"));
        btnInject->setGeometry(QRect(400, 110, 81, 31));
        lbMemo = new QLabel(centralWidget);
        lbMemo->setObjectName(QString::fromUtf8("lbMemo"));
        lbMemo->setGeometry(QRect(100, 40, 291, 91));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lbMemo->sizePolicy().hasHeightForWidth());
        lbMemo->setSizePolicy(sizePolicy1);
        lbMemo->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        lbMemo->setWordWrap(true);
        btnTranslate = new QPushButton(centralWidget);
        btnTranslate->setObjectName(QString::fromUtf8("btnTranslate"));
        btnTranslate->setGeometry(QRect(400, 70, 81, 31));
        LXDInjectorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(LXDInjectorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 500, 23));
        menuService = new QMenu(menuBar);
        menuService->setObjectName(QString::fromUtf8("menuService"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        LXDInjectorClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(LXDInjectorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setAutoFillBackground(false);
        LXDInjectorClass->setStatusBar(statusBar);

        menuBar->addAction(menuService->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuService->addAction(actionRefresh);
        menuService->addSeparator();
        menuService->addAction(actionExit);
        menuHelp->addAction(actionHelp);
        menuHelp->addAction(actionAbout);

        retranslateUi(LXDInjectorClass);

        btnInject->setDefault(true);


        QMetaObject::connectSlotsByName(LXDInjectorClass);
    } // setupUi

    void retranslateUi(QMainWindow *LXDInjectorClass)
    {
        LXDInjectorClass->setWindowTitle(QApplication::translate("LXDInjectorClass", "\346\264\233\344\273\231\351\203\275", nullptr));
        actiontuichu->setText(QApplication::translate("LXDInjectorClass", "tuichu", nullptr));
        actionFile->setText(QApplication::translate("LXDInjectorClass", "\346\226\207\344\273\266", nullptr));
        actionExit->setText(QApplication::translate("LXDInjectorClass", "\351\200\200\345\207\272", nullptr));
        actionAbout->setText(QApplication::translate("LXDInjectorClass", "\345\205\263\344\272\216", nullptr));
        actionRefresh->setText(QApplication::translate("LXDInjectorClass", "\345\210\267\346\226\260\345\244\226\346\214\202\345\210\227\350\241\250", nullptr));
        actionHelp->setText(QApplication::translate("LXDInjectorClass", "\344\275\277\347\224\250\350\257\264\346\230\216", nullptr));
        lbAvaliableCheats->setText(QApplication::translate("LXDInjectorClass", "\345\275\223\345\211\215\345\217\257\347\224\250\345\244\226\346\214\202", nullptr));
        btnInject->setText(QApplication::translate("LXDInjectorClass", "\346\263\250\345\205\245\357\274\201", nullptr));
        lbMemo->setText(QString());
        btnTranslate->setText(QApplication::translate("LXDInjectorClass", "\347\277\273\350\257\221\345\233\276", nullptr));
        menuService->setTitle(QApplication::translate("LXDInjectorClass", "\346\234\215\345\212\241", nullptr));
        menuHelp->setTitle(QApplication::translate("LXDInjectorClass", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LXDInjectorClass: public Ui_LXDInjectorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LXDINJECTOR_H
