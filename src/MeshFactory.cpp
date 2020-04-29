#include "MeshFactory.h"
#include "Math.h"
#include "Mesh.h"

namespace egl
{

Mesh MeshFactory::GetCube()
{
  Mesh cube;

  cube.AddVertex(Vec3f { -1.0f, -1.0f, -1.0f }); // 0
  cube.AddVertex(Vec3f { -1.0f, -1.0f, 1.0f });  // 1
  cube.AddVertex(Vec3f { -1.0f, 1.0f, -1.0f });  // 2
  cube.AddVertex(Vec3f { -1.0f, 1.0f, 1.0f });   // 3
  cube.AddVertex(Vec3f { 1.0f, -1.0f, -1.0f });  // 4
  cube.AddVertex(Vec3f { 1.0f, -1.0f, 1.0f });   // 5
  cube.AddVertex(Vec3f { 1.0f, 1.0f, -1.0f });   // 6
  cube.AddVertex(Vec3f { 1.0f, 1.0f, 1.0f });    // 7

  cube.AddFace(0, 4, 1); // Y-
  cube.AddFace(1, 4, 5); // Y-
  cube.AddFace(1, 5, 3); // Z+
  cube.AddFace(3, 5, 7); // Z+
  cube.AddFace(1, 3, 2); // X+
  cube.AddFace(0, 1, 2); // X+
  cube.AddFace(3, 7, 2); // Y-
  cube.AddFace(7, 6, 2); // Y-
  cube.AddFace(4, 6, 5); // X-
  cube.AddFace(5, 6, 7); // X-
  cube.AddFace(0, 6, 4); // Z-
  cube.AddFace(0, 2, 6); // Z-

  ConsolidateMesh(cube);
  return cube;
}

Mesh MeshFactory::GetSphere(const std::size_t inNumLatitudes, const std::size_t inNumLongitudes)
{
  return GetSphere(inNumLatitudes, inNumLongitudes, false);
}

Mesh MeshFactory::GetHemisphere(const std::size_t inNumLatitudes, const std::size_t inNumLongitudes)
{
  return GetSphere(inNumLatitudes + 1, inNumLongitudes, true);
}

Mesh MeshFactory::GetCone(const std::size_t inNumLongitudes)
{
  auto cone = GetHemisphere(2, inNumLongitudes);
  cone.Transform(ScaleMat(Vec3f { 1.0f, 1.0f, 2.0f }));
  ConsolidateMesh(cone);
  return cone;
}

Mesh MeshFactory::GetSphere(const std::size_t inNumLatitudes,
    const std::size_t inNumLongitudes,
    const bool inIsHemisphere)
{
  EXPECTS(inNumLatitudes >= 3);
  EXPECTS(inNumLongitudes >= 3);

  Mesh sphere;

  // Add vertices
  const auto angle_z_increment
      = -(HalfCircleRads<float>() / (inNumLatitudes - (inIsHemisphere ? 2 : 1))) / (inIsHemisphere ? 2 : 1);
  const auto angle_y_increment = -FullCircleRads() / inNumLongitudes;
  auto angle_z = (inIsHemisphere ? 0.0f : QuarterCircleRads<float>());
  std::vector<float> vertex_id_to_angles_y(inNumLongitudes * inNumLatitudes, 0.0f);
  std::vector<float> vertex_id_to_angles_z(inNumLongitudes * inNumLatitudes, 0.0f);
  for (Mesh::VertexId z = 0; z < inNumLatitudes; ++z)
  {
    if (z == 0 || z == (inNumLatitudes - 1)) // South or north pole, single vertex
    {
      sphere.AddVertex(Vec3f { 0.0f, 0.0f, std::sin(angle_z) });
      vertex_id_to_angles_y.at(sphere.GetNumberOfVertices() - 1) = 0.0f;
      vertex_id_to_angles_z.at(sphere.GetNumberOfVertices() - 1) = angle_z;
      if (inIsHemisphere)
        continue;
    }
    else // Middle vertices
    {
      const auto position_z = std::sin(angle_z);
      const auto cos_z = std::cos(angle_z);

      auto angle_y = 0.0f;
      for (Mesh::VertexId y = 0; y < inNumLongitudes; ++y)
      {
        const auto position_x = cos_z * std::cos(angle_y);
        const auto position_y = cos_z * std::sin(angle_y);
        const auto point = Vec3f { position_x, position_y, position_z };
        sphere.AddVertex(point);

        angle_y += angle_y_increment;
        vertex_id_to_angles_y.at(sphere.GetNumberOfVertices() - 1) = angle_y;
        vertex_id_to_angles_z.at(sphere.GetNumberOfVertices() - 1) = angle_z;
      }
    }

    angle_z += angle_z_increment;
  }

  // Add faces
  for (Mesh::VertexId z = 0; z < inNumLatitudes - 1; ++z)
  {
    const bool is_current_south_pole = (z == 0);
    const bool is_up_north_pole = ((z + 1) == inNumLatitudes - 1);
    for (Mesh::VertexId y = 0; y < inNumLongitudes; ++y)
    {
      if (is_current_south_pole) // Current row is the south pole point
      {
        const auto south_pole_vertex_index = 0;
        const auto up_vertex_index = 1 + y;
        const auto up_right_vertex_index = 1 + ((y + 1) % inNumLongitudes);
        sphere.AddFace(south_pole_vertex_index, up_right_vertex_index, up_vertex_index);
      }
      else if (!is_up_north_pole) // Middle vertices
      {
        const auto current_vertex_index = (1 + (z - 1) * inNumLongitudes) + y;
        const auto right_vertex_index = (1 + (z - 1) * inNumLongitudes) + ((y + 1) % inNumLongitudes);
        const auto up_vertex_index = (1 + z * inNumLongitudes) + y;
        const auto up_right_vertex_index = (1 + z * inNumLongitudes) + ((y + 1) % inNumLongitudes);
        sphere.AddFace(current_vertex_index, right_vertex_index, up_right_vertex_index);
        sphere.AddFace(current_vertex_index, up_right_vertex_index, up_vertex_index);
      }
      else // Next row above is the north pole point
      {
        const auto current_vertex_index = (1 + (z - 1) * inNumLongitudes) + y;
        const auto right_vertex_index = (1 + (z - 1) * inNumLongitudes) + ((y + 1) % inNumLongitudes);
        const auto north_pole_vertex_index = (1 + ((inNumLatitudes - 2) * inNumLongitudes));
        sphere.AddFace(north_pole_vertex_index, current_vertex_index, right_vertex_index);
      }
    }
  }

  ConsolidateMesh(sphere);
  return sphere;
}

Mesh MeshFactory::GetCylinder(const std::size_t inNumVerticesX)
{
  EXPECTS(inNumVerticesX >= 3);

  Mesh cylinder;

  // Forward and back circle vertices
  for (const auto forward : { true, false })
  {
    for (Mesh::VertexId i = 0; i < inNumVerticesX; ++i)
    {
      const auto progression = (static_cast<float>(i) / inNumVerticesX);
      const auto angle = Map(progression, 0.0f, 1.0f, 0.0f, FullCircleRads());
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
  for (const auto forward : { true, false })
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

Mesh MeshFactory::GetTorus(const std::size_t inNumLatitudes, const std::size_t inNumLongitudes, const float inHoleSize)
{
  EXPECTS(inNumLatitudes >= 3);
  EXPECTS(inNumLongitudes >= 3);
  EXPECTS(inHoleSize >= 0.0001f && inHoleSize <= 0.999f);

  const auto hole_radius = (inHoleSize * 0.5f);
  const auto torus_radius = (1.0f - inHoleSize) * 0.5f;
  const auto angle_longitude_increment = (FullCircleRads() / inNumLongitudes);
  const auto angle_latitude_increment = (FullCircleRads() / inNumLatitudes);

  Mesh torus;

  auto angle_longitude = 0.0f;
  for (Mesh::VertexId longitude = 0; longitude < inNumLongitudes; ++longitude)
  {
    auto angle_latitude = 0.0f;
    for (Mesh::VertexId latitude = 0; latitude < inNumLatitudes; ++latitude)
    {
      const auto longitude_rotation = AngleAxis(angle_longitude, Forward<Vec3f>());
      const auto longitude_center_offset = (hole_radius + torus_radius) * (longitude_rotation * Down<Vec3f>());
      const auto latitude_circle_position_local = Vec3f { 0.0f, std::sin(angle_latitude), std::cos(angle_latitude) };
      const auto latitude_circle_position_global
          = longitude_center_offset + longitude_rotation * (torus_radius * latitude_circle_position_local);
      torus.AddVertex(latitude_circle_position_global);
      angle_latitude += angle_latitude_increment;
    }
    angle_longitude += angle_longitude_increment;
  }

  for (Mesh::VertexId longitude = 0; longitude < inNumLongitudes; ++longitude)
  {
    for (Mesh::VertexId latitude = 0; latitude < inNumLatitudes; ++latitude)
    {
      const auto current_longitude_current_latitude_vertex_id = (longitude * inNumLatitudes + latitude);
      const auto current_longitude_next_latitude_vertex_id
          = (longitude * inNumLatitudes + ((latitude + 1) % inNumLatitudes));
      const auto next_longitude_current_latitude_vertex_id
          = (((longitude + 1) % inNumLongitudes) * inNumLatitudes + latitude);
      const auto next_longitude_next_latitude_vertex_id
          = (((longitude + 1) % inNumLongitudes) * inNumLatitudes + ((latitude + 1) % inNumLatitudes));
      torus.AddFace(current_longitude_current_latitude_vertex_id,
          current_longitude_next_latitude_vertex_id,
          next_longitude_current_latitude_vertex_id);
      torus.AddFace(next_longitude_current_latitude_vertex_id,
          current_longitude_next_latitude_vertex_id,
          next_longitude_next_latitude_vertex_id);
    }
  }

  ConsolidateMesh(torus);

  return torus;
}

Mesh MeshFactory::GetPlane(const std::size_t inNumVerticesX, const std::size_t inNumVerticesY)
{
  EXPECTS(inNumVerticesX >= 2);
  EXPECTS(inNumVerticesY >= 2);

  Mesh plane;

  const auto stride_x = (inNumVerticesX - 1);
  const auto stride_y = (inNumVerticesY - 1);
  for (Mesh::VertexId y = 0; y < inNumVerticesY; ++y)
  {
    for (Mesh::VertexId x = 0; x < inNumVerticesX; ++x)
    {
      const auto progression_x = (static_cast<float>(x) / stride_x);
      const auto position_x = Map(progression_x, 0.0f, 1.0f, -0.5f, 0.5f);
      const auto progression_y = (static_cast<float>(y) / stride_y);
      const auto position_y = Map(progression_y, 0.0f, 1.0f, -0.5f, 0.5f);
      const auto position_z = 0.0f;
      const auto vertex_position = Vec3f { position_x, position_y, position_z };
      plane.AddVertex(vertex_position);
    }
  }

  for (Mesh::VertexId y = 0; y < (inNumVerticesY - 1); ++y)
  {
    for (Mesh::VertexId x = 0; x < (inNumVerticesX - 1); ++x)
    {
      const auto current_x_current_y_vertex_id = (y * inNumVerticesX + x);
      const auto next_x_current_y_vertex_id = (y * inNumVerticesX + (x + 1));
      const auto current_x_next_y_vertex_id = ((y + 1) * inNumVerticesX + x);
      const auto next_x_next_y_vertex_id = ((y + 1) * inNumVerticesX + (x + 1));
      plane.AddFace(current_x_current_y_vertex_id, next_x_current_y_vertex_id, next_x_next_y_vertex_id);
      plane.AddFace(current_x_current_y_vertex_id, next_x_next_y_vertex_id, current_x_next_y_vertex_id);
    }
  }

  for (Mesh::CornerId corner_id = 0; corner_id < plane.GetNumberOfCorners(); ++corner_id)
  {
    const auto vertex_id = plane.GetVertexIdFromCornerId(corner_id);
    const auto vertex_position = plane.GetVertexPosition(vertex_id);
    const auto corner_texture_coordinates
        = Map(XY(vertex_position), All<Vec2f>(-0.5f), All<Vec2f>(0.5f), All<Vec2f>(0.0f), All<Vec2f>(1.0f));
    plane.SetCornerTextureCoordinates(corner_id, corner_texture_coordinates);
  }

  ConsolidateMesh(plane);
  return plane;
}

Mesh MeshFactory::GetCircleSection(const std::size_t inNumVertices, const float inSectionAngleRads)
{
  EXPECTS(inNumVertices >= 3);
  EXPECTS(IsBetween(inSectionAngleRads, 0.0f, FullCircleRads()));

  bool isFullCircle = (inSectionAngleRads == FullCircleRads());

  Mesh circle_section;
  {
    const auto center_vertex = Zero<Vec3f>();
    circle_section.AddVertex(center_vertex);
  }

  for (Mesh::VertexId i = 0; i < inNumVertices; ++i)
  {
    const auto progress = (static_cast<float>(i) / (isFullCircle ? inNumVertices : (inNumVertices - 1)));
    const auto angle = Map(progress, 0.0f, 1.0f, 0.0f, inSectionAngleRads);
    const auto circle_position = Vec3f { std::cos(angle), std::sin(angle), 0.0f };
    circle_section.AddVertex(circle_position);
  }

  for (Mesh::VertexId i = 0; i < (isFullCircle ? inNumVertices : (inNumVertices - 1)); ++i)
  {
    const auto current_i = (i + 1);
    const auto next_i = ((i + 1) % inNumVertices) + 1;
    circle_section.AddFace(0, current_i, next_i);
  }

  ConsolidateMesh(circle_section);
  return circle_section;
}

Mesh MeshFactory::GetCircle(const std::size_t inNumVertices)
{
  return GetCircleSection(inNumVertices, FullCircleRads());
}

void MeshFactory::ConsolidateMesh(Mesh& ioMesh, const bool inUpdateNormals)
{
  constexpr auto max_smooth_angle = DegreeToRad(45.0f);
  ioMesh.ComputeOppositeCornerIds();

  if (inUpdateNormals)
  {
    ioMesh.ComputeNormals(max_smooth_angle);
  }
}
}