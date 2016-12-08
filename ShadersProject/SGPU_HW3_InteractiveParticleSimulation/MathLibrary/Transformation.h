/*
 * Copyright (c) 2015 University of Colorado [http://www.ucdenver.edu]
 * Computer Graphics Laboratory [Min Choi, Shane Transue]
 *
 * min.choi@ucdenver.edu
 * shane.transue@ucdenver.edu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "Quaternion.h"
#include "Matrix4.h"

template <typename Real>
class Transformation;

template <typename Real>
std::ostream& operator << <> (std::ostream& out, const Transformation<Real>& transform);

/*
 * Transformation: Homogeneous representation of a mathematical transformation matrix.
 *
 * [1 0 0 x]
 * [0 1 0 y]
 * [0 0 1 z]
 * [0 0 0 1]
 * 
 * This implementation also foregoes vectorization (SSE).
 * This implementation is aimed at a flexibility while preserving 
 * understandability and complete modularity.
 */
template <typename Real>
class Transformation {
public:
    Transformation();
    Transformation(const Vector3<Real>& position);
    Transformation(const Transformation<Real>& transform);
    virtual ~Transformation();

    void setPositionX(Real x);
    void setPositionY(Real y);
    void setPositionZ(Real z);
    void addPositionX(Real x);
    void addPositionY(Real y);
    void addPositionZ(Real z);
    void setPosition(Real x, Real y, Real z);
    void setPosition(const Vector3<Real>& position);

    void setScaleX(Real sx);
    void setScaleY(Real sy);
    void setScaleZ(Real xz);
    void addScaleX(Real sx);
    void addScaleY(Real sy);
    void addScaleZ(Real sz);
    void setScale(Real sx, Real sy, Real sz);
    void setScale(const Vector3<Real>& scale);

    void addRotation(const Quaternion<Real>& rotation);
    void setRotation(const Quaternion<Real>& rotation);
    
    friend std::ostream& operator << <> (std::ostream& out, const Transformation<Real>& transform);

    const Vector3<Real>& getPosition() const;
    const Vector3<Real>& getScale() const;
    const Quaternion<Real>& getRotation() const;
    Vector3<Real>& getPosition();
    Vector3<Real>& getScale();
    Quaternion<Real>& getRotation();

    const Matrix4<Real>& toMatrix() const;
    Matrix4<Real> toTransformationMatrix() const;

    Transformation<Real>& operator = (const Transformation<Real>& transform);

    static Transformation<Real> Identity();
    static Transformation<Real> Scale(Real sx, Real sy, Real sz);
    static Transformation<Real> Translate(Real x, Real y, Real z);
    static Transformation<Real> RotateX(Real angle);
    static Transformation<Real> RotateY(Real angle);
    static Transformation<Real> RotateZ(Real angle);

protected:
    void compile(bool colMajor = true);

protected:
    Vector3<Real> position;
    Vector3<Real> scale;
    Quaternion<Real> rotation;

    Matrix4<Real> transform;
};

template <typename Real>
Transformation<Real>::Transformation() {
    this->position = Vector3<Real>();
    this->scale = Vector3<Real>(Real(1), Real(1), Real(1));
    this->rotation = Quaternion<Real>::Identity();
    this->transform = Matrix4<Real>(false);
}

template <typename Real>
Transformation<Real>::Transformation(const Vector3<Real>& position) {
    this->position = position;
    this->scale = Vector3<Real>(Real(1), Real(1), Real(1));
    this->rotation = Quaternion<Real>::Identity();
    this->transform = Matrix4<Real>(false);
}

template <typename Real>
Transformation<Real>::Transformation(const Transformation<Real>& transform) {
    this->position = transform.position;
    this->scale = transform.scale;
    this->rotation = transform.rotation;
    this->transform = transform.transform;
}

template <typename Real>
Transformation<Real>::~Transformation() {}

template <typename Real>
void Transformation<Real>::setPositionX(Real x) {
    this->position.x() = x;
    this->transform(3, 0) = x;
}

template <typename Real>
void Transformation<Real>::setPositionY(Real y) {
    this->position.y() = y;
    this->transform(3, 1) = y;
}

template <typename Real>
void Transformation<Real>::setPositionZ(Real z) {
    this->position.z() = z;
    this->transform(3, 2) = z;
}

template <typename Real>
void Transformation<Real>::addPositionX(Real x) {
    this->position.x() += x;
    this->transform(3, 0) += x;
}

template <typename Real>
void Transformation<Real>::addPositionY(Real y) {
    this->position.y() += y;
    this->transform(3, 1) += y;
}

template <typename Real>
void Transformation<Real>::addPositionZ(Real z) {
    this->position.z() += z;
    this->transform(3, 2) += z;
}

template <typename Real>
void Transformation<Real>::setPosition(Real x, Real y, Real z) {
    this->position.set(x, y, z);
    this->transform(3, 0) = x;
    this->transform(3, 1) = y;
    this->transform(3, 2) = z;
}

template <typename Real>
void Transformation<Real>::setPosition(const Vector3<Real>& position) {
    this->position = position;
    this->transform(3, 0) = position.x();
    this->transform(3, 1) = position.y();
    this->transform(3, 2) = position.z();
}

template <typename Real>
void Transformation<Real>::setScaleX(Real sx) {
    this->scale.setX(sx);
    this->compile();
}

template <typename Real>
void Transformation<Real>::setScaleY(Real sy) {
    this->scale.setY(sy);
    this->compile();
}

template <typename Real>
void Transformation<Real>::setScaleZ(Real xz) {
    this->scale.setZ(sz);
    this->compile();
}

template <typename Real>
void Transformation<Real>::addScaleX(Real sx) {
    this->scale.x() += sx;
    this->compile();
}

template <typename Real>
void Transformation<Real>::addScaleY(Real sy) {
    this->scale.y() += sy;
    this->compile();
}

template <typename Real>
void Transformation<Real>::addScaleZ(Real sz) {
    this->scale.z() += sz;
    this->compile();
}

template <typename Real>
void Transformation<Real>::setScale(Real sx, Real sy, Real sz) {
    this->scale.set(sx, sy, sz);
    this->compile();
}

template <typename Real>
void Transformation<Real>::setScale(const Vector3<Real>& scale) {
    this->scale = scale;
    this->compile();
}

template <typename Real>
void Transformation<Real>::addRotation(const Quaternion<Real>& rotation) {
    this->rotation *= rotation;
    this->compile();
}

template <typename Real>
void Transformation<Real>::setRotation(const Quaternion<Real>& rotation) {
    this->rotation = rotation;
    this->compile();
}
    
template <typename Real>
std::ostream& operator << (std::ostream& out, const Transformation<Real>& transform) {
    out << "[Transformation]" << std::endl;
    out << "Position: " << transform.position << std::endl;
    out << "Scale: " << transform.scale << std::endl;
    out << "Rotation: " << transform.rotation << std::endl;
    out << "Matrix:" << std::endl;
    out << transform.transform << std::endl;
    return out;
}

template <typename Real>
const Vector3<Real>& Transformation<Real>::getPosition() const{ 
    return this->position;
}

template <typename Real>
const Vector3<Real>& Transformation<Real>::getScale() const {
    return scale;
}

template <typename Real>
const Quaternion<Real>& Transformation<Real>::getRotation() const {
    return this->rotation;
}

template <typename Real>
Vector3<Real>& Transformation<Real>::getPosition() {
    return this->position;
}

template <typename Real>
Vector3<Real>& Transformation<Real>::getScale() {
    return this->scale;
}

template <typename Real>
Quaternion<Real>& Transformation<Real>::getRotation() {
    return this->rotation;
}

template <typename Real>
const Matrix4<Real>& Transformation<Real>::toMatrix() const {
    return this->transform;
}

template <typename Real>
Matrix4<Real> Transformation<Real>::toTransformationMatrix() const {
    return this->transform;
}

template <typename Real>
Transformation<Real>& Transformation<Real>::operator = (const Transformation<Real>& transform) {
    if ( this == &transform ) return *this;
    this->position = transform.position;
    this->scale = transform.scale;
    this->rotation = transform.rotation;
    this->transform = transform.transform;
    return *this;
}

/* Min C. [Computer Graphics]
 * T = I
 * 
 * [ 1 0 0 0 ]
 * [ 0 1 0 0 ]
 * [ 0 0 1 0 ]
 * [ 0 0 0 1 ]
 */
template <typename Real>
Transformation<Real> Transformation<Real>::Identity() {
    Transformation<Real> result;
    result.position = Vector3<Real>();
    result.scale = Vector3<Real>(Real(1), Real(1), Real(1));
    result.rotation = Quaternion<Real>::Identity();
    result.compile();
    return result;
}

/* Min C. [Computer Graphics]
 * T = S
 *
 * [ sx 0  0  0 ]
 * [ 0  sy 0  0 ]
 * [ 0  0  sz 0 ]
 * [ 0  0  0  1 ]
 */
template <typename Real>
Transformation<Real> Transformation<Real>::Scale(Real sx, Real sy, Real sz) {
    Transformation<Real> result;
    result.setScale(sx, sy, sz);
    result.compile();
    return result;
}

/* Min C. [Computer Graphics]
 * T = T
 *
 * [ 1 0 0 x ]
 * [ 0 1 0 y ]
 * [ 0 0 1 z ]
 * [ 0 0 0 1 ]
 */
template <typename Real>
Transformation<Real> Transformation<Real>::Translate(Real x, Real y, Real z) {
    Transformation<Real> result;
    result.setPosition(x, y, z);
    result.compile();
    return result;
}

/* Min C. [Computer Graphics]
 * T = Rx(a)
 * 
 * [ 1    0        0     0 ]
 * [ 0  cos(a)  -sin(a)  0 ]
 * [ 0  sin(a)   cos(a)  0 ]
 * [ 0    0        0     1 ]
 */
template <typename Real>
Transformation<Real> Transformation<Real>::RotateX(Real angle) {
    Transformation<Real> result;
    Quaternion<Real> q(Real(1), Real(0), Real(0), angle);
    result.setRotation(q);
    result.compile();
    return result;
}

/* Min C. [Computer Graphics]
 * T = Ry(a)
 * 
 * [ cos(a)    0   sin(a)  0 ]
 * [   0       1     0     0 ]
 * [ -sin(a)   0   cos(a)  0 ]
 * [   0       0     0     1 ]
 */
template <typename Real>
Transformation<Real> Transformation<Real>::RotateY(Real angle) {
    Transformation<Real> result;
    Quaternion<Real> q(Real(0), Real(1), Real(0), angle);
    result.setRotation(q);
    result.compile();
    return result;
}

/* Min C. [Computer Graphics]
 * T = Rz(a)
 *
 * [ cos(a) -sin(a)  0   0 ]
 * [ sin(a)  cos(a)  0   0 ]
 * [   0       0     1   0 ]
 * [   0       0     0   1 ]
 */
template <typename Real>
Transformation<Real> Transformation<Real>::RotateZ(Real angle) {
    Transformation<Real> result;
    Quaternion<Real> q(Real(0), Real(0), Real(1), angle);
    result.setRotation(q);
    result.compile();
    return result;
}

template <typename Real>
void Transformation<Real>::compile(bool colMajor) {
    Matrix3<Real> rotationMatrix = this->rotation.toRotationMatrix();
    Matrix3<Real> t = rotationMatrix.apply(this->scale);
    this->transform.set(t);

    if ( colMajor ) this->transform.setRow(3, this->position.x(), this->position.y(), this->position.z(), Real(1));
    else this->transform.setColumn(3, this->position.x(), this->position.y(), this->position.z(), Real(1));
}

typedef Transformation<float> Transformationf;
typedef Transformation<double> Transformationd;
typedef Transformation<long double> Transformationld;

#endif
