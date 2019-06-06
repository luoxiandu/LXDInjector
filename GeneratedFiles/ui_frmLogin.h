/********************************************************************************
** Form generated from reading UI file 'frmLogin.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMLOGIN_H
#define UI_FRMLOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_frmLogin
{
public:
    QLabel *lbUsername;
    QLabel *lbPassword;
    QLineEdit *lnUsername;
    QLineEdit *lnPassword;
    QPushButton *btnQuit;
    QPushButton *btnJoinus;
    QPushButton *btnLogin;
    QCheckBox *cbRemember;

    void setupUi(QDialog *frmLogin)
    {
        if (frmLogin->objectName().isEmpty())
            frmLogin->setObjectName(QString::fromUtf8("frmLogin"));
        frmLogin->resize(375, 111);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frmLogin->sizePolicy().hasHeightForWidth());
        frmLogin->setSizePolicy(sizePolicy);
        lbUsername = new QLabel(frmLogin);
        lbUsername->setObjectName(QString::fromUtf8("lbUsername"));
        lbUsername->setGeometry(QRect(50, 10, 51, 21));
        lbPassword = new QLabel(frmLogin);
        lbPassword->setObjectName(QString::fromUtf8("lbPassword"));
        lbPassword->setGeometry(QRect(30, 39, 71, 21));
        lnUsername = new QLineEdit(frmLogin);
        lnUsername->setObjectName(QString::fromUtf8("lnUsername"));
        lnUsername->setGeometry(QRect(100, 10, 241, 20));
        lnPassword = new QLineEdit(frmLogin);
        lnPassword->setObjectName(QString::fromUtf8("lnPassword"));
        lnPassword->setGeometry(QRect(100, 40, 241, 20));
        btnQuit = new QPushButton(frmLogin);
        btnQuit->setObjectName(QString::fromUtf8("btnQuit"));
        btnQuit->setGeometry(QRect(80, 70, 75, 31));
        btnQuit->setAutoDefault(false);
        btnJoinus = new QPushButton(frmLogin);
        btnJoinus->setObjectName(QString::fromUtf8("btnJoinus"));
        btnJoinus->setGeometry(QRect(180, 70, 75, 31));
        btnJoinus->setAutoDefault(false);
        btnLogin = new QPushButton(frmLogin);
        btnLogin->setObjectName(QString::fromUtf8("btnLogin"));
        btnLogin->setGeometry(QRect(280, 70, 75, 31));
        cbRemember = new QCheckBox(frmLogin);
        cbRemember->setObjectName(QString::fromUtf8("cbRemember"));
        cbRemember->setGeometry(QRect(10, 80, 61, 16));

        retranslateUi(frmLogin);

        QMetaObject::connectSlotsByName(frmLogin);
    } // setupUi

    void retranslateUi(QDialog *frmLogin)
    {
        frmLogin->setWindowTitle(QApplication::translate("frmLogin", "\347\231\273\345\275\225 - \346\264\233\344\273\231\351\203\275", nullptr));
        lbUsername->setText(QApplication::translate("frmLogin", "QQ\345\217\267\347\240\201", nullptr));
        lbPassword->setText(QApplication::translate("frmLogin", "\346\234\254\347\263\273\347\273\237\345\257\206\347\240\201", nullptr));
        btnQuit->setText(QApplication::translate("frmLogin", "\351\200\200\345\207\272", nullptr));
        btnJoinus->setText(QApplication::translate("frmLogin", "\350\257\225\347\224\250", nullptr));
        btnLogin->setText(QApplication::translate("frmLogin", "\347\231\273\345\275\225", nullptr));
        cbRemember->setText(QApplication::translate("frmLogin", "\350\256\260\344\275\217\346\210\221", nullptr));
    } // retranslateUi

};

namespace Ui {
    class frmLogin: public Ui_frmLogin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMLOGIN_H
