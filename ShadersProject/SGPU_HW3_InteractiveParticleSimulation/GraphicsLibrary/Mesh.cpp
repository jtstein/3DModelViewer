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
#include "Mesh.h"
#include "ObjMesh.h"
#include <unordered_map>
#include <gl/glew.h>
#include <gl/freeglut.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const static unsigned int POSITION_LOC = 0;
const static unsigned int NORMAL_LOC = 1;	
const static unsigned int TANGENT_LOC = 2;
const static unsigned int TEXTURE_COORD_LOC = 3;
const static unsigned int COLOR_LOC = 4;

Mesh::Mesh() {
    this->transform = Transformation<float>::Identity();
    this->shader = nullptr;
}

Mesh::Mesh(const Mesh& mesh) {
    this->transform = mesh.transform;
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &this->vboVertex);
    glDeleteBuffers(1, &this->vboIndex);
}

/* http://www.terathon.com/code/tangent.html */
bool CalculateTangents(std::vector<Vertex>& vertices, std::vector<TriangleFace>& faces) {
    if ( vertices.size() == 0 ) {
		std::cerr << "[Mesh:calculateTangents] Error: Vertex array of length 0." << std::endl;
		return false;
	}

    if ( faces.size() == 0 ) {
		std::cerr << "[Mesh:calculateNormals] Error: Face count = 0." << std::endl;
		return false;
	}

    std::size_t triangleCount = faces.size();
    std::vector<Vector3f> tan1 = std::vector<Vector3f>(vertices.size());
    std::vector<Vector3f> tan2 = std::vector<Vector3f>(vertices.size());

    std::size_t i0 = 0, i1 = 0, i2 = 0;
    Vector3f p1, p2, p3;
    Vector3f w1, w2, w3;
    for ( std::size_t i = 0; i < triangleCount; i++ ) {
        i0 = faces[i].indices[A];
        i1 = faces[i].indices[B];
        i2 = faces[i].indices[C];

        p1 = vertices[i0].position;
		p2 = vertices[i1].position;
		p3 = vertices[i2].position;

        w1 = vertices[i0].textureCoord;
        w2 = vertices[i1].textureCoord;
        w3 = vertices[i2].textureCoord;

        float x1 = p2.x() - p1.x();
        float x2 = p3.x() - p1.x();
        float y1 = p2.y() - p1.y();
        float y2 = p3.y() - p1.y();
        float z1 = p2.z() - p1.z();
        float z2 = p3.z() - p1.z();

        float s1 = w2.x() - w1.x();
        float s2 = w3.x() - w1.x();
        float t1 = w2.y() - w1.y();
        float t2 = w3.y() - w1.y();

        float r = 1.0f / (s1 * t2 - s2 * t1);
        Vector3f sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
        Vector3f tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

        tan1[i0] += sdir;
        tan1[i1] += sdir;
        tan1[i2] += sdir;
        
        tan2[i0] += tdir;
        tan2[i1] += tdir;
        tan2[i2] += tdir;
    }

    for ( std::size_t i = 0; i < vertices.size(); i++ ) {
        const Vector3f& n = vertices[i].normal;
        const Vector3f& t = tan1[i];

        vertices[i].tangent = Vector4f((t - n * (float)Vector3f::Dot(n, t)).normalized());
        if ( vertices[i].tangent.isEquivalent(Vector3f::Zero(), 0.01f) ) {
            if ( n.isEquivalent(Vector3f::UnitY(), 1.0e-4f) )
                vertices[i].tangent = Vector3f::UnitX();
            if ( n.isEquivalent(Vector3f::UnitNY(), 1.0e-4f) )
                vertices[i].tangent = Vector3f::UnitX();
        }

        if ( Vector3f::Dot(Vector3f::Cross(n, t), tan2[i]) < 0.0f ) vertices[i].tangent.w() = -1.0f;
        else vertices[i].tangent.w() = 1.0f;
    }

    return false;
}

bool CalculateNormals(
    const std::vector<unsigned int>& indices,
    const std::vector<Vector3f>& vertices,
    std::vector<Vector3f>& normals
    ) {

    if ( vertices.size() == 0 ) {
		std::cerr << "[Mesh:calculateNormals] Error: Vertex array of length 0." << std::endl;
		return false;
	}

	if ( indices.size() == 0 ) {
		std::cerr << "[Mesh:calculateNormals] Error: Face count = 0." << std::endl;
		return false;
	}

	std::vector<unsigned int> normalCounts = std::vector<unsigned int>(vertices.size());
    normals = std::vector<Vector3f>(vertices.size());
	std::size_t triangleCount = indices.size() / TRIANGLE_EDGE_COUNT;

	std::size_t i0 = 0, i1 = 0, i2 = 0;
	Vector3f p0, p1, p2, a, b, faceNormal;
    unsigned int index = 0;
	for ( std::size_t i = 0; i < triangleCount; i++) {
		i0 = indices[index];
        i1 = indices[index + 1];
        i2 = indices[index + 2];

		p0 = vertices[i0];
		p1 = vertices[i1];
		p2 = vertices[i2];

		a = p1 - p0;
		b = p2 - p0;

		faceNormal = a.cross(b);
		faceNormal.normalize();

        normals[i0] += faceNormal;
        normals[i1] += faceNormal;
        normals[i2] += faceNormal;

		normalCounts[i0] += 1;
		normalCounts[i1] += 1;
		normalCounts[i2] += 1;

        index += TRIANGLE_EDGE_COUNT;
	}

	for ( unsigned int i = 0; i < vertices.size(); i++ ) {
        normals[i] = normals[i] / static_cast<float>(normalCounts[i]);
        normals[i].normalize();
	}

	return true;
}

bool Decompress(
        const std::vector<unsigned int>& indices,
        const std::vector<unsigned int>& normalIndices,
        const std::vector<unsigned int>& textureIndices,
        const std::vector<Vector3f>& vertices, 
        const std::vector<Vector3f>& normals, 
        const std::vector<Vector3f>& textureCoords,
        const std::vector<Vector4f>& tangents,
        std::vector<Vertex>& outVertices,
        std::vector<TriangleFace>& outFaces
    ) {

    VertexSet vertexSet;
    unsigned int triangleCount = static_cast<unsigned int>(indices.size()) / TRIANGLE_EDGE_COUNT;

    unsigned int index = 0;
    unsigned int freeIndex = 0;
    Vector3f v1, v2, v3;
    Vector3f n1, n2, n3;
    Vector3f t1, t2, t3;
    TriangleFace face;
    Vertex v;

    for ( unsigned int i = 0; i < triangleCount; i++ ) {

        unsigned int vIndex, nIndex, tIndex;
        for ( unsigned int j = 0; j < TRIANGLE_EDGE_COUNT; j++ ) {
            vIndex = indices[index + j];
            nIndex = normalIndices[index + j];
            tIndex = textureIndices[index + j];

            v.position = vertices[vIndex];
            v.normal = normals[nIndex];
            v.textureCoord = textureCoords[tIndex];

            VertexSet::const_iterator it = vertexSet.find(v);

            if ( it != vertexSet.end() ) {
                face.indices[j] = vertexSet.find(v)->second;
            }
            else {
                vertexSet.insert(std::make_pair(v, freeIndex));
                face.indices[j] = freeIndex;
                outVertices.push_back(v);
                freeIndex++;
            }
        }

        outFaces.push_back(face);
        index += TRIANGLE_EDGE_COUNT;
    }

    return true;
}

bool Mesh::load(const std::string& filename) {
    std::shared_ptr<ObjMesh> mesh = nullptr;

    if ( !LoadObjMesh(filename, mesh) ) return false;

    if ( mesh->faces.size() > 0 ) {
        if ( mesh->faces[0].type != TRIANGLE ) {
            std::cerr << "[Mesh:load] Error: Only triangle-face Obj files supported." << std::endl;
            return false;
        }
    }

    this->name = mesh->name;
    std::vector<Vector3f> normals;
    std::vector<Vector4f> tangents;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> textureIndices;
    std::vector<unsigned int> normalIndices;

    //--------------------------------------------------------------------------
    // Copy the face indices from the *.obj file to this mesh.
    //--------------------------------------------------------------------------
    indices.resize(mesh->faces.size() * TRIANGLE_EDGE_COUNT);
    normalIndices.resize(mesh->faces.size() * TRIANGLE_EDGE_COUNT);
    textureIndices.resize(mesh->faces.size() * TRIANGLE_EDGE_COUNT);
    unsigned int index = 0;
    for ( unsigned int i = 0; i < mesh->faces.size(); i++ ) {
        indices[index] = static_cast<unsigned int>(mesh->faces[i].vertexIndices[A]);
        indices[index + 1] = static_cast<unsigned int>(mesh->faces[i].vertexIndices[B]);
        indices[index + 2] = static_cast<unsigned int>(mesh->faces[i].vertexIndices[C]);

        normalIndices[index] = static_cast<unsigned int>(mesh->faces[i].normalIndices[A]);
        normalIndices[index + 1] = static_cast<unsigned int>(mesh->faces[i].normalIndices[B]);
        normalIndices[index + 2] = static_cast<unsigned int>(mesh->faces[i].normalIndices[C]);

        textureIndices[index] = static_cast<unsigned int>(mesh->faces[i].textureIndices[A]);
        textureIndices[index + 1] = static_cast<unsigned int>(mesh->faces[i].textureIndices[B]);
        textureIndices[index + 2] = static_cast<unsigned int>(mesh->faces[i].textureIndices[C]);

        index += TRIANGLE_EDGE_COUNT;
    }

    //--------------------------------------------------------------------------
    // Calcualte the vertex normals, tangents, and face normals.
    //--------------------------------------------------------------------------
    normals.resize(mesh->normals.size());
    for ( unsigned int i = 0; i < mesh->normals.size(); i++ )
        normals[i] = mesh->normals[i];

    //--------------------------------------------------------------------------
    // Decompress the OBJ file format for rendering.
    //--------------------------------------------------------------------------
    Decompress(indices, normalIndices, textureIndices, mesh->vertices, normals, mesh->textureCoordinates, tangents, this->vertices, this->faces);
    CalculateTangents(this->vertices, this->faces);
    
    //--------------------------------------------------------------------------
    // Set all colors to black since they are not provided by an OBJ file.
    //--------------------------------------------------------------------------
    for ( unsigned int i = 0; i < this->vertices.size(); i++ )
        this->vertices[i].color = Color3f(0.0f, 0.0f, 0.0f);

    this->constructOnGPU();
    return true;
}

bool Mesh::loadShader(const std::string& vertexFilename, const std::string& fragmentFilename) {
    this->shader = std::make_shared<Shader>();

    if ( !shader->load(vertexFilename, fragmentFilename) ) {
        std::cerr << "[Mesh:loadShader] Error: Could not load shader." << std::endl;
        return false;
    }

    if ( !shader->compile() ) {
        std::cerr << "[Mesh:loadShader] Error: Could not compile shader." << std::endl;
        return false;
    }

    glBindAttribLocation(this->shader->getProgramID(), POSITION_LOC, "position");
    glBindAttribLocation(this->shader->getProgramID(), NORMAL_LOC, "normal");
    glBindAttribLocation(this->shader->getProgramID(), TANGENT_LOC, "tangent");
    glBindAttribLocation(this->shader->getProgramID(), TEXTURE_COORD_LOC, "textureCoordinate");
    glBindAttribLocation(this->shader->getProgramID(), COLOR_LOC, "color");

    if ( !shader->link() ) {
        std::cerr << "[Mesh:loadShader] Error: Could not link shader program." << std::endl;
        return false;
    }

    return true;
}

bool Mesh::save(const std::string& filename) {
    return true;
}

void Mesh::beginRender() const {
    if ( this->shader != nullptr ) this->shader->enable();

    glBindBuffer(GL_ARRAY_BUFFER, this->vboVertex);

    //--------------------------------------------------------------------------
    // Vertex position data is the first component in the vertex structure so
    // it is loaded first (with a byte offset of 0).
    //--------------------------------------------------------------------------
    glEnableVertexAttribArray(POSITION_LOC);
    glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));

    //--------------------------------------------------------------------------
    // Vertex normals occupy the second element of the vertex and must be
    // offset by 3 * sizeof(float) bytes due to the position taking the first
    // 3 * 4 = 12 bytes of the structure.
    //--------------------------------------------------------------------------
    glEnableVertexAttribArray(NORMAL_LOC);
    glVertexAttribPointer(NORMAL_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(3 * sizeof(float)));
    
    //--------------------------------------------------------------------------
    // Vertex tangents are offset by 6 * sizeof(float) due to the position
    // and normal values.
    //--------------------------------------------------------------------------
    glEnableVertexAttribArray(TANGENT_LOC);
    glVertexAttribPointer(TANGENT_LOC, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(6 * sizeof(float)));

    //--------------------------------------------------------------------------
    // Vertex texture coordinate is offset by 10 * sizeof(float) due to the
    // position, normal, and tangent.
    //--------------------------------------------------------------------------
    glEnableVertexAttribArray(TEXTURE_COORD_LOC);
    glVertexAttribPointer(TEXTURE_COORD_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(10 * sizeof(float)));

    //--------------------------------------------------------------------------
    // Vertex color is offset by 13 * sizeof(float) due to the position,
    // normal, tangent, and texture coordinate.
    //--------------------------------------------------------------------------
    glEnableVertexAttribArray(COLOR_LOC);
    glVertexAttribPointer(COLOR_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(13 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboIndex);
}

void Mesh::endRender() const {
    //--------------------------------------------------------------------------
    // Render this mesh. Based on the vertex and element indices uploaded to the
    // GPU (see constructOnGPU), this function will call the GPU to render all
    // of the elements based on the face indices.
    //--------------------------------------------------------------------------
    glDrawRangeElements(GL_TRIANGLES, 0, static_cast<GLsizei>((this->faces.size() * TRIANGLE_EDGE_COUNT) - 1), static_cast<GLsizei>(this->faces.size() * TRIANGLE_EDGE_COUNT), GL_UNSIGNED_INT, 0);

    if ( this->shader != nullptr ) this->shader->disable();
}

void Mesh::setName(const std::string& name) {
    this->name = name;
}

void Mesh::setShader(const std::shared_ptr<Shader>& shader) {
    this->shader = shader;
}

bool Mesh::setDiffuseTexture(const std::string& filename) {
    if ( this->shader == nullptr ) return false;
    return this->shader->loadDiffuseTexture(filename);
}

bool Mesh::setNormalTexture(const std::string& filename) {
    if ( this->shader == nullptr ) return false;
    return this->shader->loadNormalTexture(filename);
}

bool Mesh::setSpecularTexture(const std::string& filename) {
    if ( this->shader == nullptr ) return false;
    return this->shader->loadSpecularTexture(filename);
}

bool Mesh::setHeightmapTexture(const std::string& filename) {
    if ( this->shader == nullptr ) return false;
    return this->shader->loadHeightmapTexture(filename);
}

void Mesh::setPosition(float x, float y, float z) {
    this->transform.setPosition(x, y, z);
}

void Mesh::setPosition(const Vector3f& position) {
    this->transform.setPosition(position);
}

void Mesh::setScale(float sx, float sy, float sz) {
    this->transform.setScale(sx, sy, sz);
}

void Mesh::setScale(const Vector3f& scale) {
    this->transform.setScale(scale);
}

void Mesh::setRotation(const Quaternionf& rotation) {
    this->transform.setRotation(rotation);
}

std::string& Mesh::getName() {
    return this->name;
}

const std::string& Mesh::getName() const {
    return this->name;
}

Transformationf& Mesh::getTransform() {
    return this->transform;
}

const Transformationf& Mesh::getTransform() const {
    return this->transform;
}

std::shared_ptr<Shader>& Mesh::getShader() {
    return this->shader;
}

const std::shared_ptr<Shader>& Mesh::getShader() const {
    return this->shader;
}

bool Mesh::constructOnGPU() {
    //--------------------------------------------------------------------------
    // Vertex Buffer Object (VBO): Responsible for storing the vertex data of
    // this mesh. This segment first creates a new buffer on the GPU and then
    // fills it using the vertex data loaded in this class vertex array.
    // See (Vertex). This is the main OpenGL function call that will transfer
    // the vertex data from main memory to GPU memory. The static draw flag 
    // is used because this class represents a simple model that does not 
    // change over time. The following vertex attribute pointers define
    // how and where to define each unique vertex attribute based on this
    // original set of data (position, normal, tangent, texCoord).
    //--------------------------------------------------------------------------
    glGenBuffers(1, &this->vboVertex);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboVertex);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

    //--------------------------------------------------------------------------
    // This segment creates a new element buffer (for indexed geometry) for
    // defining the adjacencies or faces of the loaded set of vertices. This
    // section uses a trick that requires a face to be defined as a simple
    // structure containing the three indices of a face. These structures must
    // be contiguous in memory to work correctly.
    //--------------------------------------------------------------------------
    glGenBuffers(1, &this->vboIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->faces.size() * TRIANGLE_EDGE_COUNT * sizeof(unsigned int), &this->faces[0].indices[0], GL_STATIC_DRAW);
    
    return true;
}
