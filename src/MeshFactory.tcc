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

  ConsolidateMesh(cube);
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
  auto cone = GetHemisphere(inNumVerticesX, 2);
  cone.Transform(ScaleMat4(Vec3f { 1.0f, 1.0f, 2.0f }));
  ConsolidateMesh(cone);
  return cone;
}

template <typename TMesh>
TMesh GMeshFactory<TMesh>::GetSphere(const std::size_t inNumVerticesY,
    const std::size_t inNumVerticesZ,
    const bool inIsHemisphere)
{
  EXPECTS(inNumVerticesY >= 3);
  EXPECTS(inNumVerticesZ >= 3);

  TMesh sphere;

  // Add vertices
  const auto angle_z_increment
      = -(HalfCircleRads<float>() / (inNumVerticesZ - (inIsHemisphere ? 2 : 1))) / (inIsHemisphere ? 2 : 1);
  const auto angle_y_increment = -FullCircleRads<float>() / inNumVerticesY;
  auto angle_z = (inIsHemisphere ? 0.0f : QuarterCircleRads<float>());
  for (Mesh::VertexId z = 0; z < inNumVerticesZ; ++z)
  {
    if (z == 0 || z == (inNumVerticesZ - 1)) // South or north pole, single vertex
    {
      sphere.AddVertex(Vec3f { 0.0f, 0.0f, std::sin(angle_z) });
      if (inIsHemisphere)
        continue;
    }
    else // Middle vertices
    {
      const auto position_z = std::sin(angle_z);
      const auto cos_z = std::cos(angle_z);

      auto angle_y = 0.0f;
      for (Mesh::VertexId y = 0; y < inNumVerticesY; ++y)
      {
        const auto position_x = cos_z * std::cos(angle_y);
        const auto position_y = cos_z * std::sin(angle_y);
        const auto point = Vec3f { position_x, position_y, position_z };
        sphere.AddVertex(point);

        angle_y += angle_y_increment;
      }
    }

    angle_z += angle_z_increment;
  }

  // Add faces
  for (Mesh::VertexId z = 0; z < inNumVerticesZ - 1; ++z)
  {
    const bool is_current_south_pole = (z == 0);
    const bool is_up_north_pole = ((z + 1) == inNumVerticesZ - 1);
    for (Mesh::VertexId y = 0; y < inNumVerticesY; ++y)
    {
      if (is_current_south_pole) // Current row is the south pole point
      {
        const auto south_pole_vertex_index = 0;
        const auto up_vertex_index = 1 + y;
        const auto up_right_vertex_index = 1 + ((y + 1) % inNumVerticesY);
        sphere.AddFace(south_pole_vertex_index, up_right_vertex_index, up_vertex_index);
      }
      else if (!is_up_north_pole) // Middle vertices
      {
        const auto current_vertex_index = (1 + (z - 1) * inNumVerticesY) + y;
        const auto right_vertex_index = (1 + (z - 1) * inNumVerticesY) + ((y + 1) % inNumVerticesY);
        const auto up_vertex_index = (1 + z * inNumVerticesY) + y;
        const auto up_right_vertex_index = (1 + z * inNumVerticesY) + ((y + 1) % inNumVerticesY);
        sphere.AddFace(current_vertex_index, right_vertex_index, up_right_vertex_index);
        sphere.AddFace(current_vertex_index, up_right_vertex_index, up_vertex_index);
      }
      else // Next row above is the north pole point
      {
        const auto current_vertex_index = (1 + (z - 1) * inNumVerticesY) + y;
        const auto right_vertex_index = (1 + (z - 1) * inNumVerticesY) + ((y + 1) % inNumVerticesY);
        const auto north_pole_vertex_index = (1 + ((inNumVerticesZ - 2) * inNumVerticesY));
        sphere.AddFace(current_vertex_index, right_vertex_index, north_pole_vertex_index);
      }
    }
  }

  ConsolidateMesh(sphere);
  return sphere;
}

template <typename TMesh>
TMesh GMeshFactory<TMesh>::GetCylinder(const std::size_t inNumVerticesX)
{
  EXPECTS(inNumVerticesX >= 3);

  TMesh cylinder;

  // Forward and back circle vertices
  for (bool forward : { true, false })
  {
    for (Mesh::VertexId i = 0; i < inNumVerticesX; ++i)
    {
      const auto progression = (static_cast<float>(i) / inNumVerticesX);
      const auto angle = FullCircleRads<float>() * progression;
      const auto x = std::cos(angle) * 0.5f;
      const auto y = std::sin(angle) * 0.5f;
      const auto z = (forward ? -0.5f : 0.5f);
      const auto vertex_position = Vec3f(x, y, z);
      cylinder.AddVertex(vertex_position);
    }
  }

  // Forward and back central vertices
  cylinder.AddVertex(Vec3f { 0.0f, 0.0f, -0.5f });
  cylinder.AddVertex(Vec3f { 0.0f, 0.0f, 0.5f });

  // Pipe faces
  for (Mesh::VertexId forward_vertex_id = 0; forward_vertex_id < inNumVerticesX; ++forward_vertex_id)
  {
    const auto next_forward_vertex_id = (forward_vertex_id + 1) % inNumVerticesX;
    const auto back_vertex_id = (forward_vertex_id + inNumVerticesX);
    const auto next_back_vertex_id = (next_forward_vertex_id + inNumVerticesX);
    cylinder.AddFace(forward_vertex_id, next_forward_vertex_id, back_vertex_id);
    cylinder.AddFace(back_vertex_id, next_forward_vertex_id, next_back_vertex_id);
  }

  // Cap vertices
  for (bool forward : { true, false })
  {
    for (Mesh::VertexId i = 0; i < inNumVerticesX; ++i)
    {
      const auto cap_vertex_id = (i + (forward ? 0 : inNumVerticesX));
      const auto next_cap_vertex_id = (cap_vertex_id + 1) % inNumVerticesX + (forward ? 0 : inNumVerticesX);
      const auto cap_central_vertex_id = (forward ? (inNumVerticesX * 2) : (inNumVerticesX * 2 + 1));
      if (forward)
        cylinder.AddFace(next_cap_vertex_id, cap_vertex_id, cap_central_vertex_id);
      else
        cylinder.AddFace(cap_vertex_id, next_cap_vertex_id, cap_central_vertex_id);
    }
  }

  ConsolidateMesh(cylinder);
  return cylinder;
}

template <typename TMesh>
void GMeshFactory<TMesh>::ConsolidateMesh(TMesh& ioMesh)
{
  constexpr auto max_smooth_angle = DegreeToRad(45.0f);
  ioMesh.ComputeNormals(max_smooth_angle);

  if constexpr (std::is_same_v<TMesh, DrawableMesh>)
  {
    ioMesh.UpdateVAOs();
  }
}
}