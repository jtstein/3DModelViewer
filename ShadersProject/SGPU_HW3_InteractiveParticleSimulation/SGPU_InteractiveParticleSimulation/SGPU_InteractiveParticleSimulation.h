
#ifndef SGPU_INTERACTIVEPARTICLESIMULATION_H
#define SGPU_INTERACTIVEPARTICLESIMULATION_H

#include <QtWidgets/QMainWindow>
#include "ui_SGPU_InteractiveParticleSimulation.h"

class QViewport;

class SGPU_InteractiveParticleSimulation : public QMainWindow {
    Q_OBJECT

public:
    SGPU_InteractiveParticleSimulation(QWidget *parent = 0);
    ~SGPU_InteractiveParticleSimulation();

public slots:
    /* Function that is called when the Update button is released. */
    void onUpdate();

    /* Function that is called when the Color Button "..." is released */
    void onColorButton();

private:
    Ui::SGPU_InteractiveParticleSimulationClass ui;
};

#endif