#ifndef _MESH_H
#define _MESH_H

#include "../../common.h"

// utility structs for Mesh
struct Vertex {
    float Position[3];
    float Normal[3];
    float TexCoords[2];
};
struct Texture {
    unsigned int id;
    string type;
};  

class Mesh : public Shape {
    public:
        // all shapes have mass, a center of mass (the position!), orientation (the orientation!), a moment of inertia, and an elasticity value
        // graphical properties include color, material, and refraction index
        Mesh(int meshSize, float longD, int meshIndx, string fName, float mass, vec3 com, vec4 orientation, float elasticity, bool anchor, 
              vec3 color, int m, float refidx);
        
        // returns an array of width WIDTH
        vector<float> parseData() const override;

        // Collision functions
        Collision collideWith_Sphere(const Sphere& sphere);
        Collision collideWith_Box(const BBox& box);
        Collision collideWith_Capsule(const Capsule& capsule);
        Collision collideWith_Mesh(const Mesh& mesh);
        
        // Parse saved file (name provided by constructor) to read and define mesh in memory
        void parseFile();

        // GPU friendly vertices
        vector<float> getVertices() const override;

        //vector<Vertex> vertices;
        //vector<unsigned int> indices;
        //vector<Texture> textures;

        // Update shader with relevant data
        void setupMesh();

    private:
        int meshSize;
        int meshIndx;
        float longD;
        string fName;

        //  render data
        unsigned int VAO, VBO, EBO;

        vector<float> vertices;

};

#include "mesh.cpp"

#endif