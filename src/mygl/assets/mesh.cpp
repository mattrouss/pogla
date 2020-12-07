#include "mesh.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <iostream>

namespace mygl
{

    std::shared_ptr<mesh> load_mesh(std::string path)
    {
        auto res = std::make_shared<mesh>();
        std::ifstream file(path);
        if (file.bad())
            return nullptr;

        std::vector<float> verts;
        std::vector<float> normals;
        std::vector<float> uv;

        std::string line;
        while (std::getline(file, line))
        {
            std::stringstream split(line);
            std::string type;
            split >> type;
            if (type == "v")
            {
                float x, y, z;
                split >> x >> y >> z;
                verts.push_back(x);
                verts.push_back(y);
                verts.push_back(z);
            }
            else if (type == "vn")
            {
                float x, y, z;
                split >> x >> y >> z;
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
            }
            else if (type == "f")
            {
                std::string a[3];
                split >> a[0] >> a[1] >> a[2];
                std::array<mygl::Vertex, 3> triangle;
                for (unsigned k = 0; k < 3; k++)
                {
                    std::string tmp;
                    std::stringstream spa(a[k]);
                    mygl::Vertex v;
                    std::getline(spa, tmp, '/');
                    {
                        unsigned i = (std::stoi(tmp) - 1) * 3;
                        v.pos = {{verts[i], verts[i + 1], verts[i + 2]}};
                    }
                    std::getline(spa, tmp, '/');
                    try
                    {
                        unsigned i = (std::stoi(tmp) - 1) * 2;
                        v.uv = {{uv[i], uv[i + 1]}};
                    }
                    catch (std::invalid_argument&)
                    {}//ignore invalid arg
                    std::getline(spa, tmp, '/');
                    {
                        unsigned i = (std::stoi(tmp) - 1) * 3;
                        v.normal = {{normals[i], normals[i + 1], normals[i + 2]}};
                    }

                    triangle[k] = v;
                }
                compute_tangent(triangle);
                for (unsigned i = 0; i < 3; i++)
                    res->verts.push_back(triangle[i]);

            }
            else if (type == "vt")
            {
                float u, v;//does not manage optional u and w
                split >> u >> v;
                uv.push_back(u);
                uv.push_back(v);
            }
            else
            {
                continue;
            }
        }

        for (unsigned i = 0; i < verts.size(); i++)
            res->verts[i].tangent.normalized();
        return res;
    }

    std::ostream& operator<<(std::ostream& out, const mesh& m)
    {
        out << "(\n";
        for (unsigned i = 0; i < m.verts.size(); i += 3)
        {
            out << "{" << m.verts[i].pos[i] << " " << m.verts[i].pos[1] << " " << m.verts[i].pos[2] << "}\n";
        }
        out << ")";

        return out;
    }

    std::shared_ptr<mesh> mesh::transform(matrix4 mat)
    {
        auto res = std::make_shared<mesh>();
        for (unsigned i = 0; i < verts.size(); i += 3)
        {
            auto p = Vec4{};
            p[0] = verts[i].pos[0];
            p[1] = verts[i].pos[1];
            p[2] = verts[i].pos[2];
            p[3] = 1;

            auto p2 = mat * p;
            res->verts.push_back(Vertex({{p2[0], p2[1], p2[3]}}));
        }

        return res;
    }
  
    void mesh::reset_transform()
    {
        transform_mat = matrix4::identity();
    }

    void mesh::translate(Vec3 v)
    {
        transform_mat.translate(v);
        //std::cout << transform_mat << "\n";
    }

    void mesh::rotate(Vec3 v)
    {
        transform_mat.rotate(v);
        //std::cout << transform_mat << "\n";
    }

    matrix4 mesh::get_transform() const
    {
        return transform_mat;
    }

    void mesh::set_pos(Vec3 p)
    {
        transform_mat.set_pos(p);
        //std::cout << transform_mat << "\n";
    }

    void mesh::set_rot(Vec3 r)
    {
        transform_mat.set_rot(r);
        //std::cout << transform_mat << "\n";
    }

    Vec3 mesh::get_pos()
    {
        return mygl::Vec3();
    }

    void compute_tangent(std::array<mygl::Vertex, 3>& triangle) {
        Vec3 tangent;
        auto edge1 = triangle[1].pos - triangle[0].pos;
        auto edge2 = triangle[2].pos - triangle[0].pos;

        auto deltaUV1 = triangle[1].uv - triangle[0].uv;
        auto deltaUV2 = triangle[2].uv - triangle[0].uv;

        auto d = 1 / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

        tangent = d * (deltaUV2[1] * edge1 - deltaUV1[1] * edge2);

        for (unsigned i = 0; i < 3; i++)
            triangle[i].tangent += tangent;

    }
}
