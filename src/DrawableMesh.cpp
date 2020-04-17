#include "DrawableMesh.h"
#include "EBO.h"
#include "StreamOperators.h"
#include "VAO.h"
#include "VBO.h"
#include <numeric>

namespace egl
{
void DrawableMesh::Bind() const { mVAO.Bind(); }

void DrawableMesh::UpdateVAOs()
{
  mVAO.Bind();

  // Create vertices ids EBO
  std::shared_ptr<EBO> vertices_ids_ebo;
  {
    std::vector<Mesh::VertexId> face_vertices_ids;
    {
      face_vertices_ids.resize(GetNumberOfCorners());
      std::iota(face_vertices_ids.begin(), face_vertices_ids.end(), 0); // 0, 1, 2, 3, 4, ...
    }
    vertices_ids_ebo = std::make_shared<EBO>(MakeSpan(face_vertices_ids));
    mVAO.SetEBO(vertices_ids_ebo);
  }

  // Create corners positions VBO
  {
    std::shared_ptr<VBO> corners_positions_pool_vbo;
    {
      std::vector<Vec3f> corners_positions_pool;
      {
        corners_positions_pool.reserve(GetNumberOfCorners());
        for (Mesh::CornerId corner_id = 0; corner_id < GetNumberOfCorners(); ++corner_id)
        {
          const auto face_id = (corner_id / 3);
          const auto internal_corner_id = (corner_id % 3);
          const auto vertex_id = GetFacesData().at(face_id).mVerticesIds[internal_corner_id];
          const auto& vertex_position = GetVerticesData().at(vertex_id).mPosition;
          corners_positions_pool.push_back(vertex_position);
        }
      }
      corners_positions_pool_vbo = std::make_shared<VBO>(MakeSpan(corners_positions_pool));
    }
    mVAO.AddVBO(corners_positions_pool_vbo, DrawableMesh::PositionAttribLocation(), VAOVertexAttribT<Vec3f>());
  }

  // Create corners normals VBO
  {
    std::shared_ptr<VBO> corners_normals_pool_vbo;
    {
      std::vector<Vec3f> corners_normals_pool;
      {
        corners_normals_pool.reserve(GetNumberOfCorners());
        for (Mesh::CornerId corner_id = 0; corner_id < GetNumberOfCorners(); ++corner_id)
        {
          const auto& corner_normal = GetCornerNormal(corner_id);
          corners_normals_pool.push_back(corner_normal);
        }
      }
      corners_normals_pool_vbo = std::make_shared<VBO>(MakeSpan(corners_normals_pool));
    }
    mVAO.AddVBO(corners_normals_pool_vbo, DrawableMesh::NormalAttribLocation(), VAOVertexAttribT<Vec3f>());
  }

  // Create corners texture coordinates VBO
  {
    std::shared_ptr<VBO> corners_texture_coordinates_pool_vbo;
    {
      std::vector<Vec2f> corners_texture_coordinates_pool;
      {
        corners_texture_coordinates_pool.reserve(GetNumberOfCorners());
        for (Mesh::CornerId corner_id = 0; corner_id < GetNumberOfCorners(); ++corner_id)
        {
          const auto& corner_texture_coordinates = GetCornerTextureCoordinates(corner_id);
          corners_texture_coordinates_pool.push_back(corner_texture_coordinates);
        }
      }
      corners_texture_coordinates_pool_vbo = std::make_shared<VBO>(MakeSpan(corners_texture_coordinates_pool));
    }
    mVAO.AddVBO(corners_texture_coordinates_pool_vbo, DrawableMesh::TextureCoordinateAttribLocation(), VAOVertexAttribT<Vec2f>());
  }
}

void DrawableMesh::Read(const std::filesystem::path& inMeshPath)
{
  Mesh::Read(inMeshPath);
  UpdateVAOs();
}
}