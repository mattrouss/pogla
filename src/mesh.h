#ifndef POGL1_MESH_H
#define POGL1_MESH_H

#include <vector>
#include <string>
#include <memory>

#include "matrix4.h"

namespace mygl
{
    struct Vertex {
        Vertex() = default;
        Vertex(const mygl::Vec3& pos_): pos(pos_) {}
        Vertex(const mygl::Vec3& pos_, const mygl::Vec3& normal_, const mygl::Vec2& uv_, const mygl::Vec3& tangent_):
            pos(pos_), normal(normal_), uv(uv_), tangent(tangent_) {}

        mygl::Vec3 pos;
        mygl::Vec3 normal;
        mygl::Vec2 uv;
        mygl::Vec3 tangent;
    };
    struct mesh
    {
        std::vector<Vertex> verts;
        std::shared_ptr<mesh> tranform(matrix4 mat);
    };

    std::shared_ptr<mesh> load_mesh(std::string path);
    std::ostream& operator<<(std::ostream& out, const mesh& m);
}

#endif //POGL1_MESH_H
