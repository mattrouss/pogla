#include "mesh.h"
#include "matrix4.h"
#include "vector.h"
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
                for (unsigned k = 0; k < 3; k++)
                {
                    std::string tmp;
                    std::stringstream spa(a[k]);
                    std::getline(spa, tmp, '/');
                    {
                        unsigned i = (std::stoi(tmp) - 1) * 3;
                        for (unsigned j = 0; j < 3; j++)
                        {
                            res->verts.push_back(verts[i + j]);
                        }
                    }
                    std::getline(spa, tmp, '/');
                    try
                    {
                        unsigned i = (std::stoi(tmp) - 1) * 2;
                        for (unsigned j = 0; j < 2; j++)
                        {
                            res->uv.push_back(uv[i + j]);
                        }
                    }
                    catch (std::invalid_argument&)
                    {}//ignore invalid arg
                    std::getline(spa, tmp, '/');
                    {
                        unsigned i = (std::stoi(tmp) - 1) * 3;
                        for (unsigned j = 0; j < 3; j++)
                        {
                            res->normals.push_back(normals[i + j]);
                            //std::cout << "normal: " << normals[i + j] << "\n";
                        }
                    }
                }

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

        return res;
    }

    std::ostream& operator<<(std::ostream& out, const mesh& m)
    {
        out << "(\n";
        for (unsigned i = 0; i < m.verts.size(); i += 3)
        {
            out << "{" << m.verts[i] << " " << m.verts[i + 1] << " " << m.verts[i + 2] << "}\n";
        }
        out << ")";

        return out;
    }

    std::shared_ptr<mesh> mesh::tranform(matrix4 mat)
    {
        auto res = std::make_shared<mesh>();
        for (unsigned i = 0; i < verts.size(); i += 3)
        {
            auto p = Vec4{};
            p[0] = verts[i];
            p[1] = verts[i + 1];
            p[2] = verts[i + 2];
            p[3] = 1;

            auto p2 = mat * p;
            res->verts.push_back(p2[0]);
            res->verts.push_back(p2[1]);
            res->verts.push_back(p2[2]);
        }

        return res;
    }
}