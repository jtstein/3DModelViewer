#include "SGPU_InteractiveParticleSimulation.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    SGPU_InteractiveParticleSimulation w;
    w.show();
    return a.exec();
}
