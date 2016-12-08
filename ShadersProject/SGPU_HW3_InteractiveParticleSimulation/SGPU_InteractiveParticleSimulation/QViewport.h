// Jordan Stein 101390302
#ifndef Q_VIEWPORT_H
#define Q_VIEWPORT_H

#include <gl/glew.h>
#include <QtOpenGL>
#include <memory>
#include <MouseCamera.h>
//#include <ParticleSystem.h>
#include <Grid.h>
#include <Mesh.h>
#include <string>

class QTimer;

class QViewport : public QGLWidget {
    Q_OBJECT

public:
    QViewport(QWidget* parent);
    virtual ~QViewport();

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

	void setColor(float r, float g, float b){
		this->r = r;
		this->g = g;
		this->b = b;
		this->colorChange = true;
	}

	void setModel(std::string model){
		this->model = model;
	}

	void setTexture(std::string texture){
		this->texture = texture;
	}

	void setLighting(std::string lighting, float rotatingSpeed){
		this->lighting = lighting;
		this->lightRotateSpeed = 0.004363323 + rotatingSpeed/100;
	}

	void setPosition(float x, float y, float z){
		this->posX = x;
		this->posY = y;
		this->posZ = z;
		this->positionChange = true;
	}

	void setSurfaceNorm(bool surfaceNorm, float normalScale){
		this->surfaceNorm = surfaceNorm;
		this->normalScale = normalScale;
	}

	void setColorMapping(bool colorMapping){
		this->colorMapping = colorMapping;
	}

	void setPhongShading(bool phongShading){
		this->phongShading = phongShading;
	}

    void mouseMoveEvent(QMouseEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

public slots:
    void onTimeout();

protected:
    std::shared_ptr<MouseCameraf> camera;

	/* This will store information about the mesh we will be updating. */
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	/* This will store the name of the model/texture/lighting that will be rendered. */
	std::string model = "sphere"; // default model is a sphere
	std::string lastModel = ""; // stores the model that was last updated.
	std::string texture = "bark"; // default texture is bark
	std::string lastTexture = ""; // stores the texture that was last updated.
	std::string lighting = "";
	float lightRotateSpeed = 0.004363323; // rotation of the light

	/* This stores the background color. Default color is gray. */
	float r = 2.0f; // red
	float g = 2.0f; // green
	float b = 2.0f; // blue
	bool colorChange = false; // determines if the user updated the background color.

	/* Stores mesh postion x,y,z */
	float posX = 0.0f;
	float posY = 1.0f;
	float posZ = 0.0;
	bool positionChange = false;

	/* variables used if the user requests for surface normal shading. */
	bool surfaceNorm = false; // checks if the user wants to view the surface normal.
	bool passed = false; // used for rendering the surface normal.
	float normalScale = 1.0f; // default surface normal scale.

	bool colorMapping = false;
	bool passedC = false;

	bool phongShading = false;
	bool passedP = false;
    /* Timer used to update the viewport for 60[fps] */
    QTimer* timer;
    float timeStep;

    /* 
     * Variables used to control and define the spray direction of the particles.
     * The point represents the spawn position of the particles at the end of the mouse
     * in world coordinates. The direction is defined by the combination of the mouse
     * position (x, y) and the direction defined by the view frustrum of the camera.
     */
    int mouseX, mouseY;
};

#endif
