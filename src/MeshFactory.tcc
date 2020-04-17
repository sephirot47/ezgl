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

  cube.SetCornerTextureCoordinates(0, Vec2f(0.0f, 0.0f));
  cube.SetCornerTextureCoordinates(1, Vec2f(0.5f, 0.5f));
  cube.SetCornerTextureCoordinates(2, Vec2f(1.0f, 0.5f));

  ConsolidateMesh(cube);
  return cube;
}

template <typename TMesh>
TMesh GMeshFactory<TMesh>::GetSphere(const std::size_t inNumLatitudes, const std::size_t inNumLongitudes)
{
  return GetSphere(inNumLatitudes, inNumLongitudes, false);
}

template <typename TMesh>
TMesh GMeshFactory<TMesh>::GetHemisphere(const std::size_t inNumLatitudes, const std::size_t inNumLongitudes)
{
  return GetSphere(inNumLatitudes + 1, inNumLongitudes, true);
}

template <typename TMesh>
TMesh GMeshFactory<TMesh>::GetCone(const std::size_t inNumLongitudes)
{
  auto cone = GetHemisphere(2, inNumLongitudes);
  cone.Transform(ScaleMat4(Vec3f { 1.0f, 1.0f, 2.0f }));
  ConsolidateMesh(cone);
  return cone;
}

template <typename TMesh>
TMesh GMeshFactory<TMesh>::GetSphere(const std::size_t inNumLatitudes,
    const std::size_t inNumLongitudes,
    const bool inIsHemisphere)
{
  EXPECTS(inNumLatitudes >= 3);
  EXPECTS(inNumLongitudes >= 3);

  TMesh sphere;

  // Add vertices
  const auto angle_z_increment
      = -(HalfCircleRads<float>() / (inNumLatitudes - (inIsHemisphere ? 2 : 1))) / (inIsHemisphere ? 2 : 1);
  const auto angle_y_increment = -FullCircleRads<float>() / inNumLongitudes;
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

  // Set texture coordinates
  for (Mesh::CornerId corner_id = 0; corner_id < sphere.GetNumberOfCorners(); ++corner_id)
  {
    const auto face_id = sphere.GetFaceIdFromCornerId(corner_id);
    const auto vertex_id = sphere.GetVertexIdFromCornerId(corner_id);
    const auto corner_position = sphere.GetVertexPosition(vertex_id);
    const auto x = corner_position[0];
    const auto y = corner_position[1];
    const auto z = corner_position[2];
    const auto angle_y = vertex_id_to_angles_y.at(vertex_id);
    const auto angle_z = vertex_id_to_angles_z.at(vertex_id);
    auto u = (angle_y + FullCircleRads<float>()) / FullCircleRads<float>();
    auto v = (angle_z + QuarterCircleRads<float>()) / HalfCircleRads<float>();

    const auto corner_texture_coordinates = Vec2f { u, v };
    sphere.SetCornerTextureCoordinates(corner_id, corner_texture_coordinates);
  }

  // Fix texture coordinates seams
  for (Mesh::CornerId corner_id = 0; corner_id < sphere.GetNumberOfCorners(); ++corner_id)
  {
    const auto vertex_id = sphere.GetVertexIdFromCornerId(corner_id);
    const auto is_pole_vertex = (vertex_id == 0 || vertex_id >= (sphere.GetNumberOfVertices() - 1));
    if (is_pole_vertex)
      continue;

    const auto corner_texture_coordinates = sphere.GetCornerTextureCoordinates(corner_id);
    if (!VeryEqual(corner_texture_coordinates[0], 0.0f))
      continue;

    const auto face_id = sphere.GetFaceIdFromCornerId(corner_id);
    for (const auto other_corner_id : sphere.GetFaceCornersIds(face_id))
    {
      const auto other_corner_texture_coordinates = sphere.GetCornerTextureCoordinates(other_corner_id);
      if (Distance(corner_texture_coordinates[0], other_corner_texture_coordinates[0]) >= (1.1f / inNumLongitudes))
      {
        const auto new_corner_texture_coordinates = Vec2f { 1.0f, corner_texture_coordinates[1] };
        sphere.SetCornerTextureCoordinates(corner_id, new_corner_texture_coordinates);
        break;
      }
    }
  }

  // Fix texture coordinates at poles
  for (Mesh::CornerId corner_id = 0; corner_id < sphere.GetNumberOfCorners(); ++corner_id)
  {
    const auto vertex_id = sphere.GetVertexIdFromCornerId(corner_id);
    const auto is_south_pole = (vertex_id == 0);
    const auto is_north_pole = (vertex_id == (sphere.GetNumberOfVertices() - 1));
    if (!is_south_pole && !is_north_pole)
      continue;

    const auto face_id = sphere.GetFaceIdFromCornerId(corner_id);
    const auto corner_texture_coordinates = sphere.GetCornerTextureCoordinates(corner_id);
    const auto other_face_corner_ids = sphere.GetFaceOtherCornerIds(face_id, corner_id);

    auto other_corner_texture_coordinate_0 = sphere.GetCornerTextureCoordinates(other_face_corner_ids[0]);
    auto other_corner_texture_coordinate_1 = sphere.GetCornerTextureCoordinates(other_face_corner_ids[1]);
    if (Distance(other_corner_texture_coordinate_0[0], other_corner_texture_coordinate_1[0])
        >= (1.1f / inNumLongitudes))
    {
      if (other_corner_texture_coordinate_0[0] == 1)
        other_corner_texture_coordinate_0[0] = 0;
      else
        other_corner_texture_coordinate_1[0] = 0;
    }

    const auto new_corner_texture_coordinate_u
        = (other_corner_texture_coordinate_0[0] + other_corner_texture_coordinate_1[0]) * 0.5f;
    const auto new_corner_texture_coordinate_v = (is_south_pole ? 1.0f : 0.0f);
    sphere.SetCornerTextureCoordinates(other_face_corner_ids[0], other_corner_texture_coordinate_0);
    sphere.SetCornerTextureCoordinates(other_face_corner_ids[1], other_corner_texture_coordinate_1);
    sphere.SetCornerTextureCoordinates(corner_id,
        Vec2f { new_corner_texture_coordinate_u, new_corner_texture_coordinate_v });
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
  ioMesh.ComputeOppositeCornerIds();
  ioMesh.ComputeNormals(max_smooth_angle);

  if constexpr (std::is_same_v<TMesh, DrawableMesh>)
  {
    ioMesh.UpdateVAOs();
  }
}
}