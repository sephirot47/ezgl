#include "DrawableMesh.h"

#include "EBO.h"
#include "StreamOperators.h"
#include "VAO.h"
#include "VBO.h"

namespace egl
{
void DrawableMesh::UpdateVAOs()
{
    mVAO.Bind();

    // Create EBO
    std::shared_ptr<EBO> vertices_ids_ebo;
    {
        std::vector<Mesh::VertexId> face_vertices_ids;
        face_vertices_ids.reserve(GetNumberOfFaces() * 3);
        for (std::size_t face_id = 0; face_id < GetNumberOfFaces(); ++face_id)
        {
            face_vertices_ids.push_back(GetFacesData().at(face_id).mVerticesIds[0]);
            face_vertices_ids.push_back(GetFacesData().at(face_id).mVerticesIds[1]);
            face_vertices_ids.push_back(GetFacesData().at(face_id).mVerticesIds[2]);
        }
        vertices_ids_ebo = std::make_shared<EBO>(MakeSpan(face_vertices_ids));
        mVAO.SetEBO(vertices_ids_ebo);
    }

    // Create positions VBO
    {
        std::shared_ptr<VBO> vertices_positions_pool_vbo;
        {
            std::vector<Vec3f> vertices_positions_pool;
            vertices_positions_pool.reserve(GetNumberOfVertices());
            for (std::size_t vertex_id = 0; vertex_id < GetNumberOfVertices(); ++vertex_id)
            {
                vertices_positions_pool.push_back(GetVerticesData().at(vertex_id).mPosition);
            }
            vertices_positions_pool_vbo = std::make_shared<VBO>(MakeSpan(vertices_positions_pool));
        }
        mVAO.AddVBO(vertices_positions_pool_vbo, DrawableMesh::PositionAttribLocation(), VAOVertexAttribT<Vec3f>());
    }

    // Create normals VBO
    {
        std::shared_ptr<VBO> vertices_normals_pool_vbo;
        {
            std::vector<Vec3f> vertices_normals_pool;
            vertices_normals_pool.reserve(GetNumberOfVertices());
            for (std::size_t vertex_id = 0; vertex_id < GetNumberOfVertices(); ++vertex_id)
            {
                vertices_normals_pool.push_back(GetVerticesData().at(vertex_id).mNormal);
            }
            vertices_normals_pool_vbo = std::make_shared<VBO>(MakeSpan(vertices_normals_pool));
        }
        mVAO.AddVBO(vertices_normals_pool_vbo, DrawableMesh::NormalAttribLocation(), VAOVertexAttribT<Vec3f>());
    }
}

void DrawableMesh::Draw()
{
    const auto number_of_elements = GetNumberOfFaces() * 3;
    static constexpr auto ElementIdType = GLTypeTraits<Mesh::VertexId>::GLType;
    GL::DrawElements(GL::EPrimitivesMode::TRIANGLES, number_of_elements, ElementIdType);
}

void DrawableMesh::Read(const std::filesystem::path& inMeshPath)
{
    Mesh::Read(inMeshPath);
}
}