#ifndef POGL1_MESH_H
#define POGL1_MESH_H

#include <vector>
#include <string>
#include <memory>

namespace mygl
{
    struct mesh
    {
        std::vector<float> verts;
        std::vector<float> normals;
        std::vector<float> uv;
    };

    std::shared_ptr<mesh> load_mesh(std::string path);
}

#endif //POGL1_MESH_H
