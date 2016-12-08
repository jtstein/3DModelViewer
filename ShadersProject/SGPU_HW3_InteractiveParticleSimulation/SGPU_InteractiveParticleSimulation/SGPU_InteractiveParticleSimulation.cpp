// Jordan Stein 101390302
#include "SGPU_InteractiveParticleSimulation.h"
#include <QColorDialog>
#include <qtimer.h>
#include "QViewport.h"
#include <string>
#include <QDebug>


SGPU_InteractiveParticleSimulation::SGPU_InteractiveParticleSimulation(QWidget *parent) : QMainWindow(parent) {
    this->ui.setupUi(this);
}

SGPU_InteractiveParticleSimulation::~SGPU_InteractiveParticleSimulation() {}

void SGPU_InteractiveParticleSimulation::onUpdate() {
    std::cout << "Update!" << std::endl;

    //--------------------------------------------------------------------------
    // The ui member of this class holds all of the user interface components 
    // from the demo. To acces the value of a Qt SpinBox, the value() function 
    // is used.
    //--------------------------------------------------------------------------
    
	// Initialize values passed from the ui
	std::string model = this->ui.modelComboBox->currentText().toStdString();
	std::string texture = this->ui.textureComboBox->currentText().toStdString();
	std::string lighting = "";

	/* shows texture preview image on Qt */
	QString texturePath = QString::fromStdString("textures/" + texture + "_diffuse.png");
	QImage image(texturePath);
	image = image.scaled(200, 120, Qt::KeepAspectRatio);
	this->ui.imgLabel->setPixmap(QPixmap::fromImage(image));
	
	float rotatingSpeed;
	if (this->ui.fixedLightingRadio->isChecked()){
		lighting = "fixed";
	}
	else if (this->ui.rotatingLightingRadio->isChecked()){
		lighting = "rotating";
		rotatingSpeed = this->ui.rotatingSpeed->value();
	}

	float posX = this->ui.positionX->value();
	float posY = this->ui.positionY->value();
	float posZ = this->ui.positionZ->value();

	bool surfaceNorm = false;
	float normalScale;
	bool colorMapping = false;

	if (this->ui.surfaceNormalRadio->isChecked()){
		surfaceNorm = true;
		normalScale = this->ui.normalScale->value();
	}
	if (this->ui.colorMappingRadio->isChecked()){
		colorMapping = true;
	}

	bool phongShading = false;
	if (this->ui.phongCheckBox->isChecked()){
		phongShading = true;
	}
	else phongShading = false;

//	qDebug() << QString::fromStdString(model);
//	qDebug() << surfaceNorm;

    //--------------------------------------------------------------------------
    // The value set from the user interface can then be transfered through
    // the viewport to the particle system.
    //--------------------------------------------------------------------------
    
	// These calls send the update values into the QViewport
	this->ui.viewport->setModel(model);
	this->ui.viewport->setTexture(texture);
	this->ui.viewport->setLighting(lighting, rotatingSpeed);
	this->ui.viewport->setPosition(posX, posY, posZ);
	this->ui.viewport->setSurfaceNorm(surfaceNorm, normalScale);
	this->ui.viewport->setColorMapping(colorMapping);
	this->ui.viewport->setPhongShading(phongShading);

}

void SGPU_InteractiveParticleSimulation::onColorButton() {
    QColor color = QColorDialog::getColor(Qt::blue, this);

    //--------------------------------------------------------------------------
    // TODO: Update the particle color.
    //--------------------------------------------------------------------------
	
	// color.red/green/blue() returns an int from 0-255 so i'll cast it
	// to float and get the decimal between 0.0-1.0 by dividing by 255.
	float r = static_cast<float>(color.red()) / 255;
	float g = static_cast<float>(color.green()) / 255;
	float b = static_cast<float>(color.blue()) / 255;

    if ( color.isValid() ) {
		this->ui.viewport->setColor(r,g,b);
    }
}
