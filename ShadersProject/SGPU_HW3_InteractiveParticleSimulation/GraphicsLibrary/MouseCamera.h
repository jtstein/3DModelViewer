
#ifndef MOUSE_CAMERA_H
#define MOUSE_CAMERA_H

#include <Vector2.h>
#include "Camera.h"

enum MouseButtonState { LB_DOWN, LB_UP, MB_DOWN, MB_UP, RB_DOWN, RB_UP };

template <typename Real>
class MouseCamera : public Camera<Real> {
public:
    MouseCamera(Real radius = Real(1));
    ~MouseCamera();

    bool unproject(int width, int height, int x, int y, Vector3<Real>& point, Vector3<Real>& direction);
    bool unproject(int width, int height, int x, int y, Real z, Vector3<Real>& point);

    void onMouseMove(int x, int y);
    void onMouseButton(MouseButtonState buttonState, int x, int y);

    bool isMoving() const;
    bool isZooming() const;

protected:
    Vector2i mouseCur;
    Vector2i mouseLast;

    float rotateSensitivity;
    float zoomSensitivity;

    bool mouseMove;
    bool mouseZoom;
};

const static float DEFAULT_ROTATE_SENSITIVITY = 0.01f;
const static float DEFAULT_ZOOM_SENSITIVITY = 0.1f;

template <typename Real>
MouseCamera<Real>::MouseCamera(Real radius = Real(1)) : Camera(radius) {
    this->rotateSensitivity = DEFAULT_ROTATE_SENSITIVITY;
    this->zoomSensitivity = DEFAULT_ZOOM_SENSITIVITY;
    this->mouseMove = false;
    this->mouseZoom = false;
}

template <typename Real>
MouseCamera<Real>::~MouseCamera() {}

template <typename Real>
bool MouseCamera<Real>::unproject(int width, int height, int x, int y, Vector3<Real>& point, Vector3<Real>& direction) {
    Vector3<Real> p0, p1;
    this->unproject(width, height, x, y, Real(0), p0);
    this->unproject(width, height, x, y, Real(1), p1);
    point = p0;
    direction = ((p0 - this->eye) * this->fov).normalized();
    return true;
}

template <typename Real>
bool MouseCamera<Real>::unproject(int width, int height, int x, int y, Real z, Vector3<Real>& point) {
    Matrix4<Real> mvp = this->getProjectionMatrix() * this->getViewMatrix();
    Matrix4<Real> inv = mvp.inverse();

    float w = static_cast<float>(width);
    float h = static_cast<float>(height);
    Vector4<Real> in;

    in[0] = x / w * Real(2) - Real(1);
    in[1] = y / h * Real(2) - Real(1);
    in[2] = Real(2) * z - Real(1);
    in[3] = Real(1);

    Vector4<Real> out = inv.applyTo(in);
    if ( out[3] == Real(0) ) return false;

    point[0] = out[0] * out[3];
    point[1] = out[1] * out[3];
    point[2] = out[2] * out[3];

    return true;
}

template <typename Real>
void MouseCamera<Real>::onMouseMove(int x, int y) {
    this->mouseCur.set(x, y);

	int x_diff = this->mouseLast.x() - this->mouseCur.x();
	int y_diff = this->mouseLast.y() - this->mouseCur.y();

    if ( mouseMove ) {
        float dTheta = this->rotateSensitivity * (float)x_diff;
	    float dPhi = this->rotateSensitivity * (float)y_diff;
        this->rotate(-dTheta, dPhi);
    }

    if ( mouseZoom ) {
        float dr = this->zoomSensitivity * (float)y_diff;
        this->addRadius(dr);
    }

	this->mouseLast = this->mouseCur;
}

template <typename Real>
void MouseCamera<Real>::onMouseButton(MouseButtonState buttonState, int x, int y) {
    if ( buttonState == LB_DOWN ) {
        mouseLast.set(x, y);
        mouseMove = true;
    }

    if ( buttonState == RB_DOWN ) {
        mouseLast.set(x, y);
        mouseZoom = true;
    }

    if ( buttonState == LB_UP ) mouseMove = false;
    if ( buttonState == RB_UP ) mouseZoom = false;
}

template <typename Real>
bool MouseCamera<Real>::isMoving() const {
    return this->mouseMove;
}

template <typename Real>
bool MouseCamera<Real>::isZooming() const {
    return this->mouseZoom;
}

typedef MouseCamera<float> MouseCameraf;
typedef MouseCamera<double> MouseCamerad;

#endif
