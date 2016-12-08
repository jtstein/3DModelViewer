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
#ifndef QUATERNION_H
#define QUATERNION_H

#include <cstring>
#include "RotationMatrix.h"

template <typename Real>
class Quaternion;

template <typename Real>
std::ostream& operator << <> (std::ostream& out, const Quaternion<Real>& vector);

/* 
 * Quaternion: Representation of a 4x1 quaternion.
 *
 * Mathematical Definition:
 * q = (w, xi, yj, zk)
 *
 * Memory Definition:
 * [xi, yj, zk, w]
 * 
 * This implementation also foregoes vectorization (SSE).
 * This implementation is aimed at a flexibility while preserving 
 * understandability and complete modularity.
 */
template <typename Real>
class Quaternion {
    static_assert(std::is_floating_point<Real>::value, 
    "[Vector3:Type] Error: Vector3 Real type must be floating point numerical representation.");

    enum Element { X, Y, Z, W, COMPONENT_COUNT};

public:
    Quaternion(bool identity = true);
    Quaternion(const Quaternion<Real>& q);
    Quaternion(const Vector3<Real>& axis, Real angle);
    Quaternion(Real x, Real y, Real z, Real angle);
    Quaternion(Real psi, Real phi, Real theta);
    Quaternion(const RotationMatrix<Real>& rotationMatrix);
    virtual ~Quaternion();

    void normalize();
    void identity();
    bool isIdentity() const;
    void multiply(const Quaternion<Real>& q);
    void multiplyOnLeft(const Quaternion<Real>& q);
    void multiplyOnRight(const Quaternion<Real>& q);
    Quaternion<Real> normalized() const;
    RotationMatrix<Real> toRotationMatrix() const;
    Vector4<Real> toVector() const;
    Real length() const;

    void fromVector(const Vector4<Real>& v);
    void fromAxisAngle(const Vector3<Real>& axis, Real angle);
    void fromEulerAngles(Real psi, Real phi, Real theta);
    void fromRotationMatrix(const RotationMatrix<Real>& rotationMatrix);
    void fromEulerRotationX(Real psi);
    void fromEulerRotationY(Real phi);
    void fromEulerRotationZ(Real theta);

    void set(Real x, Real y, Real z, Real w);
    void set(const Vector4<Real>& v);
    void setX(Real x);
    void setY(Real y);
    void setZ(Real z);
    void setW(Real w);

	const Real& w() const;
	const Real& x() const;
	const Real& y() const;
	const Real& z() const;
    const Real& getW() const;
    const Real& getX() const;
    const Real& getY() const;
    const Real& getZ() const;

    Real& w();
	Real& x();
	Real& y();
	Real& z();
    Real& getW();
    Real& getX();
    Real& getY();
    Real& getZ();

    Quaternion<Real>& operator = (const Quaternion<Real>& q);
	Quaternion<Real>& operator *= (const Quaternion<Real>& q);
    Quaternion<Real>& operator *= (const Real scalar);
    Quaternion<Real> operator * (const Quaternion<Real>& q) const;
    Quaternion<Real> operator * (Real scalar) const;

    friend std::ostream& operator << <> (std::ostream& out, const Quaternion<Real>& v);

    static void FromAxisAngle(Real x, Real y, Real z, Real angle, Quaternion<Real>& q);
    static void FromAxisAngle(const Vector3<Real>& axis, Real angle, Quaternion<Real>& q);
    static void FromEulerAngles(Real psi, Real phi, Real theta, Quaternion<Real>& q);
    static void FromRotationMatrix(const RotationMatrix<Real>& rotationMatrix, Quaternion<Real>& q);
    static void Identity(Quaternion<Real>& q);
    static void Normalize(Quaternion<Real>& q);
    static void Conjugate(Quaternion<Real>& q);

    static Vector4<Real> ToVector(const Quaternion<Real>& q);
    static Quaternion<Real> EulerRotationX(Real psi);
    static Quaternion<Real> EulerRotationY(Real phi);
    static Quaternion<Real> EulerRotationZ(Real theta);
    static Quaternion<Real> Multiply(const Quaternion<Real>& q, const Quaternion<Real>& p);
    static Quaternion<Real> Conjugate(const Quaternion<Real>& q);
    static Quaternion<Real> Normalize(const Quaternion<Real>& q);
    static Quaternion<Real> Identity();
    static Quaternion<Real> FromRotationMatrix(const RotationMatrix<Real>& rotationMatrix);
    static Quaternion<Real> FromEulerAngles(Real psi, Real phi, Real theta);
    static Quaternion<Real> FromAxisAngle(Real x, Real y, Real z, Real angle);
    static Quaternion<Real> FromAxisAngle(const Vector3<Real>& axis, Real angle);

    static RotationMatrix<Real> ToRotationMatrix(const Quaternion<Real>& q);
    static Real Length(const Quaternion<Real>& q);
    static Real InnerProduct(const Quaternion<Real>& q, const Quaternion<Real>& p);

protected:
    Real data[COMPONENT_COUNT];
};

template <typename Real>
Quaternion<Real>::Quaternion(bool identity) {
    std::memset(this->data, 0, COMPONENT_COUNT * sizeof(Real));
    if ( identity ) this->data[W] = Real(1);
}

template <typename Real>
Quaternion<Real>::Quaternion(const Quaternion<Real>& q) {
    std::memset(this->data, 0, COMPONENT_COUNT * sizeof(Real));
    this->data[W] = Real(1);
}

template <typename Real>
Quaternion<Real>::Quaternion(const Vector3<Real>& axis, Real angle) {
    Quaternion<Real>::FromAxisAngle(axis, angle, *this);
}

template <typename Real>
Quaternion<Real>::Quaternion(Real x, Real y, Real z, Real angle) {
    Quaternion<Real>::FromAxisAngle(x, y, z, angle, *this);
}

template <typename Real>
Quaternion<Real>::Quaternion(Real psi, Real phi, Real theta) {
    Quaternion<Real>::FromEulerAngles(psi, phi, theta, *this);
}

template <typename Real>
Quaternion<Real>::Quaternion(const RotationMatrix<Real>& rotationMatrix) {
    Quaternion<Real>::FromRotationMatrix(rotationMatrix);
}

template <typename Real>
Quaternion<Real>::~Quaternion() {}

template <typename Real>
void Quaternion<Real>::normalize() {
    Quaternion<Real>::Normalize(*this);
}

template <typename Real>
void Quaternion<Real>::identity() {
    Quaternion<Real>::Identity(*this);
}

template <typename Real>
bool Quaternion<Real>::isIdentity() const {
    if ( this->data[X] == Real(0) &&
         this->data[Y] == Real(0) &&
         this->data[Z] == Real(0) &&
         this->data[W] == Real(1) )
    return true;
    else return false;
}

template <typename Real>
void Quaternion<Real>::multiply(const Quaternion<Real>& q) {
    (*this) *= q;
}

template <typename Real>
void Quaternion<Real>::multiplyOnLeft(const Quaternion<Real>& q) {
    Quaternion<Real> result = q;
    result *= (*this);
    (*this) = result;
}

template <typename Real>
void Quaternion<Real>::multiplyOnRight(const Quaternion<Real>& q) {
    (*this) *= q;
}

template <typename Real>
Quaternion<Real> Quaternion<Real>::normalized() const {
    return Quaternion<Real>::Normalize(*this);
}

template <typename Real>
RotationMatrix<Real> Quaternion<Real>::toRotationMatrix() const {
    return Quaternion<Real>::ToRotationMatrix(*this);
}

template <typename Real>
Vector4<Real> Quaternion<Real>::toVector() const {
    return Quaternion<Real>::ToVector(*this);
}

template <typename Real>
Real Quaternion<Real>::length() const {
    return Quaternion<Real>::Length(*this);
}

template <typename Real>
void Quaternion<Real>::fromVector(const Vector4<Real>& v) {
    this->data[W] = v.w();
    this->data[X] = v.x();
    this->data[Y] = v.y();
    thsi->data[Z] = v.z();
}

template <typename Real>
void Quaternion<Real>::fromAxisAngle(const Vector3<Real>& axis, Real angle) {
    Quaternion<Real>::FromAxisAngle(axis, angle, *this);
}

template <typename Real>
void Quaternion<Real>::fromEulerAngles(Real psi, Real phi, Real theta) {
    Quaternion<Real>::FromEulerAngles(psi, phi, theta, *this);
}

template <typename Real>
void Quaternion<Real>::fromRotationMatrix(const RotationMatrix<Real>& rotationMatrix) {
    Quaternion<Real>::FromRotationMatrix(rotationMatrix, *this);
}

template <typename Real>
void Quaternion<Real>::fromEulerRotationX(Real psi) {
    Quaternion<Real>::FromEulerAngles(psi, Real(0), Real(0), *this);
}

template <typename Real>
void Quaternion<Real>::fromEulerRotationY(Real phi) {
    Quaternion<Real>::FromEulerAngles(Real(0), phi, Real(0), *this);
}

template <typename Real>
void Quaternion<Real>::fromEulerRotationZ(Real theta) {
    Quaternion<Real>::FromEulerAngles(Real(0), Real(0), theta, *this);
}

template <typename Real>
void Quaternion<Real>::set(Real x, Real y, Real z, Real w) {
    this->data[W] = w;
    this->data[X] = x;
    this->data[Y] = y;
    this->data[Z] = z;
}

template <typename Real>
void Quaternion<Real>::set(const Vector4<Real>& v) {
    this->data[W] = v.w();
    this->data[X] = v.x();
    this->data[Y] = v.y();
    this->data[Z] = v.z();
}

template <typename Real>
void Quaternion<Real>::setX(Real x) {
    this->data[X] = x;
}

template <typename Real>
void Quaternion<Real>::setY(Real y) {
    this->data[Y] = y;
}

template <typename Real>
void Quaternion<Real>::setZ(Real z) {
    this->data[Z] = z;
}

template <typename Real>
void Quaternion<Real>::setW(Real w) {
    this->data[W] = w;
}

template <typename Real>
const Real& Quaternion<Real>::w() const {
    return this->data[W];
}

template <typename Real>
const Real& Quaternion<Real>::x() const {
    return this->data[X];
}

template <typename Real>
const Real& Quaternion<Real>::y() const {
    return this->data[Y];
}

template <typename Real>
const Real& Quaternion<Real>::z() const {
    return this->data[Z];
}

template <typename Real>
const Real& Quaternion<Real>::getW() const {
    return this->data[W];
}

template <typename Real>
const Real& Quaternion<Real>::getX() const {
    return this->data[X];
}

template <typename Real>
const Real& Quaternion<Real>::getY() const {
    return this->data[Y];
}

template <typename Real>
const Real& Quaternion<Real>::getZ() const {
    return this->data[Z];
}

template <typename Real>
Real& Quaternion<Real>::w() {
    return this->data[W];
}

template <typename Real>
Real& Quaternion<Real>::x() {
    return this->data[X];
}

template <typename Real>
Real& Quaternion<Real>::y() {
    return this->data[Y];
}

template <typename Real>
Real& Quaternion<Real>::z() {
    return this->data[Z];
}

template <typename Real>
Real& Quaternion<Real>::getW() {
    return this->data[W];
}

template <typename Real>
Real& Quaternion<Real>::getX() {
    return this->data[X];
}

template <typename Real>
Real& Quaternion<Real>::getY() {
    return this->data[Y];
}

template <typename Real>
Real& Quaternion<Real>::getZ() {
    return this->data[Z];
}

template <typename Real>
Quaternion<Real>& Quaternion<Real>::operator = (const Quaternion<Real>& q) {
    if ( this == &q ) return (*this);
	this->data[X] = q.data[X];
	this->data[Y] = q.data[Y];
	this->data[Z] = q.data[Z];
	this->data[W] = q.data[W];
	return (*this);
}

template <typename Real>
Quaternion<Real>& Quaternion<Real>::operator *= (const Quaternion<Real>& q) {
    this->data[W] = this->data[W] * q.data[W] - this->data[X] * q.data[X] - this->data[Y] * q.data[Y] - this->data[Z] * q.data[Z];
    this->data[X] = this->data[W] * q.data[X] + this->data[X] * q.data[W] + this->data[Y] * q.data[Z] - this->data[Z] * q.data[Y];
    this->data[Y] = this->data[W] * q.data[Y] - this->data[X] * q.data[Z] + this->data[Y] * q.data[W] + this->data[Z] * q.data[X];
    this->data[Z] = this->data[W] * q.data[Z] + this->data[X] * q.data[Y] - this->data[Y] * q.data[X] + this->data[Z] * q.data[W];
    return (*this);
}

template <typename Real>
Quaternion<Real>& Quaternion<Real>::operator *= (const Real scalar) {
    this->data[X] *= scalar;
    this->data[Y] *= scalar;
    this->data[Z] *= scalar;
    this->data[W] *= scalar;
    return (*this);
}

template <typename Real>
Quaternion<Real> Quaternion<Real>::operator * (const Quaternion<Real>& q) const {
    Quaternion<Real> result = (*this);
    result *= q;
    return result
}

template <typename Real>
Quaternion<Real> Quaternion<Real>::operator * (Real scalar) const {
    Quaternion<Real> result = (*this);
    result *= scalar;
    return result;
}

template <typename Real>
std::ostream& operator << (std::ostream& out, const Quaternion<Real>& q) {
    out << "[x:" << q.data[Quaternion<Real>::X] << " y:" << q.data[Quaternion<Real>::Y] << " z:" << q.data[Quaternion<Real>::Z] << " w:" << q.data[Quaternion<Real>::W] << "]" << std::endl;
    return out;
}

template <typename Real>
void Quaternion<Real>::FromAxisAngle(Real x, Real y, Real z, Real angle, Quaternion<Real>& q) {
    Real halfAngle = angle * Real(0.5);
	Real sinHalfAngle = std::sin(halfAngle);

	q.data[W] = std::cos(angle * Real(0.5));
	q.data[X] = x * sinHalfAngle;
	q.data[Y] = y * sinHalfAngle;
	q.data[Z] = z * sinHalfAngle;
}

template <typename Real>
void Quaternion<Real>::FromAxisAngle(const Vector3<Real>& axis, Real angle, Quaternion<Real>& q) {
    Quaternion<Real>::FromAxisAngle(axis.x(), axis.y(), axis.z(), angle, q);
}

template <typename Real>
void Quaternion<Real>::FromEulerAngles(Real psi, Real phi, Real theta, Quaternion<Real>& q) {
    Real halfTheta = theta * Real(0.5);
	Real halfPhi = phi * Real(0.5);
	Real halfPsi = psi * Real(0.5);

	Real cosHalfPhi = std::cos(halfPhi);
	Real sinHalfPhi = std::sin(halfPhi);

	Real cosHalfTheta = std::cos(halfTheta);
	Real sinHalfTheta = std::sin(halfTheta);

	Real cosHalfPsi = std::cos(halfPsi);
	Real sinHalfPsi = std::sin(halfPsi);

	q.data[W] = cosHalfPhi * cosHalfTheta * cosHalfPsi + sinHalfPhi * sinHalfTheta * sinHalfPsi;
	q.data[Y] = sinHalfPhi * cosHalfTheta * cosHalfPsi - cosHalfPhi * sinHalfTheta * sinHalfPsi;
	q.data[Z] = cosHalfPhi * sinHalfTheta * cosHalfPsi + sinHalfPhi * cosHalfTheta * sinHalfPsi;
	q.data[X] = cosHalfPhi * cosHalfTheta * sinHalfPsi - sinHalfPhi * sinHalfTheta * cosHalfPsi;
}

template <typename Real>
void Quaternion<Real>::FromRotationMatrix(const RotationMatrix<Real>& m, Quaternion<Real>& q) {
    Real trace = m(0, 0) + m(1, 1) + m(2, 2);
    Real quat[4];

    if ( trace >= Real(0) ) {
        Real s = std::sqrt(trace + m(3, 3));
        quat[0] = s * Real(0.5);
        s = Real(0.5) / s;
        quat[1] = (m(1, 2) - m(2, 1)) * s;
        quat[2] = (m(2, 0) - m(0, 2)) * s;
        quat[3] = (m(0, 1) - m(1, 0)) * s;
    }
    else {
        std::size_t i = 0;
        if ( m(1, 1) > m(0, 0) ) i = 1;
        if ( m(2, 2) > m(i, i) ) i = 2;

        std::size_t j = (i+1) % 3;
        std::size_t k = (j+1) % 3;
        Real s = std::sqrt(m(3, 3) + (m(i, i) - (m(j, j) + m(k, k))));
        quat[i+1] = s * Real(0.5);
        s = Real(0.5) / s;
        quat[j+1] = (rot(i, j) + m(j, i)) * s;
        quat[k+1] = (rot(i, k) + m(k, i)) * s;
        quat[0] = (rot(j, k) - m(k , j)) * s;
    }

    if ( rot(3, 3) != Real(1) ) {
        Real temp = Real(1) / std::sqrt(m(3, 3));
        quat[0] *= temp;
        quat[1] *= temp;
        quat[2] *= temp;
        quat[3] *= temp;
    }

    q.set(quat[1], quat[2], quat[3], quat[0]);
}

template <typename Real>
void Quaternion<Real>::Identity(Quaternion<Real>& q) {
    q.data[X] = Real(0);
    q.data[Y] = Real(0);
    q.data[Z] = Real(0);
    q.data[W] = Real(1);
}

template <typename Real>
void Quaternion<Real>::Normalize(Quaternion<Real>& q) {
	Real len = Length(q);
	if ( len == Real(0) ) return Quaternion<Real>();

    Real invLen = Real(1) / len;
	q.data[X] = q.data[X] * invLen;
	q.data[Y] = q.data[Y] * invLen;
	q.data[Z] = q.data[Z] * invLen;
	q.data[W] = q.data[W] * invLen;
}

template <typename Real>
void Quaternion<Real>::Conjugate(Quaternion<Real>& q) {
    q.data[X] = -q.data[X];
    q.data[Y] = -q.data[Y];
    q.data[Z] = -q.data[Z];
    q.data[W] = q.data[W];
}

template <typename Real>
Vector4<Real> Quaternion<Real>::ToVector(const Quaternion<Real>& q) {
    Vector4<Real> result;
    result.w() = this->data[W];
    result.x() = this->data[X];
    result.y() = this->data[Y];
    result.z() = this->data[Z];
    return result;
}

template <typename Real>
Quaternion<Real> Quaternion<Real>::EulerRotationX(Real psi) {
    return Quaternion<Real>::FromEulerAngles(psi, Real(0), Real(0));
}

template <typename Real>
Quaternion<Real> Quaternion<Real>::EulerRotationY(Real phi) {
    return Quaternion<Real>::FromEulerAngles(Real(0), phi, Real(0));
}

template <typename Real>
Quaternion<Real> Quaternion<Real>::EulerRotationZ(Real theta) {
    return Quaternion<Real>::FromEulerAngles(Real(0), Real(0), theta);
}

/* http://www.cprogramming.com/tutorial/3d/quaternions.html */
template <typename Real>
Quaternion<Real> Quaternion<Real>::Multiply(const Quaternion<Real>& q, const Quaternion<Real>& p) {
    
}

template <typename Real>
Quaternion<Real> Quaternion<Real>::Conjugate(const Quaternion<Real>& q) {
    Quaternion<Real> result = q;
    return result.conjugate();
}

template <typename Real>
Quaternion<Real> Quaternion<Real>::Normalize(const Quaternion<Real>& q) {
    Quaternion<Real> result = q;
    return result.normalized();
}

template <typename Real>
Quaternion<Real> Quaternion<Real>::Identity() {
    return Quaternion<Real>(true);
}

/* 
 * Graphics Gems IV p. 213 - The quaternion order for the algorithm is
 * [w, x, y, z]. The resulting quaternion is rearranged to [x, y, z, w].
 */
template <typename Real>
Quaternion<Real> Quaternion<Real>::FromRotationMatrix(const RotationMatrix<Real>& m) {
    Quaternion<Real> result;
    Real trace = m(0, 0) + m(1, 1) + m(2, 2);
    Real q[4];

    if ( trace >= Real(0) ) {
        Real s = std::sqrt(trace + m(3, 3));
        q[0] = s * Real(0.5);
        s = Real(0.5) / s;
        q[1] = (m(1, 2) - m(2, 1)) * s;
        q[2] = (m(2, 0) - m(0, 2)) * s;
        q[3] = (m(0, 1) - m(1, 0)) * s;
    }
    else {
        std::size_t i = 0;
        if ( m(1, 1) > m(0, 0) ) i = 1;
        if ( m(2, 2) > m(i, i) ) i = 2;

        std::size_t j = (i+1) % 3;
        std::size_t k = (j+1) % 3;
        Real s = std::sqrt(m(3, 3) + (m(i, i) - (m(j, j) + m(k, k))));
        q[i+1] = s * Real(0.5);
        s = Real(0.5) / s;
        q[j+1] = (rot(i, j) + m(j, i)) * s;
        q[k+1] = (rot(i, k) + m(k, i)) * s;
        q[0] = (rot(j, k) - m(k , j)) * s;
    }

    if ( rot(3, 3) != Real(1) ) {
        Real temp = Real(1) / std::sqrt(m(3, 3));
        q[0] *= temp;
        q[1] *= temp;
        q[2] *= temp;
        q[3] *= temp;
    }

    result.set(q[1], q[2], q[3], q[0]);
    return result;
}

template <typename Real>
Quaternion<Real> Quaternion<Real>::FromEulerAngles(Real psi, Real phi, Real theta) {
    Quaternion<Real> result;
    Quaternion<Real>::FromEulerAngles(psi, phi, theta, result);
	return result;
}

template <typename Real>
Quaternion<Real> Quaternion<Real>::FromAxisAngle(Real x, Real y, Real z, Real angle) {
    Quaternion<Real> result;
    Quaternion<Real>::FromAxisAngle(x, y, z, angle, result);
    return result;
}

template <typename Real>
Quaternion<Real> Quaternion<Real>::FromAxisAngle(const Vector3<Real>& axis, Real angle) {
    Quaternion<Real> result;
    Quaternion<Real>::FromAxisAngle(axis, angle, result);
    return result;
}

/* http://www.cprogramming.com/tutorial/3d/quaternions.html */
/* Mathematics for 3D Grame Programming and Computer Graphics (Eric Lengyel) p. 92 */
template <typename Real>
RotationMatrix<Real> Quaternion<Real>::ToRotationMatrix(const Quaternion<Real>& q) {
    RotationMatrix<Real> rot;
    Real x = q.data[X];
    Real y = q.data[Y];
    Real z = q.data[Z];
    Real w = q.data[W];

    Real xs = x*x;
    Real ys = y*y;
    Real zs = z*z;
    Real o = Real(1);
    Real t = Real(2);

    Real a11 = o - t*ys - t*zs;
    Real a12 = t*x*y - t*w*z;
    Real a13 = t*x*z + t*w*y;
    Real a21 = t*x*y + t*w*z;
    Real a22 = o - t*xs - t*zs;
    Real a23 = t*y*z - t*w*x;
    Real a31 = t*x*z - t*w*y;
    Real a32 = t*y*z + 2*w*x;
    Real a33 = o - t*xs - t*ys;

    rot.set(a11, a12, a13, a21, a22, a23, a31, a32, a33);
    return rot;
}

template <typename Real>
Real Quaternion<Real>::Length(const Quaternion<Real>& q) {
    Real length = std::sqrt(
		std::pow(q.data[X], Real(2)) + 
		std::pow(q.data[Y], Real(2)) + 
		std::pow(q.data[Z], Real(2)) +
		std::pow(q.data[W], Real(2))
    );
	return length;
}

template <typename Real>
Real Quaternion<Real>::InnerProduct(const Quaternion<Real>& q, const Quaternion<Real>& p) {
    return q.data[X] * p.data[X] + q.data[Y] * p.data[Y] + q.data[Z] * p.data[Z] + q.data[W] * p.data[W];
}

typedef Quaternion<float> Quaternionf;
typedef Quaternion<double> Quaterniond;
typedef Quaternion<long double> Quaternionld;

#endif
