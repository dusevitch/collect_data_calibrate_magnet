/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLCDNumber *mag_sense_x;
    QLCDNumber *mag_sense_y;
    QLCDNumber *mag_sense_z;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *start_sensor;
    QLineEdit *coil_0_current;
    QLabel *label_5;
    QLabel *label_6;
    QLCDNumber *timer_cur;
    QLabel *label_7;
    QPushButton *collect_data;
    QLCDNumber *tracker_base_y;
    QLCDNumber *tracker_base_x;
    QLabel *label_8;
    QLCDNumber *tracker_base_z;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLCDNumber *tracker_wand_y;
    QLCDNumber *tracker_wand_x;
    QLCDNumber *tracker_wand_z;
    QPushButton *start_polaris;
    QPushButton *activate_mag;
    QPushButton *run_calib;
    QPushButton *save_calib;
    QFrame *line;
    QLabel *label_16;
    QLabel *label_17;
    QFrame *line_2;
    QPushButton *save_coil_vals;
    QLabel *label_18;
    QLineEdit *coil_1_current;
    QLabel *label_19;
    QLineEdit *coil_2_current;
    QLabel *label_20;
    QLineEdit *collect_time_s;
    QLCDNumber *lcdNumber;
    QLCDNumber *lcdNumber_2;
    QLCDNumber *lcdNumber_3;
    QLabel *label_21;
    QLCDNumber *lcdNumber_4;
    QLabel *label_22;
    QLabel *label_23;
    QLCDNumber *lcdNumber_5;
    QLCDNumber *lcdNumber_6;
    QFrame *line_3;
    QPushButton *updateBase;
    QPushButton *printMagData;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(743, 665);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        mag_sense_x = new QLCDNumber(centralWidget);
        mag_sense_x->setObjectName(QStringLiteral("mag_sense_x"));
        mag_sense_x->setGeometry(QRect(490, 60, 64, 23));
        mag_sense_y = new QLCDNumber(centralWidget);
        mag_sense_y->setObjectName(QStringLiteral("mag_sense_y"));
        mag_sense_y->setGeometry(QRect(570, 60, 64, 23));
        mag_sense_z = new QLCDNumber(centralWidget);
        mag_sense_z->setObjectName(QStringLiteral("mag_sense_z"));
        mag_sense_z->setGeometry(QRect(640, 60, 64, 23));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(560, 40, 111, 20));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(490, 90, 59, 14));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(570, 90, 59, 14));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(640, 90, 59, 14));
        start_sensor = new QPushButton(centralWidget);
        start_sensor->setObjectName(QStringLiteral("start_sensor"));
        start_sensor->setGeometry(QRect(360, 50, 101, 41));
        start_sensor->setCheckable(true);
        coil_0_current = new QLineEdit(centralWidget);
        coil_0_current->setObjectName(QStringLiteral("coil_0_current"));
        coil_0_current->setGeometry(QRect(250, 30, 61, 22));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(150, 30, 91, 16));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(150, 100, 91, 16));
        timer_cur = new QLCDNumber(centralWidget);
        timer_cur->setObjectName(QStringLiteral("timer_cur"));
        timer_cur->setGeometry(QRect(160, 510, 91, 31));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(110, 520, 59, 14));
        collect_data = new QPushButton(centralWidget);
        collect_data->setObjectName(QStringLiteral("collect_data"));
        collect_data->setGeometry(QRect(110, 450, 141, 41));
        collect_data->setCheckable(true);
        tracker_base_y = new QLCDNumber(centralWidget);
        tracker_base_y->setObjectName(QStringLiteral("tracker_base_y"));
        tracker_base_y->setGeometry(QRect(570, 150, 64, 23));
        tracker_base_x = new QLCDNumber(centralWidget);
        tracker_base_x->setObjectName(QStringLiteral("tracker_base_x"));
        tracker_base_x->setGeometry(QRect(490, 150, 64, 23));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(550, 130, 111, 20));
        tracker_base_z = new QLCDNumber(centralWidget);
        tracker_base_z->setObjectName(QStringLiteral("tracker_base_z"));
        tracker_base_z->setGeometry(QRect(640, 150, 64, 23));
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(570, 180, 59, 14));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(640, 180, 59, 14));
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(490, 180, 59, 14));
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(550, 210, 111, 20));
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(640, 260, 59, 14));
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(490, 260, 59, 14));
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(570, 260, 59, 14));
        tracker_wand_y = new QLCDNumber(centralWidget);
        tracker_wand_y->setObjectName(QStringLiteral("tracker_wand_y"));
        tracker_wand_y->setGeometry(QRect(570, 230, 64, 23));
        tracker_wand_x = new QLCDNumber(centralWidget);
        tracker_wand_x->setObjectName(QStringLiteral("tracker_wand_x"));
        tracker_wand_x->setGeometry(QRect(490, 230, 64, 23));
        tracker_wand_z = new QLCDNumber(centralWidget);
        tracker_wand_z->setObjectName(QStringLiteral("tracker_wand_z"));
        tracker_wand_z->setGeometry(QRect(640, 230, 64, 23));
        start_polaris = new QPushButton(centralWidget);
        start_polaris->setObjectName(QStringLiteral("start_polaris"));
        start_polaris->setGeometry(QRect(360, 140, 101, 41));
        start_polaris->setCheckable(true);
        activate_mag = new QPushButton(centralWidget);
        activate_mag->setObjectName(QStringLiteral("activate_mag"));
        activate_mag->setGeometry(QRect(100, 210, 131, 51));
        activate_mag->setCheckable(true);
        activate_mag->setChecked(false);
        run_calib = new QPushButton(centralWidget);
        run_calib->setObjectName(QStringLiteral("run_calib"));
        run_calib->setGeometry(QRect(480, 470, 141, 41));
        run_calib->setCheckable(true);
        save_calib = new QPushButton(centralWidget);
        save_calib->setObjectName(QStringLiteral("save_calib"));
        save_calib->setGeometry(QRect(480, 540, 141, 41));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(30, 370, 671, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(350, 10, 59, 14));
        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(510, 420, 71, 20));
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(353, 400, 20, 221));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        save_coil_vals = new QPushButton(centralWidget);
        save_coil_vals->setObjectName(QStringLiteral("save_coil_vals"));
        save_coil_vals->setGeometry(QRect(120, 570, 121, 22));
        label_18 = new QLabel(centralWidget);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(140, 410, 101, 16));
        coil_1_current = new QLineEdit(centralWidget);
        coil_1_current->setObjectName(QStringLiteral("coil_1_current"));
        coil_1_current->setGeometry(QRect(250, 100, 61, 22));
        label_19 = new QLabel(centralWidget);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(150, 170, 91, 16));
        coil_2_current = new QLineEdit(centralWidget);
        coil_2_current->setObjectName(QStringLiteral("coil_2_current"));
        coil_2_current->setGeometry(QRect(250, 170, 61, 22));
        label_20 = new QLabel(centralWidget);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(20, 330, 181, 16));
        collect_time_s = new QLineEdit(centralWidget);
        collect_time_s->setObjectName(QStringLiteral("collect_time_s"));
        collect_time_s->setGeometry(QRect(210, 330, 113, 22));
        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));
        lcdNumber->setGeometry(QRect(10, 30, 64, 23));
        lcdNumber_2 = new QLCDNumber(centralWidget);
        lcdNumber_2->setObjectName(QStringLiteral("lcdNumber_2"));
        lcdNumber_2->setGeometry(QRect(10, 100, 64, 23));
        lcdNumber_3 = new QLCDNumber(centralWidget);
        lcdNumber_3->setObjectName(QStringLiteral("lcdNumber_3"));
        lcdNumber_3->setGeometry(QRect(70, 100, 64, 23));
        label_21 = new QLabel(centralWidget);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(20, 10, 101, 16));
        lcdNumber_4 = new QLCDNumber(centralWidget);
        lcdNumber_4->setObjectName(QStringLiteral("lcdNumber_4"));
        lcdNumber_4->setGeometry(QRect(70, 30, 64, 23));
        label_22 = new QLabel(centralWidget);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(20, 80, 101, 16));
        label_23 = new QLabel(centralWidget);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(20, 150, 101, 16));
        lcdNumber_5 = new QLCDNumber(centralWidget);
        lcdNumber_5->setObjectName(QStringLiteral("lcdNumber_5"));
        lcdNumber_5->setGeometry(QRect(10, 170, 64, 23));
        lcdNumber_6 = new QLCDNumber(centralWidget);
        lcdNumber_6->setObjectName(QStringLiteral("lcdNumber_6"));
        lcdNumber_6->setGeometry(QRect(70, 170, 64, 23));
        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(320, 50, 20, 241));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);
        updateBase = new QPushButton(centralWidget);
        updateBase->setObjectName(QStringLiteral("updateBase"));
        updateBase->setGeometry(QRect(360, 200, 101, 22));
        printMagData = new QPushButton(centralWidget);
        printMagData->setObjectName(QStringLiteral("printMagData"));
        printMagData->setGeometry(QRect(360, 100, 101, 22));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 743, 19));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label->setText(QApplication::translate("MainWindow", "Magnet Sensor", 0));
        label_2->setText(QApplication::translate("MainWindow", "X", 0));
        label_3->setText(QApplication::translate("MainWindow", "Y", 0));
        label_4->setText(QApplication::translate("MainWindow", "Z", 0));
        start_sensor->setText(QApplication::translate("MainWindow", "Start Sensor", 0));
        label_5->setText(QApplication::translate("MainWindow", "Coil 0 Current", 0));
        label_6->setText(QApplication::translate("MainWindow", "Coil 1 Current", 0));
        label_7->setText(QApplication::translate("MainWindow", "Timer", 0));
        collect_data->setText(QApplication::translate("MainWindow", "Collect Data", 0));
        label_8->setText(QApplication::translate("MainWindow", "Tracker Base", 0));
        label_9->setText(QApplication::translate("MainWindow", "Y", 0));
        label_10->setText(QApplication::translate("MainWindow", "Z", 0));
        label_11->setText(QApplication::translate("MainWindow", "X", 0));
        label_12->setText(QApplication::translate("MainWindow", "Tracker Wand", 0));
        label_13->setText(QApplication::translate("MainWindow", "Z", 0));
        label_14->setText(QApplication::translate("MainWindow", "X", 0));
        label_15->setText(QApplication::translate("MainWindow", "Y", 0));
        start_polaris->setText(QApplication::translate("MainWindow", "Start Polaris", 0));
        activate_mag->setText(QApplication::translate("MainWindow", "Activate Magnet", 0));
        run_calib->setText(QApplication::translate("MainWindow", "Run  Calibration", 0));
        save_calib->setText(QApplication::translate("MainWindow", "Save Calibration", 0));
        label_16->setText(QApplication::translate("MainWindow", "SETTINGS", 0));
        label_17->setText(QApplication::translate("MainWindow", "Calibration", 0));
        save_coil_vals->setText(QApplication::translate("MainWindow", "Save Coil Values", 0));
        label_18->setText(QApplication::translate("MainWindow", "Coil Collection", 0));
        label_19->setText(QApplication::translate("MainWindow", "Coil 2 Current", 0));
        label_20->setText(QApplication::translate("MainWindow", "TIME to Collect in sec (30 fps)", 0));
        collect_time_s->setText(QApplication::translate("MainWindow", "5", 0));
        label_21->setText(QApplication::translate("MainWindow", "Coil Temp Inner", 0));
        label_22->setText(QApplication::translate("MainWindow", "Coil Temp Middle", 0));
        label_23->setText(QApplication::translate("MainWindow", "Coil Temp Outer", 0));
        updateBase->setText(QApplication::translate("MainWindow", "Update Base", 0));
        printMagData->setText(QApplication::translate("MainWindow", "print mag data", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
