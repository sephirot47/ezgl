#include "MeshFactory.h"

namespace egl
{

template <typename TMesh>
TMesh GMeshFactory<TMesh>::GetCube()
{
  TMesh cube;

  cube.AddVertex(Vec3f { -1.0f, -1.0f, -1.0f }); // 0
  cube.AddVertex(Vec3f { -1.0f, -1.0f, 1.0f }); // 1
  cube.AddVertex(Vec3f { -1.0f, 1.0f, -1.0f }); // 2
  cube.AddVertex(Vec3f { -1.0f, 1.0f, 1.0f }); // 3
  cube.AddVertex(Vec3f { 1.0f, -1.0f, -1.0f }); // 4
  cube.AddVertex(Vec3f { 1.0f, -1.0f, 1.0f }); // 5
  cube.AddVertex(Vec3f { 1.0f, 1.0f, -1.0f }); // 6
  cube.AddVertex(Vec3f { 1.0f, 1.0f, 1.0f }); // 7

  cube.AddFace(0, 4, 1);
  cube.AddFace(1, 4, 5);
  cube.AddFace(1, 5, 3);
  cube.AddFace(3, 5, 7);
  cube.AddFace(1, 3, 2);
  cube.AddFace(0, 1, 2);
  cube.AddFace(3, 7, 2);
  cube.AddFace(7, 6, 2);
  cube.AddFace(4, 6, 5);
  cube.AddFace(5, 6, 7);
  cube.AddFace(0, 6, 4);
  cube.AddFace(0, 2, 6);

  ConsolidateMesh(cube, DrawableMesh::ENormalsType::FLAT);
  return cube;
}

template <typename TMesh>
TMesh GMeshFactory<TMesh>::GetSphere(const std::size_t inNumVerticesX, const std::size_t inNumVerticesY)
{
  return GetSphere(inNumVerticesX, inNumVerticesY, false);
}

template <typename TMesh>
TMesh GMeshFactory<TMesh>::GetHemisphere(const std::size_t inNumVerticesX, const std::size_t inNumVerticesY)
{
  return GetSphere(inNumVerticesX, inNumVerticesY + 1, true);
}

template <typename TMesh>
TMesh GMeshFactory<TMesh>::GetCone(const std::size_t inNumVerticesX)
{
  return GetHemisphere(inNumVerticesX, 2);
}

template <typename TMesh>
TMesh GMeshFactory<TMesh>::GetSphere(const std::size_t inNumVerticesX,
    const std::size_t inNumVerticesY,
    const bool inIsHemisphere)
{
  EXPECTS(inNumVerticesX >= 3);
  EXPECTS(inNumVerticesY >= 3);

  TMesh sphere;

  // Add vertices
  const auto angle_y_increment = (HalfCircleRads<float>() / (inNumVerticesY - (inIsHemisphere ? 2 : 1) )) / (inIsHemisphere ? 2 : 1);
  const auto angle_x_increment = -FullCircleRads<float>() / inNumVerticesX;
  auto angle_y = (inIsHemisphere ? 0.0f : -QuarterCircleRads<float>());
  for (Mesh::VertexId y = 0; y < inNumVerticesY; ++y)
  {
    if (y == 0 || y == (inNumVerticesY - 1)) // South or north pole, single vertex
    {
      sphere.AddVertex(Vec3f { 0.0f, std::sin(angle_y), 0.0f });
      if (inIsHemisphere)
        continue;
    }
    else // Middle vertices
    {
      const auto position_y = std::sin(angle_y);
      const auto cos_y = std::cos(angle_y);

      auto angle_x = 0.0f;
      for (Mesh::VertexId x = 0; x < inNumVerticesX; ++x)
      {
        const auto position_x = cos_y * std::cos(angle_x);
        const auto position_z = cos_y * std::sin(angle_x);
        const auto point = Vec3f { position_x, position_y, position_z };
        sphere.AddVertex(point);

        angle_x += angle_x_increment;
      }
    }

    angle_y += angle_y_increment;
  }

  // Add faces
  for (Mesh::VertexId y = 0; y < inNumVerticesY - 1; ++y)
  {
    const bool is_current_south_pole = (y == 0);
    const bool is_up_north_pole = ((y + 1) == inNumVerticesY - 1);
    for (Mesh::VertexId x = 0; x < inNumVerticesX; ++x)
    {
      // PRINT(y << ", " << x);
      if (is_current_south_pole) // Current row is the south pole point
      {
        const auto south_pole_vertex_index = 0;
        const auto up_vertex_index = 1 + x;
        const auto up_right_vertex_index = 1 + ((x + 1) % inNumVerticesX);
        sphere.AddFace(south_pole_vertex_index, up_right_vertex_index, up_vertex_index);
      }
      else if (!is_up_north_pole) // Middle vertices
      {
        const auto current_vertex_index = (1 + (y - 1) * inNumVerticesX) + x;
        const auto right_vertex_index = (1 + (y - 1) * inNumVerticesX) + ((x + 1) % inNumVerticesX);
        const auto up_vertex_index = (1 + y * inNumVerticesX) + x;
        const auto up_right_vertex_index = (1 + y * inNumVerticesX) + ((x + 1) % inNumVerticesX);
        sphere.AddFace(current_vertex_index, right_vertex_index, up_right_vertex_index);
        sphere.AddFace(current_vertex_index, up_right_vertex_index, up_vertex_index);
      }
      else // Next row above is the north pole point
      {
        const auto current_vertex_index = (1 + (y - 1) * inNumVerticesX) + x;
        const auto right_vertex_index = (1 + (y - 1) * inNumVerticesX) + ((x + 1) % inNumVerticesX);
        const auto north_pole_vertex_index = (1 + ((inNumVerticesY - 2) * inNumVerticesX));
        sphere.AddFace(current_vertex_index, right_vertex_index, north_pole_vertex_index);
      }
    }
  }

  ConsolidateMesh(sphere, DrawableMesh::ENormalsType::SMOOTH);
  return sphere;
}

template <typename TMesh>
void GMeshFactory<TMesh>::ConsolidateMesh(TMesh& ioMesh, const DrawableMesh::ENormalsType& inNormalsType)
{
  if constexpr (std::is_same_v<TMesh, DrawableMesh>)
  {
    ioMesh.UpdateVAOs(inNormalsType);
  }
}
}