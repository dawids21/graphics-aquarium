#include "ObjLoader.hpp"

#include <iostream>

#include "tiny_obj_loader.h"

using namespace std;

struct Loaded {
    float x;
    float y;
    float z;
};

struct Coord {
    int x;
    int y;
    int z;
};

struct Triangle {
    struct Coord vertex;
    struct Coord normal;
};

ObjLoader::ObjLoader() {
}

ObjLoader::~ObjLoader() {
}

void ObjLoader::load(string filename) {
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./models";  // Path to material files
    reader_config.triangulate = false;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filename, reader_config)) {
        if (!reader.Error().empty()) {
            cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    std::vector<struct Loaded> fishVertex;
    std::vector<struct Loaded> fishNormal;

    for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
        fishVertex.push_back({attrib.vertices[i],
                              attrib.vertices[i + 1],
                              attrib.vertices[i + 2]});
        fishNormal.push_back({attrib.normals[i],
                              attrib.normals[i + 1],
                              attrib.normals[i + 2]});
    }

    std::vector<struct Triangle> triangles;

    for (auto shape = shapes.begin(); shape < shapes.end(); ++shape) {
        const std::vector<tinyobj::index_t>& indices = shape->mesh.indices;
        const std::vector<int>& material_ids = shape->mesh.material_ids;

        for (size_t index = 0; index < material_ids.size(); ++index) {
            // offset by 3 because values are grouped as vertex/normal/texture
            triangles.push_back({{indices[4 * index].vertex_index, indices[4 * index + 1].vertex_index, indices[4 * index + 2].vertex_index},
                                 {indices[4 * index].normal_index, indices[4 * index + 1].normal_index, indices[4 * index + 2].normal_index}});
            triangles.push_back({{indices[4 * index].vertex_index, indices[4 * index + 2].vertex_index, indices[4 * index + 3].vertex_index},
                                 {indices[4 * index].normal_index, indices[4 * index + 2].normal_index, indices[4 * index + 3].normal_index}});
        }
    }

    for (size_t i = 0; i < triangles.size(); i++) {
        this->vertices.push_back(fishVertex[triangles[i].vertex.x].x);
        this->vertices.push_back(fishVertex[triangles[i].vertex.x].y);
        this->vertices.push_back(fishVertex[triangles[i].vertex.x].z);
        this->vertices.push_back(1.0f);
        this->vertices.push_back(fishVertex[triangles[i].vertex.y].x);
        this->vertices.push_back(fishVertex[triangles[i].vertex.y].y);
        this->vertices.push_back(fishVertex[triangles[i].vertex.y].z);
        this->vertices.push_back(1.0f);
        this->vertices.push_back(fishVertex[triangles[i].vertex.z].x);
        this->vertices.push_back(fishVertex[triangles[i].vertex.z].y);
        this->vertices.push_back(fishVertex[triangles[i].vertex.z].z);
        this->vertices.push_back(1.0f);

        this->normals.push_back(fishNormal[triangles[i].normal.x].x);
        this->normals.push_back(fishNormal[triangles[i].normal.x].y);
        this->normals.push_back(fishNormal[triangles[i].normal.x].z);
        this->normals.push_back(0.0f);
        this->normals.push_back(fishNormal[triangles[i].normal.y].x);
        this->normals.push_back(fishNormal[triangles[i].normal.y].y);
        this->normals.push_back(fishNormal[triangles[i].normal.y].z);
        this->normals.push_back(0.0f);
        this->normals.push_back(fishNormal[triangles[i].normal.z].x);
        this->normals.push_back(fishNormal[triangles[i].normal.z].y);
        this->normals.push_back(fishNormal[triangles[i].normal.z].z);
        this->normals.push_back(0.0f);
    }
}

std::vector<float> ObjLoader::getVertices() {
    return this->vertices;
}
std::vector<float> ObjLoader::getNormals() {
    return this->normals;
}