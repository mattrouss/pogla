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

        std::shared_ptr<mesh> transform(matrix4 mat);
        void reset_transform();
        void translate(Vec3 v);
        void rotate(Vec3 v);

        matrix4 get_transform() const;

    private:
        matrix4 transform_mat;
    };

    std::shared_ptr<mesh> load_mesh(std::string path);
    std::ostream& operator<<(std::ostream& out, const mesh& m);
}

#endif //POGL1_MESH_H
