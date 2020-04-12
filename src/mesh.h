#ifndef POGL1_MESH_H
#define POGL1_MESH_H

#include <vector>
#include <string>
#include <memory>

#include "matrix4.h"

namespace mygl
{
    struct mesh
    {
        std::vector<float> verts;
        std::vector<float> normals;
        std::vector<float> uv;

        std::shared_ptr<mesh> tranform(matrix4 mat);
    };

    std::shared_ptr<mesh> load_mesh(std::string path);
    std::ostream& operator<<(std::ostream& out, const mesh& m);
}

#endif //POGL1_MESH_H
