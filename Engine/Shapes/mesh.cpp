#include "mesh.h"

// all shapes have mass, a center of mass (the position!), orientation (the orientation!), a moment of inertia, and an elasticity value
// graphical properties include color, material, and refraction index
Mesh::Mesh(int meshSize, float longD, int meshIndx, string fName, float mass, vec3 com, vec4 orientation, float elasticity, bool anchor, vec3 color, int m, float refidx) : 
Shape(mass, com, orientation, elasticity, anchor, color, m, refidx), meshSize(meshSize), longD(longD), meshIndx(meshIndx), fName(fName) {
    parseFile();
}

// returns an array of width WIDTH
vector<float> Mesh::parseData() const {
    vector<float> returned{
        3,
        com.X(),
        com.Y(),
        com.Z(),
        rot.X(),
        rot.Y(),
        rot.Z(),
        rot.W(),
        (float)meshSize,
        longD,
        0,
        (float)m,
        refidx,
        color.X(),
        color.Y(),
        color.Z()
    };

    return returned;
}

// Collision functions
Collision Mesh::collideWith_Sphere(const Sphere &sphere) {

}
Collision Mesh::collideWith_Box(const BBox &box) {

}
Collision Mesh::collideWith_Capsule(const Capsule &capsule) {

}
Collision Mesh::collideWith_Mesh(const Mesh &mesh) {

}

// Parse saved file (name provided by constructor) to read and define mesh in memory
void Mesh::parseFile()
{
    // taken directly from https://github.com/tinyobjloader/tinyobjloader
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(fName, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                vertices.insert(vertices.end(), {(float)vx, (float)vy, (float)vz});
                
                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                }

                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
            }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }

    meshSize = vertices.size();
}

vector<float> Mesh::getVertices() const {
    return vertices;
}

// Update shader with relevant data
void Mesh::setupMesh() {
}