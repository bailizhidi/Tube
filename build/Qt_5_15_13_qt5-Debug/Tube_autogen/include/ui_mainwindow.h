/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTabWidget *tabWidget_2;
    QWidget *tab_5;
    QListView *listView;
    QPushButton *pushButton_ImportPart;
    QPushButton *pushButton_ImportTool;
    QPushButton *pushButton_ImportCurves;
    QLabel *label_25;
    QSpinBox *spinBox;
    QPushButton *pushButton_Mesh1;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QPushButton *pushButton_extractFace;
    QWidget *tab_6;
    QPushButton *pushButton_AddMaterial;
    QWidget *tab_7;
    QTabWidget *tabWidget_4;
    QWidget *tab_4;
    QWidget *tab_11;
    QLabel *label_28;
    QSpinBox *spinBox_2;
    QPushButton *pushButton_CenterLine;
    QLabel *label_29;
    QLineEdit *lineEdit_LineLength;
    QWidget *tab_2;
    QTabWidget *tabWidget_3;
    QWidget *tab_8;
    QPushButton *pushButton_AddCenterLine;
    QWidget *tab_10;
    QListView *listView_2;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_30;
    QLabel *label_31;
    QLineEdit *lineEdit_BendingLength;
    QLineEdit *lineEdit_LineLength_2;
    QLabel *label_19;
    QLabel *label_32;
    QLineEdit *lineEdit_DeviationMax;
    QLineEdit *lineEdit_DeviationMean;
    QLabel *label_33;
    QPushButton *pushButton_BendingLine;
    QWidget *tab_3;
    QTabWidget *tabWidget_5;
    QWidget *tab_17;
    QListView *listView_4;
    QLabel *label_21;
    QLabel *label;
    QLineEdit *lineEdit_Rout;
    QLabel *label_2;
    QLineEdit *lineEdit_Rin;
    QLabel *label_3;
    QLineEdit *lineEdit_Length;
    QLabel *label_22;
    QLabel *label_11;
    QLineEdit *lineEdit_SleeveThickness1;
    QLabel *label_12;
    QLineEdit *lineEdit_SleeveLength1;
    QLabel *label_6;
    QLineEdit *lineEdit_RotaryPos;
    QLabel *label_23;
    QLabel *label_4;
    QLineEdit *lineEdit_SleeveThickness;
    QLabel *label_5;
    QLineEdit *lineEdit_SleeveLength;
    QLabel *label_7;
    QLineEdit *lineEdit_FixedPos;
    QLabel *label_24;
    QLabel *label_8;
    QLineEdit *lineEdit_ArcR;
    QLabel *label_9;
    QLineEdit *lineEdit_ArcThickness;
    QLabel *label_10;
    QLineEdit *lineEdit_ArcAngle;
    QPushButton *pushButton_confirm;
    QWidget *tab_12;
    QListView *listView_3;
    QLabel *label_34;
    QPushButton *pushButton_Step;
    QLabel *label_20;
    QLineEdit *init_step;
    QLabel *label_35;
    QLabel *label_38;
    QLabel *label_39;
    QLineEdit *all_step;
    QLineEdit *min_step;
    QLineEdit *max_step;
    QWidget *tab_13;
    QPushButton *pushButton_Inp;
    QWidget *tab_14;
    QWidget *widget;
    QTableWidget *tableWidget;
    QMdiArea *mdiArea;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(984, 799);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(119, 118, 123);"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMinimumSize(QSize(336, 0));
        tabWidget->setMaximumSize(QSize(336, 16777215));
        tabWidget->setFocusPolicy(Qt::NoFocus);
        tabWidget->setStyleSheet(QString::fromUtf8("QTabBar::tab {\n"
"	background-color: rgb(94, 92, 100);\n"
"	color: rgb(0, 0, 0);\n"
"}\n"
"QTabBar::tab:selected {\n"
"	background-color: rgb(222, 221, 218);\n"
"    color: #303030; \n"
"}\n"
"QTabWidget::pane { border: 0; }"));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setElideMode(Qt::ElideNone);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(false);
        tabWidget->setTabBarAutoHide(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget_2 = new QTabWidget(tab);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(0, 0, 336, 701));
        tabWidget_2->setMinimumSize(QSize(336, 0));
        tabWidget_2->setMaximumSize(QSize(336, 16777215));
        tabWidget_2->setStyleSheet(QString::fromUtf8("QTabBar::tab {\n"
"	background-color: rgb(154, 153, 150);\n"
"	color: rgb(0, 0, 0);\n"
"}\n"
"QTabBar::tab:selected {\n"
"	background-color: rgb(192, 191, 188);\n"
"}\n"
"QTabWidget::pane { border: 0; }"));
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        listView = new QListView(tab_5);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(0, 0, 331, 361));
        listView->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);"));
        pushButton_ImportPart = new QPushButton(tab_5);
        pushButton_ImportPart->setObjectName(QString::fromUtf8("pushButton_ImportPart"));
        pushButton_ImportPart->setGeometry(QRect(50, 120, 231, 41));
        pushButton_ImportPart->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        pushButton_ImportTool = new QPushButton(tab_5);
        pushButton_ImportTool->setObjectName(QString::fromUtf8("pushButton_ImportTool"));
        pushButton_ImportTool->setGeometry(QRect(50, 230, 231, 41));
        pushButton_ImportTool->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        pushButton_ImportCurves = new QPushButton(tab_5);
        pushButton_ImportCurves->setObjectName(QString::fromUtf8("pushButton_ImportCurves"));
        pushButton_ImportCurves->setGeometry(QRect(50, 310, 231, 41));
        pushButton_ImportCurves->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_25 = new QLabel(tab_5);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(60, 30, 76, 25));
        label_25->setMinimumSize(QSize(76, 25));
        label_25->setMaximumSize(QSize(75, 25));
        label_25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(36, 31, 49);"));
        spinBox = new QSpinBox(tab_5);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(160, 30, 75, 25));
        spinBox->setMinimumSize(QSize(75, 25));
        spinBox->setMaximumSize(QSize(75, 25));
        spinBox->setStyleSheet(QString::fromUtf8("padding-left: 3px;\n"
"color: rgb(255, 255, 255);"));
        pushButton_Mesh1 = new QPushButton(tab_5);
        pushButton_Mesh1->setObjectName(QString::fromUtf8("pushButton_Mesh1"));
        pushButton_Mesh1->setGeometry(QRect(250, 60, 75, 25));
        pushButton_Mesh1->setMinimumSize(QSize(75, 25));
        pushButton_Mesh1->setMaximumSize(QSize(75, 25));
        pushButton_Mesh1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_13 = new QLabel(tab_5);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(0, 0, 331, 20));
        label_13->setStyleSheet(QString::fromUtf8("background-color: rgb(119, 118, 123);\n"
"color: rgb(255, 255, 255);"));
        label_14 = new QLabel(tab_5);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(0, 90, 331, 20));
        label_14->setStyleSheet(QString::fromUtf8("background-color: rgb(119, 118, 123);\n"
"color: rgb(255, 255, 255);"));
        label_15 = new QLabel(tab_5);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(0, 200, 331, 20));
        label_15->setStyleSheet(QString::fromUtf8("background-color: rgb(119, 118, 123);\n"
"color: rgb(255, 255, 255);"));
        label_16 = new QLabel(tab_5);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(0, 280, 331, 20));
        label_16->setStyleSheet(QString::fromUtf8("background-color: rgb(119, 118, 123);\n"
"color: rgb(255, 255, 255);"));
        pushButton_extractFace = new QPushButton(tab_5);
        pushButton_extractFace->setObjectName(QString::fromUtf8("pushButton_extractFace"));
        pushButton_extractFace->setGeometry(QRect(250, 170, 80, 25));
        pushButton_extractFace->setMinimumSize(QSize(80, 25));
        pushButton_extractFace->setMaximumSize(QSize(80, 25));
        pushButton_extractFace->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        tabWidget_2->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        pushButton_AddMaterial = new QPushButton(tab_6);
        pushButton_AddMaterial->setObjectName(QString::fromUtf8("pushButton_AddMaterial"));
        pushButton_AddMaterial->setGeometry(QRect(10, 10, 311, 51));
        pushButton_AddMaterial->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        tabWidget_2->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QString::fromUtf8("tab_7"));
        tabWidget_4 = new QTabWidget(tab_7);
        tabWidget_4->setObjectName(QString::fromUtf8("tabWidget_4"));
        tabWidget_4->setGeometry(QRect(0, 10, 336, 336));
        tabWidget_4->setMinimumSize(QSize(336, 336));
        tabWidget_4->setMaximumSize(QSize(336, 336));
        tabWidget_4->setContextMenuPolicy(Qt::PreventContextMenu);
        tabWidget_4->setAutoFillBackground(false);
        tabWidget_4->setStyleSheet(QString::fromUtf8("background-color: rgb(222, 221, 218);"));
        tabWidget_4->setTabPosition(QTabWidget::North);
        tabWidget_4->setTabShape(QTabWidget::Rounded);
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        tabWidget_4->addTab(tab_4, QString());
        tab_11 = new QWidget();
        tab_11->setObjectName(QString::fromUtf8("tab_11"));
        tabWidget_4->addTab(tab_11, QString());
        label_28 = new QLabel(tab_7);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(100, 380, 50, 25));
        label_28->setMinimumSize(QSize(50, 25));
        label_28->setMaximumSize(QSize(50, 25));
        label_28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(119, 118, 123);"));
        spinBox_2 = new QSpinBox(tab_7);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setGeometry(QRect(150, 380, 75, 25));
        spinBox_2->setMinimumSize(QSize(75, 25));
        spinBox_2->setMaximumSize(QSize(75, 25));
        spinBox_2->setStyleSheet(QString::fromUtf8("padding-left: 3px;\n"
"color: rgb(255, 255, 255);"));
        pushButton_CenterLine = new QPushButton(tab_7);
        pushButton_CenterLine->setObjectName(QString::fromUtf8("pushButton_CenterLine"));
        pushButton_CenterLine->setGeometry(QRect(260, 400, 75, 25));
        pushButton_CenterLine->setMinimumSize(QSize(75, 25));
        pushButton_CenterLine->setMaximumSize(QSize(75, 25));
        pushButton_CenterLine->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_29 = new QLabel(tab_7);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(100, 350, 50, 25));
        label_29->setMinimumSize(QSize(50, 25));
        label_29->setMaximumSize(QSize(50, 25));
        label_29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(119, 118, 123);"));
        lineEdit_LineLength = new QLineEdit(tab_7);
        lineEdit_LineLength->setObjectName(QString::fromUtf8("lineEdit_LineLength"));
        lineEdit_LineLength->setGeometry(QRect(150, 350, 75, 25));
        lineEdit_LineLength->setMinimumSize(QSize(75, 25));
        lineEdit_LineLength->setMaximumSize(QSize(75, 25));
        tabWidget_2->addTab(tab_7, QString());
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget_3 = new QTabWidget(tab_2);
        tabWidget_3->setObjectName(QString::fromUtf8("tabWidget_3"));
        tabWidget_3->setGeometry(QRect(0, 0, 336, 701));
        tabWidget_3->setMinimumSize(QSize(336, 0));
        tabWidget_3->setMaximumSize(QSize(336, 16777215));
        tabWidget_3->setStyleSheet(QString::fromUtf8("QTabBar::tab {\n"
"	background-color: rgb(154, 153, 150);\n"
"	color: rgb(0, 0, 0);\n"
"}\n"
"QTabBar::tab:selected {\n"
"	background-color: rgb(192, 191, 188);\n"
"}\n"
"QTabWidget::pane { border: 0; }"));
        tab_8 = new QWidget();
        tab_8->setObjectName(QString::fromUtf8("tab_8"));
        pushButton_AddCenterLine = new QPushButton(tab_8);
        pushButton_AddCenterLine->setObjectName(QString::fromUtf8("pushButton_AddCenterLine"));
        pushButton_AddCenterLine->setGeometry(QRect(10, 10, 311, 51));
        pushButton_AddCenterLine->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        tabWidget_3->addTab(tab_8, QString());
        tab_10 = new QWidget();
        tab_10->setObjectName(QString::fromUtf8("tab_10"));
        listView_2 = new QListView(tab_10);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setGeometry(QRect(0, 0, 331, 171));
        listView_2->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);"));
        label_17 = new QLabel(tab_10);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(0, 0, 331, 20));
        label_17->setStyleSheet(QString::fromUtf8("background-color: rgb(119, 118, 123);\n"
"color: rgb(255, 255, 255);"));
        label_18 = new QLabel(tab_10);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(0, 170, 331, 20));
        label_18->setStyleSheet(QString::fromUtf8("background-color: rgb(119, 118, 123);\n"
"color: rgb(255, 255, 255);"));
        label_30 = new QLabel(tab_10);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(60, 200, 80, 25));
        label_30->setMinimumSize(QSize(80, 25));
        label_30->setMaximumSize(QSize(80, 25));
        label_30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(119, 118, 123);"));
        label_31 = new QLabel(tab_10);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(60, 230, 80, 25));
        label_31->setMinimumSize(QSize(80, 25));
        label_31->setMaximumSize(QSize(80, 25));
        label_31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(119, 118, 123);"));
        lineEdit_BendingLength = new QLineEdit(tab_10);
        lineEdit_BendingLength->setObjectName(QString::fromUtf8("lineEdit_BendingLength"));
        lineEdit_BendingLength->setGeometry(QRect(150, 200, 75, 25));
        lineEdit_BendingLength->setMinimumSize(QSize(75, 25));
        lineEdit_BendingLength->setMaximumSize(QSize(75, 25));
        lineEdit_BendingLength->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lineEdit_LineLength_2 = new QLineEdit(tab_10);
        lineEdit_LineLength_2->setObjectName(QString::fromUtf8("lineEdit_LineLength_2"));
        lineEdit_LineLength_2->setGeometry(QRect(150, 230, 75, 25));
        lineEdit_LineLength_2->setMinimumSize(QSize(75, 25));
        lineEdit_LineLength_2->setMaximumSize(QSize(75, 25));
        lineEdit_LineLength_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_19 = new QLabel(tab_10);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(0, 260, 331, 20));
        label_19->setStyleSheet(QString::fromUtf8("background-color: rgb(119, 118, 123);\n"
"color: rgb(255, 255, 255);"));
        label_32 = new QLabel(tab_10);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(90, 290, 55, 25));
        label_32->setMinimumSize(QSize(55, 25));
        label_32->setMaximumSize(QSize(55, 25));
        label_32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(119, 118, 123);"));
        lineEdit_DeviationMax = new QLineEdit(tab_10);
        lineEdit_DeviationMax->setObjectName(QString::fromUtf8("lineEdit_DeviationMax"));
        lineEdit_DeviationMax->setGeometry(QRect(150, 290, 75, 25));
        lineEdit_DeviationMax->setMinimumSize(QSize(75, 25));
        lineEdit_DeviationMax->setMaximumSize(QSize(75, 25));
        lineEdit_DeviationMax->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lineEdit_DeviationMean = new QLineEdit(tab_10);
        lineEdit_DeviationMean->setObjectName(QString::fromUtf8("lineEdit_DeviationMean"));
        lineEdit_DeviationMean->setGeometry(QRect(150, 320, 75, 25));
        lineEdit_DeviationMean->setMinimumSize(QSize(75, 25));
        lineEdit_DeviationMean->setMaximumSize(QSize(75, 25));
        lineEdit_DeviationMean->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_33 = new QLabel(tab_10);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(90, 320, 55, 25));
        label_33->setMinimumSize(QSize(55, 25));
        label_33->setMaximumSize(QSize(55, 25));
        label_33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(119, 118, 123);"));
        pushButton_BendingLine = new QPushButton(tab_10);
        pushButton_BendingLine->setObjectName(QString::fromUtf8("pushButton_BendingLine"));
        pushButton_BendingLine->setGeometry(QRect(260, 350, 75, 25));
        pushButton_BendingLine->setMinimumSize(QSize(75, 25));
        pushButton_BendingLine->setMaximumSize(QSize(75, 25));
        pushButton_BendingLine->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        tabWidget_3->addTab(tab_10, QString());
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tabWidget_5 = new QTabWidget(tab_3);
        tabWidget_5->setObjectName(QString::fromUtf8("tabWidget_5"));
        tabWidget_5->setGeometry(QRect(0, 0, 336, 701));
        tabWidget_5->setMinimumSize(QSize(336, 0));
        tabWidget_5->setMaximumSize(QSize(336, 16777215));
        tabWidget_5->setStyleSheet(QString::fromUtf8("QTabBar::tab {\n"
"	background-color: rgb(154, 153, 150);\n"
"	color: rgb(0, 0, 0);\n"
"}\n"
"QTabBar::tab:selected {\n"
"	background-color: rgb(192, 191, 188);\n"
"}\n"
"QTabWidget::pane { border: 0; }"));
        tab_17 = new QWidget();
        tab_17->setObjectName(QString::fromUtf8("tab_17"));
        listView_4 = new QListView(tab_17);
        listView_4->setObjectName(QString::fromUtf8("listView_4"));
        listView_4->setGeometry(QRect(0, 0, 331, 511));
        listView_4->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);"));
        label_21 = new QLabel(tab_17);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(0, 0, 331, 20));
        label_21->setStyleSheet(QString::fromUtf8("background-color: rgb(119, 118, 123);\n"
"color: rgb(255, 255, 255);"));
        label = new QLabel(tab_17);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 30, 60, 25));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        lineEdit_Rout = new QLineEdit(tab_17);
        lineEdit_Rout->setObjectName(QString::fromUtf8("lineEdit_Rout"));
        lineEdit_Rout->setGeometry(QRect(70, 30, 100, 25));
        lineEdit_Rout->setMinimumSize(QSize(100, 25));
        lineEdit_Rout->setMaximumSize(QSize(100, 25));
        lineEdit_Rout->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_2 = new QLabel(tab_17);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 60, 60, 25));
        label_2->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        lineEdit_Rin = new QLineEdit(tab_17);
        lineEdit_Rin->setObjectName(QString::fromUtf8("lineEdit_Rin"));
        lineEdit_Rin->setGeometry(QRect(70, 60, 100, 25));
        lineEdit_Rin->setMinimumSize(QSize(100, 25));
        lineEdit_Rin->setMaximumSize(QSize(100, 25));
        lineEdit_Rin->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_3 = new QLabel(tab_17);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 90, 45, 25));
        label_3->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        lineEdit_Length = new QLineEdit(tab_17);
        lineEdit_Length->setObjectName(QString::fromUtf8("lineEdit_Length"));
        lineEdit_Length->setGeometry(QRect(70, 90, 100, 25));
        lineEdit_Length->setMinimumSize(QSize(100, 25));
        lineEdit_Length->setMaximumSize(QSize(100, 25));
        lineEdit_Length->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_22 = new QLabel(tab_17);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(0, 120, 331, 20));
        label_22->setStyleSheet(QString::fromUtf8("background-color: rgb(119, 118, 123);\n"
"color: rgb(255, 255, 255);"));
        label_11 = new QLabel(tab_17);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 150, 45, 25));
        label_11->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        lineEdit_SleeveThickness1 = new QLineEdit(tab_17);
        lineEdit_SleeveThickness1->setObjectName(QString::fromUtf8("lineEdit_SleeveThickness1"));
        lineEdit_SleeveThickness1->setGeometry(QRect(70, 150, 100, 25));
        lineEdit_SleeveThickness1->setMinimumSize(QSize(100, 25));
        lineEdit_SleeveThickness1->setMaximumSize(QSize(100, 25));
        lineEdit_SleeveThickness1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_12 = new QLabel(tab_17);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 180, 45, 25));
        label_12->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        lineEdit_SleeveLength1 = new QLineEdit(tab_17);
        lineEdit_SleeveLength1->setObjectName(QString::fromUtf8("lineEdit_SleeveLength1"));
        lineEdit_SleeveLength1->setGeometry(QRect(70, 180, 100, 25));
        lineEdit_SleeveLength1->setMinimumSize(QSize(100, 25));
        lineEdit_SleeveLength1->setMaximumSize(QSize(100, 25));
        lineEdit_SleeveLength1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_6 = new QLabel(tab_17);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 210, 45, 25));
        label_6->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        lineEdit_RotaryPos = new QLineEdit(tab_17);
        lineEdit_RotaryPos->setObjectName(QString::fromUtf8("lineEdit_RotaryPos"));
        lineEdit_RotaryPos->setGeometry(QRect(70, 210, 100, 25));
        lineEdit_RotaryPos->setMinimumSize(QSize(100, 25));
        lineEdit_RotaryPos->setMaximumSize(QSize(100, 25));
        lineEdit_RotaryPos->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_23 = new QLabel(tab_17);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(0, 240, 331, 20));
        label_23->setStyleSheet(QString::fromUtf8("background-color: rgb(119, 118, 123);\n"
"color: rgb(255, 255, 255);"));
        label_4 = new QLabel(tab_17);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 270, 45, 25));
        label_4->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        lineEdit_SleeveThickness = new QLineEdit(tab_17);
        lineEdit_SleeveThickness->setObjectName(QString::fromUtf8("lineEdit_SleeveThickness"));
        lineEdit_SleeveThickness->setGeometry(QRect(70, 270, 100, 25));
        lineEdit_SleeveThickness->setMinimumSize(QSize(100, 25));
        lineEdit_SleeveThickness->setMaximumSize(QSize(100, 25));
        lineEdit_SleeveThickness->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_5 = new QLabel(tab_17);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 300, 45, 25));
        label_5->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        lineEdit_SleeveLength = new QLineEdit(tab_17);
        lineEdit_SleeveLength->setObjectName(QString::fromUtf8("lineEdit_SleeveLength"));
        lineEdit_SleeveLength->setGeometry(QRect(70, 300, 100, 25));
        lineEdit_SleeveLength->setMinimumSize(QSize(100, 25));
        lineEdit_SleeveLength->setMaximumSize(QSize(100, 25));
        lineEdit_SleeveLength->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_7 = new QLabel(tab_17);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 330, 45, 25));
        label_7->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        lineEdit_FixedPos = new QLineEdit(tab_17);
        lineEdit_FixedPos->setObjectName(QString::fromUtf8("lineEdit_FixedPos"));
        lineEdit_FixedPos->setGeometry(QRect(70, 330, 100, 25));
        lineEdit_FixedPos->setMinimumSize(QSize(100, 25));
        lineEdit_FixedPos->setMaximumSize(QSize(100, 25));
        lineEdit_FixedPos->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_24 = new QLabel(tab_17);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(0, 360, 331, 20));
        label_24->setStyleSheet(QString::fromUtf8("background-color: rgb(119, 118, 123);\n"
"color: rgb(255, 255, 255);"));
        label_8 = new QLabel(tab_17);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 390, 45, 25));
        label_8->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        lineEdit_ArcR = new QLineEdit(tab_17);
        lineEdit_ArcR->setObjectName(QString::fromUtf8("lineEdit_ArcR"));
        lineEdit_ArcR->setGeometry(QRect(70, 390, 100, 25));
        lineEdit_ArcR->setMinimumSize(QSize(100, 25));
        lineEdit_ArcR->setMaximumSize(QSize(100, 25));
        lineEdit_ArcR->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_9 = new QLabel(tab_17);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 420, 45, 25));
        label_9->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        lineEdit_ArcThickness = new QLineEdit(tab_17);
        lineEdit_ArcThickness->setObjectName(QString::fromUtf8("lineEdit_ArcThickness"));
        lineEdit_ArcThickness->setGeometry(QRect(70, 420, 100, 25));
        lineEdit_ArcThickness->setMinimumSize(QSize(100, 25));
        lineEdit_ArcThickness->setMaximumSize(QSize(100, 25));
        lineEdit_ArcThickness->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_10 = new QLabel(tab_17);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 450, 45, 25));
        label_10->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        lineEdit_ArcAngle = new QLineEdit(tab_17);
        lineEdit_ArcAngle->setObjectName(QString::fromUtf8("lineEdit_ArcAngle"));
        lineEdit_ArcAngle->setGeometry(QRect(70, 450, 100, 25));
        lineEdit_ArcAngle->setMinimumSize(QSize(100, 25));
        lineEdit_ArcAngle->setMaximumSize(QSize(100, 25));
        lineEdit_ArcAngle->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        pushButton_confirm = new QPushButton(tab_17);
        pushButton_confirm->setObjectName(QString::fromUtf8("pushButton_confirm"));
        pushButton_confirm->setGeometry(QRect(250, 480, 75, 25));
        pushButton_confirm->setMinimumSize(QSize(75, 25));
        pushButton_confirm->setMaximumSize(QSize(75, 25));
        pushButton_confirm->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        tabWidget_5->addTab(tab_17, QString());
        tab_12 = new QWidget();
        tab_12->setObjectName(QString::fromUtf8("tab_12"));
        listView_3 = new QListView(tab_12);
        listView_3->setObjectName(QString::fromUtf8("listView_3"));
        listView_3->setGeometry(QRect(0, 0, 331, 191));
        listView_3->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 31, 49);"));
        label_34 = new QLabel(tab_12);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(60, 30, 50, 25));
        label_34->setMinimumSize(QSize(50, 25));
        label_34->setMaximumSize(QSize(50, 25));
        label_34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(36, 31, 49);"));
        pushButton_Step = new QPushButton(tab_12);
        pushButton_Step->setObjectName(QString::fromUtf8("pushButton_Step"));
        pushButton_Step->setGeometry(QRect(250, 160, 75, 25));
        pushButton_Step->setMinimumSize(QSize(75, 25));
        pushButton_Step->setMaximumSize(QSize(75, 25));
        pushButton_Step->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_20 = new QLabel(tab_12);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(0, 0, 331, 20));
        label_20->setStyleSheet(QString::fromUtf8("background-color: rgb(119, 118, 123);\n"
"color: rgb(255, 255, 255);"));
        init_step = new QLineEdit(tab_12);
        init_step->setObjectName(QString::fromUtf8("init_step"));
        init_step->setGeometry(QRect(110, 30, 75, 25));
        init_step->setMinimumSize(QSize(75, 25));
        init_step->setMaximumSize(QSize(75, 25));
        init_step->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_35 = new QLabel(tab_12);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setGeometry(QRect(60, 60, 50, 25));
        label_35->setMinimumSize(QSize(50, 25));
        label_35->setMaximumSize(QSize(50, 25));
        label_35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(36, 31, 49);"));
        label_38 = new QLabel(tab_12);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setGeometry(QRect(60, 90, 50, 25));
        label_38->setMinimumSize(QSize(50, 25));
        label_38->setMaximumSize(QSize(50, 25));
        label_38->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(36, 31, 49);"));
        label_39 = new QLabel(tab_12);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setGeometry(QRect(60, 120, 50, 25));
        label_39->setMinimumSize(QSize(50, 25));
        label_39->setMaximumSize(QSize(50, 25));
        label_39->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(36, 31, 49);"));
        all_step = new QLineEdit(tab_12);
        all_step->setObjectName(QString::fromUtf8("all_step"));
        all_step->setGeometry(QRect(110, 60, 75, 25));
        all_step->setMinimumSize(QSize(75, 25));
        all_step->setMaximumSize(QSize(75, 25));
        all_step->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        min_step = new QLineEdit(tab_12);
        min_step->setObjectName(QString::fromUtf8("min_step"));
        min_step->setGeometry(QRect(110, 90, 75, 25));
        min_step->setMinimumSize(QSize(75, 25));
        min_step->setMaximumSize(QSize(75, 25));
        min_step->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        max_step = new QLineEdit(tab_12);
        max_step->setObjectName(QString::fromUtf8("max_step"));
        max_step->setGeometry(QRect(110, 120, 75, 25));
        max_step->setMinimumSize(QSize(75, 25));
        max_step->setMaximumSize(QSize(75, 25));
        max_step->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        tabWidget_5->addTab(tab_12, QString());
        tab_13 = new QWidget();
        tab_13->setObjectName(QString::fromUtf8("tab_13"));
        pushButton_Inp = new QPushButton(tab_13);
        pushButton_Inp->setObjectName(QString::fromUtf8("pushButton_Inp"));
        pushButton_Inp->setGeometry(QRect(10, 10, 311, 51));
        pushButton_Inp->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        tabWidget_5->addTab(tab_13, QString());
        tab_14 = new QWidget();
        tab_14->setObjectName(QString::fromUtf8("tab_14"));
        tabWidget_5->addTab(tab_14, QString());
        tabWidget->addTab(tab_3, QString());
        widget = new QWidget();
        widget->setObjectName(QString::fromUtf8("widget"));
        tabWidget->addTab(widget, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 2, 1);

        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setMaximumSize(QSize(16777215, 100));
        tableWidget->setFrameShape(QFrame::NoFrame);

        gridLayout->addWidget(tableWidget, 0, 1, 1, 1);

        mdiArea = new QMdiArea(centralwidget);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
        mdiArea->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        QBrush brush(QColor(246, 245, 244, 255));
        brush.setStyle(Qt::SolidPattern);
        mdiArea->setBackground(brush);

        gridLayout->addWidget(mdiArea, 1, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 984, 27));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);
        tabWidget_4->setCurrentIndex(0);
        tabWidget_3->setCurrentIndex(1);
        tabWidget_5->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "AVIC Tube", nullptr));
        pushButton_ImportPart->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\205\245\351\203\250\344\273\266\347\232\204\345\207\240\344\275\225\345\275\242\347\212\266", nullptr));
        pushButton_ImportTool->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\205\245\345\267\245\345\205\267\346\210\226\351\242\235\345\244\226\347\232\204\345\207\240\344\275\225\345\275\242\347\212\266", nullptr));
        pushButton_ImportCurves->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\205\245\346\233\262\347\272\277\346\210\226\347\202\271", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "\347\275\221\346\240\274\345\244\247\345\260\217\357\274\232", nullptr));
        pushButton_Mesh1->setText(QCoreApplication::translate("MainWindow", "\347\241\256\345\256\232", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "\347\275\221\346\240\274\350\256\276\347\275\256", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "\344\272\247\345\223\201\345\207\240\344\275\225\345\275\242\347\212\266", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "\345\267\245\345\205\267\350\241\250\351\235\242\344\270\216\351\231\204\345\212\240\345\207\240\344\275\225\345\275\242\347\212\266", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "\346\233\262\347\272\277\346\210\226\347\202\271", nullptr));
        pushButton_extractFace->setText(QCoreApplication::translate("MainWindow", "\346\217\220\345\217\226\345\244\226\350\241\250\351\235\242", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_5), QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213\345\257\274\345\205\245", nullptr));
        pushButton_AddMaterial->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\346\235\220\346\226\231", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_6), QCoreApplication::translate("MainWindow", "\346\235\220\346\226\231\351\200\211\346\213\251", nullptr));
        tabWidget_4->setTabText(tabWidget_4->indexOf(tab_4), QCoreApplication::translate("MainWindow", "\345\272\224\345\217\230", nullptr));
        tabWidget_4->setTabText(tabWidget_4->indexOf(tab_11), QCoreApplication::translate("MainWindow", "\345\221\250\351\225\277", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", "\347\233\264\345\276\204\357\274\232", nullptr));
        pushButton_CenterLine->setText(QCoreApplication::translate("MainWindow", "\347\241\256\345\256\232", nullptr));
        label_29->setText(QCoreApplication::translate("MainWindow", "\351\225\277\345\272\246\357\274\232", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_7), QCoreApplication::translate("MainWindow", "\344\270\255\347\272\277\346\217\220\345\217\226", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213\350\256\276\347\275\256", nullptr));
        pushButton_AddCenterLine->setText(QCoreApplication::translate("MainWindow", "\346\277\200\346\264\273\344\270\255\345\277\203\347\272\277\344\275\234\344\270\272\345\275\223\345\211\215\351\233\266\344\273\266\345\207\240\344\275\225\345\275\242\347\212\266\347\232\204\344\270\255\345\277\203\347\272\277", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_8), QCoreApplication::translate("MainWindow", "\344\270\255\345\277\203\347\272\277", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "\345\274\257\346\233\262\345\217\202\346\225\260", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "\344\270\255\345\277\203\347\272\277\345\222\214\345\274\257\346\233\262\347\272\277\347\232\204\351\225\277\345\272\246", nullptr));
        label_30->setText(QCoreApplication::translate("MainWindow", "\345\274\257\346\233\262\347\272\277\351\225\277\345\272\246\357\274\232", nullptr));
        label_31->setText(QCoreApplication::translate("MainWindow", "\344\270\255\345\277\203\347\272\277\351\225\277\345\272\246\357\274\232", nullptr));
        lineEdit_BendingLength->setText(QString());
        label_19->setText(QCoreApplication::translate("MainWindow", "\344\270\255\345\277\203\347\272\277\344\270\216\345\274\257\346\233\262\347\272\277\344\271\213\351\227\264\347\232\204\345\201\217\345\267\256", nullptr));
        label_32->setText(QCoreApplication::translate("MainWindow", "\346\234\200\345\244\247\345\200\274\357\274\232", nullptr));
        label_33->setText(QCoreApplication::translate("MainWindow", "\345\235\207\345\214\200\345\200\274\357\274\232", nullptr));
        pushButton_BendingLine->setText(QCoreApplication::translate("MainWindow", "\347\241\256\345\256\232", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_10), QCoreApplication::translate("MainWindow", "\345\274\257\346\233\262\347\272\277", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\345\267\245\350\211\272\350\256\276\347\275\256", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "\347\256\241\344\275\223", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\345\244\226\345\215\212\345\276\204\357\274\232", nullptr));
        lineEdit_Rout->setText(QCoreApplication::translate("MainWindow", "10", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\345\206\205\345\215\212\345\276\204\357\274\232", nullptr));
        lineEdit_Rin->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\351\225\277\345\272\246\357\274\232", nullptr));
        lineEdit_Length->setText(QCoreApplication::translate("MainWindow", "200", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "\346\227\213\350\275\254\345\245\227\347\255\222", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "\345\216\232\345\272\246\357\274\232", nullptr));
        lineEdit_SleeveThickness1->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "\351\225\277\345\272\246\357\274\232", nullptr));
        lineEdit_SleeveLength1->setText(QCoreApplication::translate("MainWindow", "20", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\344\275\215\347\275\256\357\274\232", nullptr));
        lineEdit_RotaryPos->setText(QCoreApplication::translate("MainWindow", "30", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "\345\233\272\345\256\232\345\245\227\347\255\222", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\345\216\232\345\272\246\357\274\232", nullptr));
        lineEdit_SleeveThickness->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\351\225\277\345\272\246\357\274\232", nullptr));
        lineEdit_SleeveLength->setText(QCoreApplication::translate("MainWindow", "50", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\344\275\215\347\275\256\357\274\232", nullptr));
        lineEdit_FixedPos->setText(QCoreApplication::translate("MainWindow", "150", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "\345\215\212\345\234\206\345\274\247\345\245\227", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\345\215\212\345\276\204\357\274\232", nullptr));
        lineEdit_ArcR->setText(QCoreApplication::translate("MainWindow", "50", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\345\216\232\345\272\246\357\274\232", nullptr));
        lineEdit_ArcThickness->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\345\274\247\345\272\246\357\274\232", nullptr));
        lineEdit_ArcAngle->setText(QCoreApplication::translate("MainWindow", "1.57", nullptr));
        pushButton_confirm->setText(QCoreApplication::translate("MainWindow", "\347\241\256\345\256\232", nullptr));
        tabWidget_5->setTabText(tabWidget_5->indexOf(tab_17), QCoreApplication::translate("MainWindow", "\345\274\257\347\256\241\345\273\272\346\250\241", nullptr));
        label_34->setText(QCoreApplication::translate("MainWindow", "\345\210\235\345\247\213\357\274\232", nullptr));
        pushButton_Step->setText(QCoreApplication::translate("MainWindow", "\347\241\256\345\256\232", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "\346\227\266\351\227\264\346\255\245\351\225\277\350\256\276\347\275\256", nullptr));
        init_step->setText(QCoreApplication::translate("MainWindow", "1e-2", nullptr));
        label_35->setText(QCoreApplication::translate("MainWindow", "\346\200\273\344\275\223\357\274\232", nullptr));
        label_38->setText(QCoreApplication::translate("MainWindow", "\346\234\200\345\260\217\357\274\232", nullptr));
        label_39->setText(QCoreApplication::translate("MainWindow", "\346\234\200\345\244\247\357\274\232", nullptr));
        all_step->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        min_step->setText(QCoreApplication::translate("MainWindow", "1e-8", nullptr));
        max_step->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        tabWidget_5->setTabText(tabWidget_5->indexOf(tab_12), QCoreApplication::translate("MainWindow", "\346\227\266\351\227\264\346\255\245\351\225\277", nullptr));
        pushButton_Inp->setText(QCoreApplication::translate("MainWindow", "\347\224\237\346\210\220inp\346\226\207\344\273\266", nullptr));
        tabWidget_5->setTabText(tabWidget_5->indexOf(tab_13), QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\347\224\237\346\210\220", nullptr));
        tabWidget_5->setTabText(tabWidget_5->indexOf(tab_14), QCoreApplication::translate("MainWindow", "\350\256\241\347\256\227\345\274\200\345\247\213", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "\346\217\220\344\272\244\350\256\241\347\256\227", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(widget), QCoreApplication::translate("MainWindow", "\347\273\223\346\236\234\345\210\206\346\236\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
