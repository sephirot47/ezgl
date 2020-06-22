#include "ez/MeshDrawData.h"
#include "ez/EBO.h"
#include "ez/StreamOperators.h"
#include "ez/VAO.h"
#include "ez/VBO.h"
#include <numeric>

namespace ez
{

MeshDrawData::MeshDrawData(const Mesh& inMesh) { ComputeFromMesh(inMesh); }

void MeshDrawData::Bind() const
{
  EXPECTS(mVAO);
  mVAO->Bind();
}

[[nodiscard]] MeshDrawData::GLGuardType MeshDrawData::BindGuarded() const
{
  MeshDrawData::GLGuardType guard;
  Bind();
  return guard;
}

void MeshDrawData::ComputeFromMesh(const Mesh& inMesh)
{
  // Create vertices ids EBO
  std::shared_ptr<EBO> vertices_ids_ebo;
  {
    std::vector<Mesh::VertexId> face_vertices_ids;
    {
      face_vertices_ids.resize(inMesh.GetNumberOfCorners());
      std::iota(face_vertices_ids.begin(), face_vertices_ids.end(), 0); // 0, 1, 2, 3, 4, ...
    }
    vertices_ids_ebo = std::make_shared<EBO>(MakeSpan(face_vertices_ids));
    mVAO->SetEBO(vertices_ids_ebo);
  }

  // Create corners positions VBO
  {
    std::shared_ptr<VBO> corners_positions_pool_vbo;
    {
      std::vector<Vec3f> corners_positions_pool;
      {
        corners_positions_pool.reserve(inMesh.GetNumberOfCorners());
        for (Mesh::CornerId corner_id = 0; corner_id < inMesh.GetNumberOfCorners(); ++corner_id)
        {
          const auto face_id = (corner_id / 3);
          const auto internal_corner_id = (corner_id % 3);
          const auto vertex_id = inMesh.GetFacesData().at(face_id).mVerticesIds[internal_corner_id];
          const auto& vertex_position = inMesh.GetVerticesData().at(vertex_id).mPosition;
          corners_positions_pool.push_back(vertex_position);
        }
      }
      corners_positions_pool_vbo = std::make_shared<VBO>(MakeSpan(corners_positions_pool));
    }
    mVAO->AddVBO(corners_positions_pool_vbo, MeshDrawData::PositionAttribLocation(), VAOVertexAttribT<Vec3f>());
  }

  // Create corners normals VBO
  {
    std::shared_ptr<VBO> corners_normals_pool_vbo;
    {
      std::vector<Vec3f> corners_normals_pool;
      {
        corners_normals_pool.reserve(inMesh.GetNumberOfCorners());
        for (Mesh::CornerId corner_id = 0; corner_id < inMesh.GetNumberOfCorners(); ++corner_id)
        {
          auto normal = inMesh.GetCornerNormal(corner_id);
          if (normal == Zero<Vec3f>())
            normal = inMesh.GetFaceNormal(inMesh.GetFaceIdFromCornerId(corner_id));
          corners_normals_pool.push_back(normal);
        }
      }
      corners_normals_pool_vbo = std::make_shared<VBO>(MakeSpan(corners_normals_pool));
    }
    mVAO->AddVBO(corners_normals_pool_vbo, MeshDrawData::NormalAttribLocation(), VAOVertexAttribT<Vec3f>());
  }

  // Create corners texture coordinates VBO
  {
    std::shared_ptr<VBO> corners_texture_coordinates_pool_vbo;
    {
      std::vector<Vec2f> corners_texture_coordinates_pool;
      {
        corners_texture_coordinates_pool.reserve(inMesh.GetNumberOfCorners());
        for (Mesh::CornerId corner_id = 0; corner_id < inMesh.GetNumberOfCorners(); ++corner_id)
        {
          const auto& corner_texture_coordinates = inMesh.GetCornerTextureCoordinates(corner_id);
          corners_texture_coordinates_pool.push_back(corner_texture_coordinates);
        }
      }
      corners_texture_coordinates_pool_vbo = std::make_shared<VBO>(MakeSpan(corners_texture_coordinates_pool));
    }
    mVAO->AddVBO(corners_texture_coordinates_pool_vbo,
        MeshDrawData::TextureCoordinateAttribLocation(),
        VAOVertexAttribT<Vec2f>());
  }

  mNumberOfElements = inMesh.GetNumberOfCorners();
}
}