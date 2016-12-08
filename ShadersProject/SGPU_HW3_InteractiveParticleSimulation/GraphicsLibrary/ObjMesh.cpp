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
#include "ObjMesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>


/* Constants specified by the Wavefront Obj file format. */
static const std::string OBJ_File_EXTENSION = "obj";
static const std::string OBJ_DELIMITER = " ";
static const std::string OBJ_OBJECT = "o";
static const std::string OBJ_GROUP = "g";
static const std::string OBJ_SMOOTHING_GROUP = "s";
static const std::string OBJ_FACE = "f";
static const std::string OBJ_VERTEX_NORMAL = "vn";
static const std::string OBJ_VERTEX_TEXTURE = "vt";
static const std::string OBJ_VERTEX = "v";
static const std::string OBJ_MATERIAL_LIBRARY = "mtllib";
static const std::string OBJ_USE_MATERIAL = "usemtl";

/* Default names for groups and materials if they are not specified. */
static const std::string OBJ_NO_MESH_NAME = "DefaultName";
static const std::string OBJ_NO_MATERIAL = "DefaultMaterial";
static const std::string OBJ_NO_GROUP = "DefaultGroup";

static const char OBJ_DELIMITER_CHAR = ' ';
static const char OBJ_NODE_DELIMITER = '/';
static const char OBJ_COMMENT = '#';

static const unsigned int OBJ_PRECISION = 6u;
static const int OBJ_INDEX_OFFSET = 1;
static const int OBJ_INVALID_FACE_INDEX = -1;

ObjFile::ObjFile() {}
ObjFile::~ObjFile() {}

std::size_t ObjFile::addMesh(const std::string& name) {
    std::shared_ptr<ObjMesh> mesh = std::make_shared<ObjMesh>(name);
    this->meshes.push_back(mesh);
    return this->meshes.size() - 1;
}

std::size_t ObjFile::addMesh(const std::shared_ptr<ObjMesh>& mesh) {
    this->meshes.push_back(mesh);
    return this->meshes.size() - 1;
}

std::size_t ObjFile::addMaterialLibrary(const std::string& libraryName) {
    if ( libraryName.length() == 0 ) {
        std::cerr << "[ObjFile:addMaterialLibrary] Error: Cannot add material library with a name of length 0." << std::endl;
        return 0u;
    }

    this->materialLibraries.push_back(libraryName);
    return this->materialLibraries.size() - 1;
}

std::size_t ObjFile::addGroup(const std::string& groupName) {
    if ( groupName.length() == 0 ) {
        std::cerr << "[ObjFile:addGroup] Error: Cannot add group with a name of length 0." << std::endl;
        return 0u;
    }

    this->groups.insert(std::make_pair(this->groups.size() - 1, groupName));
    return this->groups.size() - 1;
}

std::size_t ObjFile::addMaterial(const std::string& materialName) {
    if ( materialName.length() == 0 ) {
        std::cerr << "[ObjFile:addMaterial] Error: Cannot add group with a name of length 0." << std::endl;
        return 0u;
    }

    this->materials.insert(std::make_pair(this->materials.size() - 1, materialName));
    return this->materials.size() - 1;
}

bool ObjFile::addGroup(std::size_t index, const std::string& groupName) {
    if ( groupName.length() == 0 ) {
        std::cerr << "[ObjFile:addGroup] Error: Cannot add group with a name of length 0." << std::endl;
        return false;
    }

    if ( index != 0 ) {
        if ( this->groups.find(index) != this->groups.end() ) {
            std::cerr << "[ObjFile:addGroup] Error: Cannot add group to existing index: " << index << std::endl;
            return false;
        }
    }

    this->groups.insert(std::make_pair(index, groupName));
    return true;
}

bool ObjFile::addMaterial(std::size_t index, const std::string& name) {
    if ( name.length() == 0 ) {
        std::cerr << "[ObjFile:addMaterial] Error: Cannot add material with a name of length 0." << std::endl;
        return false;
    }

    if ( this->materials.find(index) != this->materials.end() ) {
        std::cerr << "[ObjFile:addMaterial] Error: Cannot add material with existing index: " << index << std::endl;
        return false;
    }


    this->materials.insert(std::make_pair(index, name));
    return true;
}

/* Parse a 3-component vector from the provided stream: 1.0 2.0 3.0 */
inline bool Parse_Obj_Vector(std::istringstream& argumentStream, Vector3f& vector) {
    argumentStream >> vector.x();
    argumentStream >> vector.y();
    argumentStream >> vector.z();
    return true;
}

/* Parse an individual int from the provided stream and return the value. */
int Parse_Obj_Int(std::istringstream& argumentStream, char delimiter) {
    std::string token;
    std::getline(argumentStream, token, delimiter);
    return std::atoi(token.c_str());
}

bool Parse_Obj_Vertex(ObjFile* const objFile, std::istringstream& argumentStream) {
    if ( objFile == nullptr ) return false;
    if ( objFile->getMesh(objFile->size() - 1) == nullptr ) objFile->addMesh();

    Vector3f vertex;
    Parse_Obj_Vector(argumentStream, vertex);
    objFile->getMesh(objFile->size() - 1)->vertices.push_back(vertex);
    return true;
}

bool Parse_Obj_TextureCoordinate(ObjFile* const objFile, std::istringstream& argumentStream) {
    if ( objFile == nullptr ) return false;
    if ( objFile->getMesh(objFile->size() - 1) == nullptr ) objFile->addMesh();

    Vector3f textureCoordinate;
    Parse_Obj_Vector(argumentStream, textureCoordinate);
    objFile->getMesh(objFile->size() - 1)->textureCoordinates.push_back(textureCoordinate);
    return true;
}

bool Parse_Obj_Normal(ObjFile* const objFile, std::istringstream& argumentStream) {
    if ( objFile == nullptr ) return false;
    if ( objFile->getMesh(objFile->size() - 1) == nullptr ) objFile->addMesh();

    Vector3f normal;
    Parse_Obj_Vector(argumentStream, normal);
    objFile->getMesh(objFile->size() - 1)->normals.push_back(normal);
    return true;
}

bool Parse_Obj_Object(ObjFile* const objFile, std::istringstream& argumentStream, std::size_t& curSmoothingGroupIndex) {
    if ( objFile == nullptr ) return false;

    std::string name;
    std::getline(argumentStream, name, OBJ_DELIMITER_CHAR);

    if ( name.length() != 0 ) objFile->addMesh(name);
    else objFile->addMesh();

    curSmoothingGroupIndex = 0u;
    return true;
}

bool Parse_Obj_SmoothingGroup(ObjFile* const objFile, std::istringstream& argumentStream, std::size_t& curSmoothingGroupIndex) {
    if ( objFile == nullptr ) return false;

    std::string smoothingGroup;
    std::getline(argumentStream, smoothingGroup, OBJ_DELIMITER_CHAR);

    if ( smoothingGroup.length() != 0 )
        curSmoothingGroupIndex = std::atoi(smoothingGroup.c_str());

    return true;
}

bool Parse_Obj_Group(ObjFile* const objFile, std::istringstream& argumentStream, std::size_t& curGroupIndex) {
    if ( objFile == nullptr ) return false;
    if ( objFile->getMesh(objFile->size() - 1 ) == nullptr ) objFile->addMesh();

    std::string groupName;
    std::getline(argumentStream, groupName, OBJ_DELIMITER_CHAR);
    
    if ( groupName.length() != 0 ) {
        objFile->addGroup(curGroupIndex, groupName);
        objFile->getMesh(objFile->size() - 1)->name = groupName;
        curGroupIndex++;
    }

    return true;
}

bool Parse_Obj_MaterialLibrary(ObjFile* const objFile, std::istringstream& argumentStream) {
    if ( objFile == nullptr ) return false;

    std::string libraryName;
    std::getline(argumentStream, libraryName, OBJ_DELIMITER_CHAR);

    if ( libraryName.length() != 0 ) objFile->addMaterialLibrary(libraryName);
    else return false;

    return true;
}

bool Parse_Obj_Material(ObjFile* const objFile, std::istringstream& argumentStream, std::size_t& curMaterialIndex) {
    if ( objFile == nullptr ) return false;

    std::string materialName;
    std::getline(argumentStream, materialName, OBJ_DELIMITER_CHAR);
    
    if ( materialName.length() != 0 ) {
        curMaterialIndex++;
        objFile->addGroup(curMaterialIndex, materialName);
    }
    else return false;
    
    return true;
}

bool Parse_Obj_Node(ObjFile* const objFile, std::istringstream& argumentStream, int& vertexIndex, int& textureCoordIndex, int& normalIndex) {
    std::string node = argumentStream.str();

    //--------------------------------------------------------------------------
    // Determine the number of components that are included in the current nodes
    // definition. The number of delimiting slashes in the node will declare
    // which components are defined.
    //--------------------------------------------------------------------------
    unsigned int slashCount = 0u;
    for ( unsigned int i = 0; i < node.length(); i++ )
        if ( node[i] == OBJ_NODE_DELIMITER ) slashCount++;

    //--------------------------------------------------------------------------
    // The node information of this face only contains the vertex information.
    // Ex: f 1 2 3
    //--------------------------------------------------------------------------
    if ( slashCount == 0 ) {
        vertexIndex = Parse_Obj_Int(argumentStream, OBJ_NODE_DELIMITER) - OBJ_INDEX_OFFSET;
        return true;
    }
    //--------------------------------------------------------------------------
    // The node defines the vertex and texture-coord indices.
    // Ex: f 1/1 2/2 3/3
    //--------------------------------------------------------------------------
    else if ( slashCount == 1 ) {
        vertexIndex = Parse_Obj_Int(argumentStream, OBJ_NODE_DELIMITER) - OBJ_INDEX_OFFSET;
        normalIndex = 0;
        textureCoordIndex = Parse_Obj_Int(argumentStream, OBJ_NODE_DELIMITER) - OBJ_INDEX_OFFSET;
        return true;
    }
    //--------------------------------------------------------------------------
    // The node defines the vertex, texture-coord, and normal indices
    // Ex: f 1/1/1 2/2/2 3/3/3
    //--------------------------------------------------------------------------
    else if ( slashCount == 2 ) {
        vertexIndex = Parse_Obj_Int(argumentStream, OBJ_NODE_DELIMITER) - OBJ_INDEX_OFFSET;
        textureCoordIndex = Parse_Obj_Int(argumentStream, OBJ_NODE_DELIMITER) - OBJ_INDEX_OFFSET;
        normalIndex = Parse_Obj_Int(argumentStream, OBJ_NODE_DELIMITER);

        if ( normalIndex == 0 ) {
            normalIndex = textureCoordIndex;
            textureCoordIndex = OBJ_INVALID_FACE_INDEX;
        }
        else normalIndex -= OBJ_INDEX_OFFSET;
    }
    else {
		std::cout << "[ObjFile:Parse_Obj_Node] Error: Invalid face node encountered." << std::endl;
		return false;
	}

    return true;
}

/* 
 * Ensures that a valid Obj face will be added to a mesh. If an index
 * invalidates the face, then it will be ignored and not added to the mesh.
 */
bool Valid_Obj_Face(const Obj_Face& face) {
	for ( unsigned int i = 0; i < face.vertexIndices.size(); i++ )
		if ( face.vertexIndices[i] < 0 ) return false;
	for ( unsigned int i = 0; i < face.textureIndices.size(); i++ )
		if ( face.textureIndices[i] < 0 ) return false;
	for ( unsigned int i = 0; i < face.normalIndices.size(); i++ )
		if ( face.normalIndices[i] < 0 ) return false;
	return true;
}

bool Parse_Obj_Face(ObjFile* const objFile, std::istringstream& argumentStream, std::size_t& curGroupIndex, std::size_t& curSmoothingGroupIndex, std::size_t& curMaterialIndex) {
    Obj_Face face;
    std::size_t nodeCount = 0u;
    std::string token;

    int vertexIndex = OBJ_INVALID_FACE_INDEX;
    int textureCoordIndex = OBJ_INVALID_FACE_INDEX;
    int normalIndex = OBJ_INVALID_FACE_INDEX;

    //--------------------------------------------------------------------------
    // For each node that defines a face, parse each of the vertex, texture-
    // coord, and normal indices. A node is defined as: vtx/tex/n where a face
    // contains several nodes:
    // Ex: f vtx0/tex0/n0 vtx1/tex1/n1 vtx2/tex2/n2
    //--------------------------------------------------------------------------
    while ( std::getline(argumentStream, token, OBJ_DELIMITER_CHAR) ) {
        std::istringstream nodeArguments(token);
        Parse_Obj_Node(objFile, nodeArguments, vertexIndex, textureCoordIndex, normalIndex);

        if ( vertexIndex < 0 ) {
			std::cout << "[ObjFile:Parse_Obj_Face] Error: Invalid vertex index. Aborting." << std::endl;
			return false;
		}

        if ( vertexIndex >= 0 ) face.vertexIndices.push_back(vertexIndex);
		else face.vertexIndices.push_back(0);

		if ( textureCoordIndex >= 0 ) face.textureIndices.push_back(textureCoordIndex);
		else face.textureIndices.push_back(0);

		if ( normalIndex >= 0 ) face.normalIndices.push_back(normalIndex);
		else face.normalIndices.push_back(0);
		nodeCount++;
    }

    if ( nodeCount <= 2 ) {
		std::cout << "[ObjFile:Parse_Obj_Face] Warning: Invalid OBJ face. Each OBJ face must have at least 3 sides. Ignoring face." << std::endl;
		return true;
	}

    //--------------------------------------------------------------------------
    // Determine the type of face based on its connectivity. Obj supports 3
    // types of faces: Triangle, Quad, and Polygon. If any face contains any
    // more than 5 vertices then it is automtaically considered a polygon.
    //--------------------------------------------------------------------------
    if ( nodeCount == 3 ) face.type = TRIANGLE;
	else if ( nodeCount == 4 ) face.type = QUAD;
    else face.type = POLYGON;

    face.groupIndex = curGroupIndex;
	face.smoothingGroupIndex = curSmoothingGroupIndex;
	face.materialIndex = curMaterialIndex;

    if ( objFile->getMesh(objFile->size() - 1) == nullptr ) objFile->addMesh();
	if ( Valid_Obj_Face(face) ) objFile->getMesh(objFile->size() - 1)->faces.push_back(face);
	return true;
}

/* 
 * Parses an individual line from an Obj file. The type of line is defined by
 * a unique identifier at the beginning of the line (ex. v, vt, vn). Depending
 * on the identifier of the line, the corresponding function will be called to
 * parse the required components and store them into the provided Obj file.
 */
bool Parse_ObjFileLine(ObjFile* const objFile, const std::string& line, std::size_t& curGroupIndex, std::size_t& curSmoothingGroupIndex, std::size_t& curMaterialIndex) {
    if ( line.length() == 0 ) return true;

    std::string id, arguments;

    std::istringstream argumentStream(line);
    std::getline(argumentStream, id, OBJ_DELIMITER_CHAR);

    //--------------------------------------------------------------------------
    // Parse each component of the Wavefront Obj file defintion based on the
    // keyword that identifies the predefined components of the line.
    // Ex: Identifier Component_0 Component_1 Component_2 ... Component_n
    //--------------------------------------------------------------------------
    if ( id[0] == OBJ_COMMENT ) return true;
    else if ( id.compare(OBJ_VERTEX) == 0 ) return Parse_Obj_Vertex(objFile, argumentStream);
    else if ( id.compare(OBJ_VERTEX_TEXTURE) == 0 ) return Parse_Obj_TextureCoordinate(objFile, argumentStream);
    else if ( id.compare(OBJ_VERTEX_NORMAL) == 0 ) return Parse_Obj_Normal(objFile, argumentStream);
    else if ( id.compare(OBJ_FACE) == 0 ) return Parse_Obj_Face(objFile, argumentStream, curGroupIndex, curSmoothingGroupIndex, curMaterialIndex);
    else if ( id.compare(OBJ_SMOOTHING_GROUP) == 0 ) return Parse_Obj_SmoothingGroup(objFile, argumentStream, curSmoothingGroupIndex);
    else if ( id.compare(OBJ_GROUP) == 0 ) return Parse_Obj_Group(objFile, argumentStream, curGroupIndex);
    else if ( id.compare(OBJ_OBJECT) == 0 ) return Parse_Obj_Object(objFile, argumentStream, curSmoothingGroupIndex);
    else if ( id.compare(OBJ_MATERIAL_LIBRARY) == 0 ) return Parse_Obj_MaterialLibrary(objFile, argumentStream);
    else if ( id.compare(OBJ_USE_MATERIAL) == 0 ) return Parse_Obj_Material(objFile, argumentStream, curMaterialIndex);
    else std::cerr << "[ObjFile:Parse_ObjFileLine] Warning: Encountered an unrecognized OBJ file line. Ignoring Command." << std::endl;

    return true;
}

bool ObjFile::load(const std::string& filename) {
    if ( filename.length() == 0 ) {
        std::cerr << "[ObjFile:load] Error: Invalid filename of length 0." << std::endl;
        return false;
    }

    std::ifstream file(filename.c_str());
    if ( file.is_open() == false ) {
        std::cerr << "[ObjFile:load] Error: The file: " << filename << " could not be opened." << std::endl;
        return false;
    }

    std::string line;
    std::size_t curGroupIndex = 0u;
    std::size_t curSmoothingGroupIndex = 0u;
    std::size_t curMaterialIndex = 0u;

    this->materials.insert(std::make_pair(curMaterialIndex, OBJ_NO_MATERIAL));
    this->groups.insert(std::make_pair(curGroupIndex, OBJ_NO_GROUP));

    //--------------------------------------------------------------------------
    // Parses the Obj file line-by-line.
    //--------------------------------------------------------------------------
    while ( std::getline(file, line) ) {
        if ( !Parse_ObjFileLine(this, line, curGroupIndex, curSmoothingGroupIndex, curMaterialIndex) ) {
            std::cout << "[ObjFile:load] Error: Failed to parse an OBJ file line and cannot recover." << std::endl;
			std::cout << "  Aborting OBJ file parsing process at line: " << line << std::endl;
			return false;
		}
    }

    file.close();
    return true;
}

/* 
 * Prints out an information header for an Obj file. This information is only
 * included in a comment.
 */
bool Save_Obj_Header(std::ostream& out) {
    out << "# CG Library Wavefront Obj Version 1.0 April 2015" << std::endl;
	out << std::endl;
	return true;
}

/* 
 * Prints out the names of the external libraries that are referenced by an
 * Obj file.
 */
bool Save_Obj_MaterialLibraries(std::ostream& out, const StringArray& materialLibraries) {
    if ( materialLibraries.size() == 0 ) return true;

    out << "# Obj Material Libraries" << std::endl;
    for ( unsigned int i = 0; i < materialLibraries.size(); i++ )
        out << OBJ_MATERIAL_LIBRARY << OBJ_DELIMITER << materialLibraries[i] << std::endl;
    out << std::endl;
    return true;
}

/*
 * Save all of the provided meshes texture coordinates to the provided stream.
 * Ex: for each texture coordinate
 *         vt 1.0 1.0 0.0
 *     # n texture coordinates
 */
bool Save_ObjMesh_TextureCoords(std::ostream& out, const std::shared_ptr<ObjMesh>& mesh) {
    for ( unsigned int i = 0; i < mesh->textureCoordinates.size(); i++ ) {
        out << OBJ_VERTEX_TEXTURE 
            << OBJ_DELIMITER 
            << mesh->textureCoordinates[i].x() 
            << OBJ_DELIMITER 
            << mesh->textureCoordinates[i].y() 
            << OBJ_DELIMITER 
            << mesh->textureCoordinates[i].z() 
            << std::endl;      
    }

    if ( mesh->textureCoordinates.size() > 0 ) {
        out << "# " << mesh->textureCoordinates.size() << " texture coordinates" << std::endl;
        out << std::endl;
    }

    return true;
}

/* 
 * Save all of the provided meshes normals to the provided out stream.
 * Ex: for each normal:
 *         vn 1.0 2.0 3.0
 *     # n vertex normals
 */
bool Save_ObjMesh_Normals(std::ostream& out, const std::shared_ptr<ObjMesh>& mesh) {
    for ( unsigned int i = 0; i < mesh->normals.size(); i++ ) {
        out << OBJ_VERTEX_NORMAL 
            << OBJ_DELIMITER 
            << mesh->normals[i].x() 
            << OBJ_DELIMITER 
            << mesh->normals[i].y() 
            << OBJ_DELIMITER 
            << mesh->normals[i].z() 
            << std::endl;
    }

    if ( mesh->normals.size() > 0 ) {
        out << "# " << mesh->normals.size() << " vertex normals" << std::endl;
        out << std::endl;
    }

    return true;
}

bool Save_ObjMesh_Faces(std::ostream& out, const ObjFile* const objFile, std::shared_ptr<ObjMesh>& mesh, bool saveTextureCoords, bool saveNormals) {
    std::size_t lastMaterialIndex = 0;
	std::size_t lastGroupIndex = 0;
	std::size_t lastSmoothingGroupIndex = 0;
    std::size_t numFaces = mesh->faces.size();
	Obj_Face face;

    for ( unsigned int i = 0; i < numFaces; i++ ) {
		face = mesh->faces[i];

		//----------------------------------------------------------------------
		// If the face belongs to a group, define the group. If the faces group
		// index is 0 that means it belongs to no group.
		//----------------------------------------------------------------------
		if ( face.groupIndex != 0 && face.groupIndex != lastGroupIndex ) {
            if ( objFile->getFaceGroup(face.groupIndex).compare(OBJ_NO_GROUP) == 0 )
                out << OBJ_GROUP << OBJ_DELIMITER << mesh->name << std::endl;
			else out << OBJ_GROUP << OBJ_DELIMITER << objFile->getFaceGroup(face.groupIndex) << std::endl;
			lastGroupIndex = face.groupIndex;
		}

		//----------------------------------------------------------------------
		// If the face has a material, define the material. If the faces 
		// material index is 0 that means it has no material.
		//----------------------------------------------------------------------
		if ( face.materialIndex != 0 && face.materialIndex != lastMaterialIndex ) {
			out << OBJ_USE_MATERIAL << OBJ_DELIMITER << objFile->getFaceMaterial(face.groupIndex) << std::endl;
			lastMaterialIndex = face.materialIndex;
		}

		//----------------------------------------------------------------------
		// If a face does not belong to a smoothing group then it is simply not
		// included.
		//----------------------------------------------------------------------
		if ( face.smoothingGroupIndex != 0 && face.smoothingGroupIndex != lastSmoothingGroupIndex ) {
			out << OBJ_SMOOTHING_GROUP << OBJ_DELIMITER << face.smoothingGroupIndex << std::endl;
			lastSmoothingGroupIndex = face.smoothingGroupIndex;
		}

		//----------------------------------------------------------------------
		// Write the face indices of the face. Vertices are required, therefore
		// the size of that index array is used for indexing through all of the
		// indice arrays.
		//----------------------------------------------------------------------
		out << OBJ_FACE << OBJ_DELIMITER;
		for ( unsigned int i = 0; i < face.vertexIndices.size(); i++ ) {
			//------------------------------------------------------------------
			// Depending on which vertex components are included (normal, 
			// texture coordinate), write the proper definition of each face 
			// node.
			//------------------------------------------------------------------
			out << face.vertexIndices[i] + OBJ_INDEX_OFFSET;
			if ( saveTextureCoords == true && saveNormals == true ) {
				if ( face.textureIndices.size() > 0 && face.normalIndices.size() > 0 )
					out << OBJ_NODE_DELIMITER << face.textureIndices[i] + OBJ_INDEX_OFFSET << OBJ_NODE_DELIMITER << face.normalIndices[i] + OBJ_INDEX_OFFSET;
				else if ( face.textureIndices.size() > 0 && face.normalIndices.size() == 0 )
					out << OBJ_NODE_DELIMITER << face.textureIndices[i] + OBJ_INDEX_OFFSET;
			}
			else if ( saveTextureCoords == true && saveNormals == false ) {
				if ( face.textureIndices.size() > 0 )
					out << OBJ_NODE_DELIMITER << face.textureIndices[i] + OBJ_INDEX_OFFSET;
			}
			else {
				if ( face.normalIndices.size() > 0 )
					out << OBJ_NODE_DELIMITER << OBJ_NODE_DELIMITER << face.normalIndices[i] + OBJ_INDEX_OFFSET;
			}
			
			out << OBJ_DELIMITER;
		}

        out << std::endl;
    }

    if ( mesh->faces.size() > 0 ) {
        out << "# " << mesh->faces.size() << " faces" << std::endl;
        out << std::endl;
    }

    return true;
}

/*
 * This function saves all of the meshes contained within the provided Obj file.
 * For each mesh, its vertices, texture-coords, normals, and face information
 * will be printed individually.
 *
 * @param out - The output stream (file).
 * @param objFile - The Obj file defintion containing an array of ObjMeshes
 * @param saveTextureCoords - Flag to save the texture coords of each mesh
 * @param saveNormals - Flag to save the normals of each mesh
 * 
 * @return If the meshes are saved successfully then this function will return
 * true; otherwise it will return false.
 */
bool Save_ObjMeshes(std::ostream& out, const ObjFile* const objFile, bool saveTextureCoords, bool saveNormals) {
    if ( objFile == nullptr ) {
        std::cerr << "[Save_ObjMesh:save] Error: Cannot save nullptr ObjFile." << std::endl;
        return true;
    }

    //--------------------------------------------------------------------------
    // For each mesh within the Obj file, save it to the provided out stream.
    //--------------------------------------------------------------------------
    for ( unsigned int i = 0; i < objFile->size(); i++ ) {
        std::shared_ptr<ObjMesh>& mesh = objFile->getMesh(i);
        bool saveMeshTextureCoords = saveTextureCoords;
        bool saveMeshNormals = saveNormals;

        for ( unsigned int i = 0; i < mesh->vertices.size(); i++ ) {
		    out << OBJ_VERTEX 
                << OBJ_DELIMITER 
                << mesh->vertices[i].x() 
                << OBJ_DELIMITER 
                << mesh->vertices[i].y() 
                << OBJ_DELIMITER 
                << mesh->vertices[i].z() 
                << std::endl;
        }

	    out << "# " << mesh->vertices.size() << " vertices" << std::endl;
	    out << std::endl;

        if ( mesh->textureCoordinates.size() == 0 ) saveMeshTextureCoords = false;
        if ( mesh->normals.size() == 0 ) saveMeshNormals = false;

        if ( saveMeshTextureCoords == true ) Save_ObjMesh_TextureCoords(out, mesh);
        if ( saveMeshNormals == true ) Save_ObjMesh_Normals(out, mesh);
        Save_ObjMesh_Faces(out, objFile, mesh, saveMeshTextureCoords, saveMeshNormals);
    }

    return true;
}

bool ObjFile::save(const std::string& filename, bool saveTextureCoords, bool saveNormals) {
    if ( filename.length() == 0 ) {
        std::cerr << "[ObjFile:save] Error: Cannot save to obj filename of length 0." << std::endl;
        return false;
    }

    std::ofstream out;
    out.open(filename.c_str());
    out << std::fixed << std::setprecision(OBJ_PRECISION);

    if ( out.is_open() == false ) {
        std::cerr << "[ObjFile:save] Error: Could not open file: " << filename << std::endl;
        return false;
    }

    //--------------------------------------------------------------------------
    // Write the Obj file.
    //--------------------------------------------------------------------------
    Save_Obj_Header(out);
    Save_Obj_MaterialLibraries(out, this->materialLibraries);
    Save_ObjMeshes(out, this, saveTextureCoords, saveNormals);

    out.close();
    return true;
}

std::size_t ObjFile::size() const {
    return this->meshes.size();
}

std::size_t ObjFile::getMeshCount() const {
    return this->meshes.size();
}

std::size_t ObjFile::getGroupCount() const {
    return this->groups.size();
}

std::size_t ObjFile::getMaterialCount() const {
    return this->materials.size();
}

std::size_t ObjFile::getMaterialLibraryCount() const {
    return this->materialLibraries.size();
}

std::string ObjFile::getFaceGroup(std::size_t index) const {
    if ( this->groups.find(index) == this->groups.end() ) return OBJ_NO_GROUP;
    return this->groups.find(index)->second;
}

std::string ObjFile::getFaceMaterial(std::size_t index) const {
    if ( this->materials.find(index) == this->materials.end() ) return OBJ_NO_MATERIAL;
    return this->materials.find(index)->second;
}

std::shared_ptr<ObjMesh> ObjFile::getMesh(std::size_t index) const {
    if ( index >= this->meshes.size() ) return nullptr;
    return this->meshes[index];
}

std::string ObjFile::getMaterialLibrary(std::size_t index) const {
    if ( index >= this->materialLibraries.size() ) {
        std::cerr << "[ObjFile:getMaterialLibrary] Error: Library index out of bounds: " << index << std::endl;
        return "Invalid";
    }

    return this->materialLibraries[index];
}

void Print_ObjMesh(std::stringstream& str, const std::shared_ptr<ObjMesh>& mesh) {
    Vector3f vector;
    Obj_Face face;

    str << "\t[Mesh]" << std::endl;
    str << "\t\tName: " << mesh->name << std::endl;
    str << "\t\tVertices: " << mesh->vertices.size() << std::endl;
    for ( unsigned int v = 0; v < mesh->vertices.size(); v++ ) {
        vector = mesh->vertices[v];
        str << "\t\t\tv " << vector.x() << " " << vector.y() << " " << vector.z() << std::endl;
    }
    str << "\t\tTexture Coords: " << mesh->textureCoordinates.size() << std::endl;
    for ( unsigned int t = 0; t < mesh->textureCoordinates.size(); t++ ) {
        vector = mesh->textureCoordinates[t];
        str << "\t\t\tvt " << vector.x() << " " << vector.y() << " " << vector.z() << std::endl;
    }
    str << "\t\tNormals: " << mesh->normals.size() << std::endl;
    for ( unsigned int n = 0; n < mesh->normals.size(); n++ ) {
        vector = mesh->normals[n];
        str << "\t\t\tvn " << vector.x() << " " << vector.y() << " " << vector.z() << std::endl;
    }
    str << "\t\tFaces: " << mesh->faces.size() << std::endl;
    for ( unsigned int f = 0; f < mesh->faces.size(); f++ ) {
        face = mesh->faces[f];
        str << "\t\t\tf ";
        for ( unsigned int x = 0; x < face.vertexIndices.size(); x++ ) {
            str << face.vertexIndices[x]+1 << "/" << face.textureIndices[x]+1 << "/" << face.normalIndices[x]+1 << " ";
        }
        str << std::endl;
    }
}

void Print_Obj_Groups(std::stringstream& str, const NameMap& groups) {
    str << "\t[Groups]" << std::endl;
    for ( NameMap::const_iterator i = groups.begin(); i != groups.end(); i++ )
        str << "\t\t[" << i->first << "] " << i->second << std::endl;
    str << std::endl;
}

void Print_Obj_Materials(std::stringstream& str, const NameMap& materials) {
    str << "\t[Materials]" << std::endl;
    for ( NameMap::const_iterator i = materials.begin(); i != materials.end(); i++ )
        str << "\t\t[" << i->first << "] " << i->second << std::endl;
    str << std::endl;
}

void Print_Obj_MaterialLibraries(std::stringstream& str, const StringArray& materialLibraries) {
    if ( materialLibraries.size() != 0 ) {
        str << "\t[MaterialLibraries]" << std::endl;
        for ( unsigned int i = 0; i < materialLibraries.size(); i++ )
           str << "\t\t[" << materialLibraries[i] << std::endl;
        str << std::endl;
    }
}

void Print_ObjFileInfo(std::stringstream& str, const ObjFile* const file) {
    str << "\tMesh Count: " << file->getMeshCount() << std::endl;
    str << "\tGroup Count: " << file->getGroupCount() << std::endl;
    str << "\tMaterial Count: " << file->getMaterialCount() << std::endl;
    str << "\tMaterial Library Count: " << file->getMaterialLibraryCount() << std::endl;
    str << std::endl;
}

std::string ObjFile::toString() const {
    std::stringstream str;

    str << "ObjFile {" << std::endl;
    Print_ObjFileInfo(str, this);
    Print_Obj_Groups(str, this->groups);
    Print_Obj_Materials(str, this->materials);
    Print_Obj_MaterialLibraries(str, this->materialLibraries);

    for ( unsigned int i = 0; i < this->meshes.size(); i++ )
        Print_ObjMesh(str, this->meshes[i]);

    str << "}" << std::endl;

    return str.str();
}

const NameMap& ObjFile::getGroups() const {
    return this->groups;
}

const NameMap& ObjFile::getMaterials() const {
    return this->materials;
}

const StringArray& ObjFile::getMaterialLibraries() const {
    return this->materialLibraries;
}

bool LoadObjMesh(const std::string& filename, std::shared_ptr<ObjMesh>& mesh) {
    ObjFile file;

    if ( !file.load(filename) ) {
        std::cerr << "[LoadObjMesh] Error: Could not load Obj file: " << filename << std::endl;
        return false;
    }

    if ( file.getMeshCount() == 0 ) {
        std::cerr << "[LoadObjMesh] Error: Obj file: " << filename << " contains no meshes." << std::endl;
        return false;
    }

    mesh = file.getMesh(0);
    return true;
}