// Jordan Stein

#include "QViewport.h"
#include <GeometryShader.h>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <MouseCamera.h>
#include <Mesh.h>
#include <Shader.h>
#include <Texture.h>

const static float RAY_EXT = 20.0f;
const static float POINT_EXT = 6.0f;

float rotationLightPhi = 0.001f; // used for rotation lighting.

QViewport::QViewport(QWidget* parent) : QGLWidget(parent) {
    this->setMouseTracking(true);

    //------------------------------------------------------------------------------
    // Set the timer to call the onTimeout function at 60[fps]
    //------------------------------------------------------------------------------
    this->timer = new QTimer(this);
    this->connect(this->timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    this->timer->setInterval(16);
    this->timer->start();

    this->camera = std::make_shared<MouseCameraf>();

    //------------------------------------------------------------------------------
    // The simulation time-step matches the timer update specified in seconds.
    //------------------------------------------------------------------------------
    this->timeStep = 0.016f;
}

QViewport::~QViewport() {}

void GetModelViewMatrix(std::shared_ptr<MouseCameraf>& camera, double* m) {
    float modelViewMatrix[16];
    camera->getViewMatrix().toRawMatrix(modelViewMatrix, true);
    for ( unsigned int i = 0; i < 16; i++ ) m[i] = static_cast<double>(modelViewMatrix[i]);
}

void GetProjectionMatrix(std::shared_ptr<MouseCameraf>& camera, double* m) {
    float projectionMatrix[16];
    camera->getProjectionMatrix().toRawMatrix(projectionMatrix, true);
    for ( unsigned int i = 0; i < 16; i++ ) m[i] = static_cast<double>(projectionMatrix[i]);
}


void QViewport::initializeGL() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // alters the background color
	glPointSize(10.0f);
	glEnable(GL_DEPTH_TEST);
    glewInit();
    
    this->camera->setPosition(30.0f, 0.0f, 10.570f * 0.7f); // update initial camera position
    this->camera->setLookAt(Vector3f(0.0f, 1.0f, 0.0f));
	
	// load the inital mesh properties (sphere with bark texture)
	this->mesh->load("modellib/"+this->model+".obj");
	this->mesh->loadShader("shaders/RealisticMesh.vert", "shaders/RealisticMesh.frag"); // default shaders
	this->mesh->setDiffuseTexture("textures/"+this->texture+"_diffuse.png");
	this->mesh->setNormalTexture("textures/"+this->texture+"_normal.png");
	this->mesh->setSpecularTexture("textures/"+this->texture+"_specular.png");
	this->mesh->setPosition(0.0f, 1.0f, 0.0f);

}

void QViewport::onTimeout() {

    //------------------------------------------------------------------------------
    // Get the model view and projection matrices from the camera. Sine the
    // gluUnProject function requires double arrays, the cameras float arrays
    // are stored within a double array.
    //------------------------------------------------------------------------------
    double modelViewMatrix[16];
    double projectionMatrix[16];
    GetModelViewMatrix(camera, modelViewMatrix);
    GetProjectionMatrix(camera, projectionMatrix);
    double pointX, pointY, pointZ;

    //------------------------------------------------------------------------------
    // Get the size of the current OpenGL viewport. This is required for the
    // calculations within the gluUnProject function.
    //
    // viewport[0] = x
    // viewport[1] = y
    // viewport[2] = width
    // viewport[3] = height
    //------------------------------------------------------------------------------
    int viewport[4];
    std::memset(viewport, 0, sizeof(viewport));
    viewport[2] = this->width();
    viewport[3] = this->height();
    
    //------------------------------------------------------------------------------
    // Unproject the 2D mouse coordinate (flipped y) to a 3D point in the global
    // coordinate space. The 3D point is stored in pointX, pointY, pointZ.
    //------------------------------------------------------------------------------
    gluUnProject(this->mouseX, viewport[3] - this->mouseY, 0.1f, modelViewMatrix, projectionMatrix, viewport, &pointX, &pointY, &pointZ);
    Vector3f p(pointX, pointY, pointZ);

    this->updateGL();
}


void QViewport::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (this->colorChange == true){ // will be true if the background color is flagged to be changed.
		glClearColor(this->r, this->g, this->b, 1.0f); // alters the background color
		this->colorChange = false; // update it to false for the next iteration.
	}

	/* this if statement allows us to smoothly switch between any special effect setting and fixed/rotating lighting */
	if ((this->surfaceNorm == false && this->passed == true) || (this->colorMapping == false && this->passedC == true) || (this->phongShading == false && this->passedP == true)){
		this->mesh = std::make_shared<Mesh>(); // reinitialize new mesh
		this->mesh->load("modellib/" + this->model + ".obj");

		this->mesh->loadShader("shaders/RealisticMesh.vert", "shaders/RealisticMesh.frag");

		/* load the selected texture into the mesh. */
		this->mesh->setDiffuseTexture("textures/" + this->texture + "_diffuse.png");
		this->mesh->setNormalTexture("textures/" + this->texture + "_normal.png");
		this->mesh->setSpecularTexture("textures/" + this->texture + "_specular.png");
		
		
		this->mesh->setDiffuseTexture("textures/" + this->texture + "_diffuse.png");
		this->mesh->setNormalTexture("textures/" + this->texture + "_normal.png");
		this->mesh->setSpecularTexture("textures/" + this->texture + "_specular.png");

		//this->mesh->setPosition(this->posX, this->posY, this->posZ);

		this->lastModel = this->model; // update the lastmodel info with the new model

		if (this->surfaceNorm == false && this->passed == true){ // switched surfaceNorm passed to false
			this->passed = false;
		}
		else if (this->colorMapping == false && this->passedC == true){ // switches colormapping passed to false
			this->passedC = false;
		}
		else this->passedP = false; // switched phongshading passed to false

	}

    //------------------------------------------------------------------------------
    // Render the mesh
    //------------------------------------------------------------------------------

	if (this->model != ""  && this->lastModel != this->model){ // check if we need to update the model
		this->mesh = std::make_shared<Mesh>(); // reinitialize new mesh
		this->mesh->load("modellib/" + this->model + ".obj");
		

		this->mesh->loadShader("shaders/RealisticMesh.vert", "shaders/RealisticMesh.frag");

		/* load the selected texture into the mesh. */
		this->mesh->setDiffuseTexture("textures/" + this->texture + "_diffuse.png");
		this->mesh->setNormalTexture("textures/" + this->texture + "_normal.png");
		this->mesh->setSpecularTexture("textures/" + this->texture + "_specular.png");


		//this->mesh->setPosition(this->posX, this->posY, this->posZ);

		this->lastModel = this->model; // update the lastmodel info with the new model
	}

	if (this->texture != "" && this->lastTexture != this->texture){ // check if we need to update the texture
		this->mesh->setDiffuseTexture("textures/" + this->texture + "_diffuse.png");
		this->mesh->setNormalTexture("textures/" + this->texture + "_normal.png");
		this->mesh->setSpecularTexture("textures/" + this->texture + "_specular.png");
		this->lastTexture = this->texture; // update the lastTexture info to the new texture
	}

	if (this->phongShading == true){
		this->mesh->loadShader("shaders/PhongShading2.vert", "shaders/PhongShading2.frag");
		this->passedP = true;
	}

	if (this->lighting == "rotating"){
		rotationLightPhi += this->lightRotateSpeed; // used to rotate phi so the light x/y will oscillate between -1 and 1 with sin/cos
	}

	float r = 10.0f;
	float x, y;
	x = r * std::cos(rotationLightPhi);
	y = r * std::sin(rotationLightPhi);

	Matrix4f modelView = camera->getViewMatrix()*mesh->getTransform().toMatrix();
	this->mesh->beginRender();
	this->mesh->getShader()->uniformMatrix("projectionMatrix", camera->getProjectionMatrix());
	this->mesh->getShader()->uniformMatrix("modelViewMatrix", modelView);
	this->mesh->getShader()->uniformMatrix("normalMatrix", Matrix4f::Transpose(modelView.toInverse()));
	this->mesh->getShader()->uniformVector("lightPosition", Vector3f(x, 5.0f, y));

	this->mesh->setPosition(this->posX, this->posY, this->posZ);

	if (!surfaceNorm && !colorMapping)
	this->mesh->endRender();

	/* Extra feature: Used if the user wishes to view the surface normal of the model. */
	if (this->surfaceNorm == true){

	//	this->mesh->setPosition(this->posX, this->posY, this->posZ);
		
		std::shared_ptr<Shader> phongShader = nullptr;
		std::shared_ptr<GeometryShader> normalShader = nullptr;

		
		phongShader = std::make_shared<Shader>();
		phongShader->load("shaders/PhongShading.vert", "shaders/PhongShading.frag");
		phongShader->compile();
		phongShader->link();
		
		normalShader = std::make_shared<GeometryShader>();
		normalShader->load("shaders/NormalVisualization.vert", "shaders/NormalVisualization.geom", "shaders/NormalVisualization.frag");
		normalShader->compile();
		normalShader->link();
		Matrix4f transform = this->mesh->getTransform().toMatrix();
		Matrix4f view = camera->getViewMatrix();
		Matrix4f modelViewMatrix = transform * camera->getViewMatrix();
		Matrix3f normalMatrix = Matrix4f::NormalMatrix(modelViewMatrix);
		Matrix4f projectionMatrix = camera->getProjectionMatrix();

		/* First Pass: Phong Surface */
		this->mesh->setShader(phongShader);
		this->mesh->beginRender();
		this->mesh->getShader()->uniformMatrix("projectionMatrix", projectionMatrix);
		this->mesh->getShader()->uniformMatrix("modelViewMatrix", modelViewMatrix);
		this->mesh->getShader()->uniformMatrix("normalMatrix", normalMatrix);
		this->mesh->getShader()->uniformVector("lightPosition", Vector3f(0.0f, 1.0f, 20.0f));
		this->mesh->endRender();

		/* Second Pass: Normals */
		this->mesh->setShader(normalShader);
		this->mesh->beginRender();
		this->mesh->getShader()->uniformMatrix("projectionMatrix", projectionMatrix);
		this->mesh->getShader()->uniformMatrix("modelViewMatrix", modelViewMatrix);
		this->mesh->getShader()->uniformMatrix("normalMatrix", normalMatrix);
		this->mesh->getShader()->uniform1f("normalScale", this->normalScale);
		this->mesh->endRender();

		this->passed = true;
	}


	if (this->colorMapping == true){
		float phi = 0.001f;
		float r = 10.0f;
		float x, y;

		x = r * std::cos(phi);
		y = r * std::sin(phi);

		this->mesh->loadShader("shaders/ColorMapping.vert", "shaders/ColorMapping.frag");


		this->mesh->setPosition(this->posX, this->posY, this->posZ);

		this->mesh->beginRender();
		this->mesh->getShader()->uniformMatrix("projectionMatrix", camera->getProjectionMatrix());
		this->mesh->getShader()->uniformMatrix("modelViewMatrix", camera->getViewMatrix());
		this->mesh->getShader()->uniformMatrix("normalMatrix", Matrix4f::Transpose(camera->getViewMatrix().toInverse()));
		this->mesh->endRender();

		this->passedC = true;
	}

    glFlush();
}


void QViewport::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    this->camera->setPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float proj[16];
    this->camera->getProjectionMatrix().toRawMatrix(proj);
    glMultMatrixf(proj);
    glMatrixMode(GL_MODELVIEW);
}

void QViewport::mouseMoveEvent(QMouseEvent* e) {
    this->camera->onMouseMove(e->x(), e->y());
    this->mouseX = e->x();
    this->mouseY = e->y();
}

void QViewport::mousePressEvent(QMouseEvent* e) {
    if ( e->button() == Qt::LeftButton ) this->camera->onMouseButton(LB_DOWN, e->x(), e->y());
    else if ( e->button() == Qt::RightButton ) {
    }
}

void QViewport::mouseReleaseEvent(QMouseEvent* e) {
    if ( e->button() == Qt::LeftButton ) this->camera->onMouseButton(LB_UP, e->x(), e->y());
    else if ( e->button() == Qt::RightButton ) {
    }
}
