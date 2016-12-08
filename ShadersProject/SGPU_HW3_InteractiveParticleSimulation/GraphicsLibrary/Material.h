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
#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <type_traits>
#include "Color4.h"

#include <gl/glew.h>
#include <gl/freeglut.h>

template <typename Real> 
class Material;

/*
 * Due to the templated nature of the Color4 class a foreward declaration of
 * the standard out stream operator must be defined.
 */
template <typename Real>
std::ostream& operator << (std::ostream& out, const Material<Real>& Material);

template <typename Real>
class Material {
    static_assert(std::is_floating_point<Real>::value,
    "[Material:Type] Error: Material Real type must be floating point numerical representation.");

public:
    Material();
    Material(const Material<Real>& material);
    Material(const Color4<Real>& a, const Color4<Real>& d, const Color4<Real>& s);
    virtual ~Material();

    void render() const;

    void setAmbient(const Color4<Real>& a);
    void setAmbient(Real r, Real g, Real b);
    void setDiffuse(const Color4<Real>& d);
    void setDiffuse(Real r, Real g, Real b);
    void setSpecular(const Color4<Real>& s);
    void setSpecular(Real r, Real g, Real b);
    void setShininess(int s);

    Color4<Real>& getAmbient();
    Color4<Real>& getDiffuse();
    Color4<Real>& getSpecular();
    int& getShininess();

    const Color4<Real>& getAmbient() const;
    const Color4<Real>& getDiffuse() const;
    const Color4<Real>& getSpecular() const;
    const int& getShininess() const;
    
    friend std::ostream& operator << (std::ostream& out, const Material<Real>& Material);;

protected:
    Color4<Real> ambient;
    Color4<Real> diffuse;
    Color4<Real> specular;

    int shininess;
};

template <typename Real>
Material<Real>::Material() {
    this->ambient = Color4<Real>(Real(0.1), Real(0.1), Real(0.1), Real(0));
    this->diffuse = Color4<Real>(Real(0.6), Real(0.6), Real(0.6), Real(0));
    this->specular = Color4<Real>(Real(0.2), Real(0.2), Real(0.2), Real(0));
    this->shininess = 10;
}

template <typename Real>
Material<Real>::Material(const Material<Real>& material) {
    this->ambient = material.ambient;
    this->diffuse = material.diffuse;
    this->specular = material.specular;
    this->shininess = material.shininess;
}

template <typename Real>
Material<Real>::Material(const Color4<Real>& a, const Color4<Real>& d, const Color4<Real>& s) {
    this->ambient = Color4<Real>(a.r(), a.g(), a.b(), Real(1));
    this->diffuse = Color4<Real>(d.r(), d.g(), d.b(), Real(1));
    this->specular = Color4<Real>(s.r(), s.g(), s.b(), Real(1));
}

template <typename Real>
Material<Real>::~Material() {}

template <typename Real>
void Material<Real>::render() const {
    /* Material type must be float */
    glMaterialfv(GL_FRONT, GL_AMBIENT, this->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, this->diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, this->specular);
	glMaterialiv(GL_FRONT, GL_SHININESS, &this->shininess);
}

template <typename Real>
void Material<Real>::setAmbient(const Color4<Real>& a) {
    this->ambient = a;
}

template <typename Real>
void Material<Real>::setAmbient(Real r, Real g, Real b) {
    this->ambient = Color4<Real>(r, g, b, Real(1));
}

template <typename Real>
void Material<Real>::setDiffuse(const Color4<Real>& d) {
    this->diffuse = d;
}

template <typename Real>
void Material<Real>::setDiffuse(Real r, Real g, Real b) {
    this->diffuse = Color4<Real>(r, g, b, Real(1));
}

template <typename Real>
void Material<Real>::setSpecular(const Color4<Real>& s) {
    this->specular = Color4<Real>(s.r(), s.g(), s.b(), Real(1));
}

template <typename Real>
void Material<Real>::setSpecular(Real r, Real g, Real b) {
    this->specular = Color4<Real>(r, g, b, Real(1));
}

template <typename Real>
void Material<Real>::setShininess(int s) {
    this->shininess = s;
}

template <typename Real>
Color4<Real>& Material<Real>::getAmbient() {
    return this->ambient;
}

template <typename Real>
Color4<Real>& Material<Real>::getDiffuse() {
    return this->diffuse;
}

template <typename Real>
Color4<Real>& Material<Real>::getSpecular() {
    return this->specular;
}

template <typename Real>
int& Material<Real>::getShininess() {
    return this->shininess;
}

template <typename Real>
const Color4<Real>& Material<Real>::getAmbient() const {
    return this->ambient;
}

template <typename Real>
const Color4<Real>& Material<Real>::getDiffuse() const {
    return this->diffuse;
}

template <typename Real>
const Color4<Real>& Material<Real>::getSpecular() const {
    return this->specular;
}

template <typename Real>
const int& Material<Real>::getShininess() const {
    return this->shininess;
}

#endif
