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
#ifndef ObjMesh_H
#define ObjMesh_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <Mathematics.h>

struct ObjMesh;

/* *.obj Supported geometry face types */
enum ObjFaceType { TRIANGLE, QUAD, POLYGON };

/*
 * Simple mesh loader. This function allows a single *.obj file to
 * be read with the first mesh automatically extracted from the file.
 */
bool LoadObjMesh(const std::string& filename, std::shared_ptr<ObjMesh>& mesh);

struct Obj_Face {
    ObjFaceType type;

    std::vector<std::size_t> vertexIndices;
    std::vector<std::size_t> textureIndices;
    std::vector<std::size_t> normalIndices;

    std::size_t smoothingGroupIndex;
    std::size_t groupIndex;
    std::size_t materialIndex;
};

struct ObjMesh {
    ObjMesh(const std::string& name) {
        this->name = name;
    }

    std::string name;

    std::vector<Vector3f> vertices;
    std::vector<Vector3f> normals;
    std::vector<Vector3f> textureCoordinates;

    std::vector<Obj_Face> faces;
};

typedef std::map<std::size_t, std::string> NameMap;
typedef std::vector<std::shared_ptr<ObjMesh> > MeshArray;
typedef std::vector<std::string> StringArray;

/* 
 * This class provides an Obj file definition into a set of meshes. Obj material 
 * libraries are supported as external references. The implementation of this
 * Wavefront Obj reader is focused on the readability and clearness of the 
 * presented code and does not provide a highly optimized Obj reader. This 
 * implementation utilizes the definition of an Wavefront Obj file below:
 *
 * # Obj Comment
 * # Vertices (x, y, z) form.
 * v 0.123 0.456 0.789
 * 
 * # Texture Coordinates (u, v, w) form where u, v, w exist within 0.0 - 1.0
 * vt 1.0 0.0 0.0
 * 
 * # Normals (x, y, z) form (not guarenteed to be unit length)
 * vn 1.0 0.0 0.0
 *
 * # Group
 * g GroupName
 *
 * # Object
 * o ObjectName
 *
 * # Face Defintions (3 potential forms):
 * f 1 2 3
 * f 1/2 3/4 5/6
 * f 1/2/3 4/5/6 7/8/9
 *
 * # Smoothing Groups
 * s GroupIndex
 */
class ObjFile {
public:
    ObjFile();
    ~ObjFile();

    /*
     * Loads a set of Obj mesh definitions from an Obj file.
     * 
     * @param filename - The name of the Obj file to be read (include .obj).
     *
     * @return If the file is successfully loaded from the provided file then
     * this function will return true; otherwise it will return false.
     */
    bool load(const std::string& filename);

    /*
     * Saves this definition of set of ObjMeshes as an Obj file.
     *
     * @param filename - The name of the Obj file to be written (include .obj).
     * @param saveTextureCoords - Flag for saving the texture coordinates of
     * the meshes included in this Obj file definition.
     * @param saveNormals - Flag for saving the normals of the meshes included
     * in this Obj file defintion.
     *
     * @return If this function is successful then it will return true,
     * otherwise it will return false.
     */
    bool save(const std::string& filename, bool saveTextureCoords = true, bool saveNormals = true);

    /* 
     * Add a newly allocated mesh to this Obj file with the provided mesh. The
     * newly allocated mesh will not contain any vertices, texture coords, or
     * normals. The index of the new mesh is returned.
     *
     * @param name - The name of the new mesh. If no name is specified, the mesh 
     * will be considered nameless.
     *
     * @return This function returns the 0-based index of the mesh that was
     * added under the provided name. The index can be used for retrieving the
     * mesh from the list of meshes included in this Obj file defintion.
     */
    std::size_t addMesh(const std::string& name = std::string());

    /* 
     * Adds the provided mesh to this Obj file. The index of the meshes position
     * within this Obj file is returned.
     * 
     * @param mesh - The new mesh to be added.
     *
     * @return Returns the 0-based index of the new mesh within the definition
     * of this Obj file.
     */
    std::size_t addMesh(const std::shared_ptr<ObjMesh>& mesh);

    /* 
     * Add the name of an external material library for this Obj file.
     * 
     * @param libraryName - The name of the library that must be accessed to
     * provide the defintions of the materials used in this Obj file.
     *
     * @return Returns the index of the material library.
     */
    std::size_t addMaterialLibrary(const std::string& libraryName);

    /* 
     * Add a group to this Obj file definition. The index of the newly added
     * group will be returned.
     *
     * @param groupName - The name of the group to be added to this Obj file.
     *
     * @return Returns the index of the newly created group.
     */
    std::size_t addGroup(const std::string& groupName);

    /* 
     * Add a material to this Obj file defintion, the index of the new material
     * is returned.
     *
     * @param name - The name of the new material.
     *
     * @return Returns the index of the new material.
     */
    std::size_t addMaterial(const std::string& name);

    /*
     * Add a group to this Obj file definition for the provided index. This
     * function is used primarily for loading Obj file defintions from a file.
     *
     * @param index - The index of the new material.
     *
     * @return If the group can be added, then this function will return true;
     * otherwise this function will return false.
     */
    bool addGroup(std::size_t index, const std::string& groupName);

    /*
     * Add a material to this Obj file defintion for the provided index. This
     * function is used primarily for loading Obj file definitions from a file.
     * 
     * @param index - The index of the new material.
     * @param name - The name of the new material.
     *
     * @return If the material can be added with the associated index, this
     * function will return true; otherwise this function will return false.
     */
    bool addMaterial(std::size_t index, const std::string& name);

    /* Returns the number of meshes in this OBJ file. */
    std::size_t size() const;

    /* Returns the number of meshes within this Obj file. */
    std::size_t getMeshCount() const;

    /* Returns the number of groups in this Obj file. */
    std::size_t getGroupCount() const;

    /* Returns the number of materials used within this Obj file. */
    std::size_t getMaterialCount() const;

    /* Returns the number of external material libraries in by this Obj file. */
    std::size_t getMaterialLibraryCount() const;

    /* Returns the name of the group associated with a faces group index.*/
    std::string getFaceGroup(std::size_t index) const;

    /* Returns the name of the material associated with a faces mat index. */
    std::string getFaceMaterial(std::size_t index) const;

    /* 
     * Returns the mesh at the provided index, if the index is out of bounds,
     * then a nullptr mesh will be returned.
     * 
     * @param index - The index of the mesh to retrieve.
     *
     * @return Returns the mesh at the provided index given that the index is
     * valid. If the index is invalid then this function will return a nullptr.
     */
    std::shared_ptr<ObjMesh> getMesh(std::size_t index) const;

    /* Returns the material library (name) at the provided index */
    std::string getMaterialLibrary(std::size_t index) const;

    /* Provides a string (human-readable) representation of this Obj file. */
    std::string toString() const;

    /* Returns the group names within this Obj file. */
    const NameMap& getGroups() const;

    /* Returns the materials used by the meshes within this Obj definition. */
    const NameMap& getMaterials() const;

    /* Returns the list of external material libraries */
    const StringArray& getMaterialLibraries() const;

protected:
    /* Stores the individual meshes within this Obj file. */
    MeshArray meshes;

    /* Stores the configuration of how meshes are grouped together. */
    NameMap groups;

    /* 
     * Stores an indexed map between faces (that contain a material index) and
     * the name of the associated material. The face material index can be used
     * as the key to determine the name of the applied material.
     */
    NameMap materials;

    /* 
     * List of the external (*.mtl) files that store the definitions of the 
     * surface materials applied to the meshes within this Obj file.
     */
    StringArray materialLibraries;
};

#endif
