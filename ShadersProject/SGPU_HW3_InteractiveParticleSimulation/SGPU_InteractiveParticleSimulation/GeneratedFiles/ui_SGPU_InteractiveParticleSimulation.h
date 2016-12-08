/********************************************************************************
** Form generated from reading UI file 'SGPU_InteractiveParticleSimulation.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SGPU_INTERACTIVEPARTICLESIMULATION_H
#define UI_SGPU_INTERACTIVEPARTICLESIMULATION_H

#include <QViewport.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SGPU_InteractiveParticleSimulationClass
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QWidget *toolFrame;
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *label_14;
    QLabel *label_11;
    QDoubleSpinBox *positionX;
    QRadioButton *colorMappingRadio;
    QLabel *label_13;
    QDoubleSpinBox *positionY;
    QLabel *label_12;
    QLabel *label_15;
    QComboBox *modelComboBox;
    QRadioButton *surfaceNormalRadio;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *textureComboBox;
    QDoubleSpinBox *positionZ;
    QHBoxLayout *horizontalLayout;
    QDoubleSpinBox *normalScale;
    QLabel *imgLabel;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_7;
    QLabel *label_2;
    QRadioButton *rotatingLightingRadio;
    QToolButton *colorButton;
    QLabel *label_6;
    QLabel *label_4;
    QRadioButton *fixedLightingRadio;
    QDoubleSpinBox *rotatingSpeed;
    QCheckBox *phongCheckBox;
    QPushButton *updateButton;
    QSpacerItem *verticalSpacer;
    QLabel *label_5;
    QViewport *viewport;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SGPU_InteractiveParticleSimulationClass)
    {
        if (SGPU_InteractiveParticleSimulationClass->objectName().isEmpty())
            SGPU_InteractiveParticleSimulationClass->setObjectName(QStringLiteral("SGPU_InteractiveParticleSimulationClass"));
        SGPU_InteractiveParticleSimulationClass->resize(1264, 892);
        actionExit = new QAction(SGPU_InteractiveParticleSimulationClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(SGPU_InteractiveParticleSimulationClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        toolFrame = new QWidget(splitter);
        toolFrame->setObjectName(QStringLiteral("toolFrame"));
        toolFrame->setMaximumSize(QSize(400, 16777215));
        gridLayout_4 = new QGridLayout(toolFrame);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(6, 6, 6, 6);
        groupBox_2 = new QGroupBox(toolFrame);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(8);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(8, 8, 8, 8);
        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_2->addWidget(label_14, 7, 0, 1, 1);

        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_2->addWidget(label_11, 5, 5, 1, 1);

        positionX = new QDoubleSpinBox(groupBox_2);
        positionX->setObjectName(QStringLiteral("positionX"));
        positionX->setMinimum(-100);
        positionX->setMaximum(100);
        positionX->setSingleStep(1);

        gridLayout_2->addWidget(positionX, 5, 4, 1, 1);

        colorMappingRadio = new QRadioButton(groupBox_2);
        colorMappingRadio->setObjectName(QStringLiteral("colorMappingRadio"));

        gridLayout_2->addWidget(colorMappingRadio, 25, 0, 1, 2);

        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_2->addWidget(label_13, 1, 0, 1, 1);

        positionY = new QDoubleSpinBox(groupBox_2);
        positionY->setObjectName(QStringLiteral("positionY"));
        positionY->setMinimum(-100);
        positionY->setMaximum(100);
        positionY->setSingleStep(1);
        positionY->setValue(1);

        gridLayout_2->addWidget(positionY, 5, 6, 1, 1);

        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_2->addWidget(label_12, 0, 0, 1, 1);

        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_2->addWidget(label_15, 5, 7, 1, 1);

        modelComboBox = new QComboBox(groupBox_2);
        modelComboBox->setObjectName(QStringLiteral("modelComboBox"));

        gridLayout_2->addWidget(modelComboBox, 0, 4, 1, 5);

        surfaceNormalRadio = new QRadioButton(groupBox_2);
        surfaceNormalRadio->setObjectName(QStringLiteral("surfaceNormalRadio"));

        gridLayout_2->addWidget(surfaceNormalRadio, 20, 0, 1, 2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));

        gridLayout_2->addLayout(horizontalLayout_3, 27, 8, 1, 1);

        textureComboBox = new QComboBox(groupBox_2);
        textureComboBox->setObjectName(QStringLiteral("textureComboBox"));

        gridLayout_2->addWidget(textureComboBox, 1, 4, 1, 5);

        positionZ = new QDoubleSpinBox(groupBox_2);
        positionZ->setObjectName(QStringLiteral("positionZ"));
        positionZ->setMinimum(-100);
        positionZ->setMaximum(100);
        positionZ->setSingleStep(1);

        gridLayout_2->addWidget(positionZ, 5, 8, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

        gridLayout_2->addLayout(horizontalLayout, 20, 8, 1, 1);

        normalScale = new QDoubleSpinBox(groupBox_2);
        normalScale->setObjectName(QStringLiteral("normalScale"));
        normalScale->setMaximum(15);
        normalScale->setSingleStep(0.5);
        normalScale->setValue(1);

        gridLayout_2->addWidget(normalScale, 20, 6, 1, 1);

        imgLabel = new QLabel(groupBox_2);
        imgLabel->setObjectName(QStringLiteral("imgLabel"));

        gridLayout_2->addWidget(imgLabel, 4, 0, 1, 2);

        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 27, 0, 1, 2);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 11, 0, 1, 5);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_2->addWidget(label_7, 5, 0, 1, 2);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 20, 3, 1, 3);

        rotatingLightingRadio = new QRadioButton(groupBox_2);
        rotatingLightingRadio->setObjectName(QStringLiteral("rotatingLightingRadio"));

        gridLayout_2->addWidget(rotatingLightingRadio, 7, 3, 1, 3);

        colorButton = new QToolButton(groupBox_2);
        colorButton->setObjectName(QStringLiteral("colorButton"));

        gridLayout_2->addWidget(colorButton, 27, 2, 1, 1);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_2->addWidget(label_6, 5, 3, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 7, 6, 1, 2);

        fixedLightingRadio = new QRadioButton(groupBox_2);
        fixedLightingRadio->setObjectName(QStringLiteral("fixedLightingRadio"));

        gridLayout_2->addWidget(fixedLightingRadio, 7, 1, 1, 2);

        rotatingSpeed = new QDoubleSpinBox(groupBox_2);
        rotatingSpeed->setObjectName(QStringLiteral("rotatingSpeed"));
        rotatingSpeed->setMinimum(1);
        rotatingSpeed->setMaximum(10);
        rotatingSpeed->setSingleStep(1);
        rotatingSpeed->setValue(1);

        gridLayout_2->addWidget(rotatingSpeed, 7, 8, 1, 1);

        phongCheckBox = new QCheckBox(groupBox_2);
        phongCheckBox->setObjectName(QStringLiteral("phongCheckBox"));

        gridLayout_2->addWidget(phongCheckBox, 11, 6, 1, 3);


        gridLayout_4->addWidget(groupBox_2, 0, 0, 1, 1);

        updateButton = new QPushButton(toolFrame);
        updateButton->setObjectName(QStringLiteral("updateButton"));

        gridLayout_4->addWidget(updateButton, 19, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 320, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 33, 0, 1, 1);

        label_5 = new QLabel(toolFrame);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_4->addWidget(label_5, 20, 0, 1, 1);

        splitter->addWidget(toolFrame);
        viewport = new QViewport(splitter);
        viewport->setObjectName(QStringLiteral("viewport"));
        splitter->addWidget(viewport);

        gridLayout->addWidget(splitter, 0, 0, 1, 1);

        SGPU_InteractiveParticleSimulationClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SGPU_InteractiveParticleSimulationClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1264, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        SGPU_InteractiveParticleSimulationClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(SGPU_InteractiveParticleSimulationClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SGPU_InteractiveParticleSimulationClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit);

        retranslateUi(SGPU_InteractiveParticleSimulationClass);
        QObject::connect(actionExit, SIGNAL(triggered()), SGPU_InteractiveParticleSimulationClass, SLOT(close()));
        QObject::connect(updateButton, SIGNAL(released()), SGPU_InteractiveParticleSimulationClass, SLOT(onUpdate()));
        QObject::connect(colorButton, SIGNAL(released()), SGPU_InteractiveParticleSimulationClass, SLOT(onColorButton()));

        QMetaObject::connectSlotsByName(SGPU_InteractiveParticleSimulationClass);
    } // setupUi

    void retranslateUi(QMainWindow *SGPU_InteractiveParticleSimulationClass)
    {
        SGPU_InteractiveParticleSimulationClass->setWindowTitle(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "SGPU_InteractiveParticleSimulation", 0));
        actionExit->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Exit", 0));
        groupBox_2->setTitle(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Visual Properties", 0));
        label_14->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Lighting", 0));
        label_11->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "y", 0));
        colorMappingRadio->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Color Mapping", 0));
        label_13->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Texture", 0));
        label_12->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Model", 0));
        label_15->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "z", 0));
        modelComboBox->clear();
        modelComboBox->insertItems(0, QStringList()
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "sphere", 0)
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "teapot", 0)
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "torus", 0)
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "cube", 0)
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "bunny", 0)
        );
        surfaceNormalRadio->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Surface Normal", 0));
        textureComboBox->clear();
        textureComboBox->insertItems(0, QStringList()
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "bark", 0)
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "brick", 0)
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "brushed", 0)
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "cleanwood", 0)
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "liquidmetal", 0)
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "pavers", 0)
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "plate", 0)
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "rustymetal", 0)
         << QApplication::translate("SGPU_InteractiveParticleSimulationClass", "scrap", 0)
        );
        imgLabel->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Texture Preview:", 0));
        label->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Background Color:", 0));
        label_3->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Special Effects:", 0));
        label_7->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Model Position:", 0));
        label_2->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "normalScale:", 0));
        rotatingLightingRadio->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Rotating", 0));
        colorButton->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "...", 0));
        label_6->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "x", 0));
        label_4->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Rotate Speed", 0));
        fixedLightingRadio->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Fixed", 0));
        phongCheckBox->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Phong Shading", 0));
        updateButton->setText(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "Update", 0));
        label_5->setText(QString());
        menuFile->setTitle(QApplication::translate("SGPU_InteractiveParticleSimulationClass", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class SGPU_InteractiveParticleSimulationClass: public Ui_SGPU_InteractiveParticleSimulationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SGPU_INTERACTIVEPARTICLESIMULATION_H
