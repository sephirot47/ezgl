#include "DrawableMesh.h"
#include "EBO.h"
#include "StreamOperators.h"
#include "VAO.h"
#include "VBO.h"
#include <numeric>

namespace egl
{
void DrawableMesh::Bind() const { mVAO.Bind(); }

void DrawableMesh::UpdateVAOs(const DrawableMesh::ENormalsType& inNormalsType)
{
  mVAO.Bind();

  const auto use_smooth_normals = (inNormalsType == DrawableMesh::ENormalsType::SMOOTH);

  // Create vertices ids EBO
  std::shared_ptr<EBO> vertices_ids_ebo;
  {
    std::vector<Mesh::VertexId> face_vertices_ids;

    if (use_smooth_normals)
    {
      face_vertices_ids.reserve(GetNumberOfFaces() * 3);
      for (std::size_t face_id = 0; face_id < GetNumberOfFaces(); ++face_id)
      {
        face_vertices_ids.push_back(GetFacesData().at(face_id).mVerticesIds[0]);
        face_vertices_ids.push_back(GetFacesData().at(face_id).mVerticesIds[1]);
        face_vertices_ids.push_back(GetFacesData().at(face_id).mVerticesIds[2]);
      }
    }
    else
    {
      face_vertices_ids.resize(GetNumberOfFaces() * 3);
      std::iota(face_vertices_ids.begin(), face_vertices_ids.end(), 0); // 0, 1, 2, 3, 4, ...
    }
    vertices_ids_ebo = std::make_shared<EBO>(MakeSpan(face_vertices_ids));
    mVAO.SetEBO(vertices_ids_ebo);
  }

  // Create positions VBO
  {
    std::shared_ptr<VBO> vertices_positions_pool_vbo;
    {
      std::vector<Vec3f> vertices_positions_pool;
      if (use_smooth_normals)
      {
        vertices_positions_pool.reserve(GetNumberOfVertices());
        for (std::size_t vertex_id = 0; vertex_id < GetNumberOfVertices(); ++vertex_id)
        {
          const auto& vertex_position = GetVerticesData().at(vertex_id).mPosition;
          vertices_positions_pool.push_back(vertex_position);
        }
      }
      else
      {
        vertices_positions_pool.reserve(GetNumberOfFaces() * 3);
        for (std::size_t face_id = 0; face_id < GetNumberOfFaces(); ++face_id)
        {
          for (Mesh::InternalCornerId internal_corner_id = 0; internal_corner_id < 3; ++internal_corner_id)
          {
            const auto& vertex_id = GetFacesData().at(face_id).mVerticesIds.at(internal_corner_id);
            const auto& vertex_position = GetVerticesData().at(vertex_id).mPosition;
            vertices_positions_pool.push_back(vertex_position);
          }
        }
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
      if (use_smooth_normals)
      {
        vertices_normals_pool.reserve(GetNumberOfVertices());
        for (std::size_t vertex_id = 0; vertex_id < GetNumberOfVertices(); ++vertex_id)
        {
          const auto& vertex_smooth_normal = ComputeVertexSmoothNormal(vertex_id);
          vertices_normals_pool.push_back(vertex_smooth_normal);
        }
      }
      else
      {
        vertices_normals_pool.reserve(GetNumberOfFaces() * 3);
        for (std::size_t face_id = 0; face_id < GetNumberOfFaces(); ++face_id)
        {
          const auto face_normal = ComputeFaceNormal(face_id);
          vertices_normals_pool.push_back(face_normal);
          vertices_normals_pool.push_back(face_normal);
          vertices_normals_pool.push_back(face_normal);
        }
      }
      vertices_normals_pool_vbo = std::make_shared<VBO>(MakeSpan(vertices_normals_pool));
    }
    mVAO.AddVBO(vertices_normals_pool_vbo, DrawableMesh::NormalAttribLocation(), VAOVertexAttribT<Vec3f>());
  }
}

void DrawableMesh::Read(const std::filesystem::path& inMeshPath)
{
  Mesh::Read(inMeshPath);
  UpdateVAOs();
}
}