#include "ez/Mesh.h"
#include "ez/Macros.h"
#include "ez/Math.h"
#include "ez/MeshIO.h"
#include "ez/MeshIterators.h"
#include "ez/StreamOperators.h"
#include "ez/Transformation.h"
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <utility>

namespace ez
{
Mesh::VertexId Mesh::AddVertex(const Vec3f& inPosition)
{
  Mesh::VertexData vertex_data;
  vertex_data.mPosition = inPosition;
  mVerticesData.push_back(std::move(vertex_data));
  mCornerTableComputed = false;

  const auto new_vertex_id = mVerticesData.size() - 1;
  return new_vertex_id;
}

Mesh::FaceId Mesh::AddFace(const Mesh::VertexId& inFaceVertexId0,
    const Mesh::VertexId& inFaceVertexId1,
    const Mesh::VertexId& inFaceVertexId2)
{
  EXPECTS(inFaceVertexId0 < GetNumberOfVertices());
  EXPECTS(inFaceVertexId1 < GetNumberOfVertices());
  EXPECTS(inFaceVertexId2 < GetNumberOfVertices());

  // Add face
  mFacesData.emplace_back();
  const auto new_face_id = mFacesData.size() - 1;
  const std::array new_face_vertices_ids = { inFaceVertexId0, inFaceVertexId1, inFaceVertexId2 };
  mFacesData.at(new_face_id).mVerticesIds = new_face_vertices_ids;

  mVerticesData.at(inFaceVertexId0).mFaceId = new_face_id;
  mVerticesData.at(inFaceVertexId1).mFaceId = new_face_id;
  mVerticesData.at(inFaceVertexId2).mFaceId = new_face_id;

  // For each vertex of the new face, create corners
  for (Mesh::InternalCornerId internal_corner_id = 0; internal_corner_id < 3; ++internal_corner_id)
  { mCornersData.emplace_back(); }
  mCornerTableComputed = false;

  return new_face_id;
}

void Mesh::SetFaceNormal(const Mesh::FaceId inFaceId, const Vec3f& inFaceNormal)
{
  EXPECTS(inFaceId < mFacesData.size());
  mFacesData.at(inFaceId).mNormal = inFaceNormal;
}

void Mesh::SetCornerNormal(const Mesh::CornerId inCornerId, const Vec3f& inCornerNormal)
{
  EXPECTS(inCornerId < mCornersData.size());
  mCornersData.at(inCornerId).mNormal = inCornerNormal;
}

void Mesh::SetCornerTextureCoordinates(const Mesh::CornerId& inCornerId, const Vec2f& inTextureCoordinates)
{
  EXPECTS(inCornerId < mCornersData.size());
  mCornersData.at(inCornerId).mTextureCoordinates = inTextureCoordinates;
}

const Vec3f& Mesh::GetVertexPosition(const Mesh::VertexId& inVertexId) const
{
  EXPECTS(inVertexId < mVerticesData.size());
  return mVerticesData.at(inVertexId).mPosition;
}

const Vec3f& Mesh::GetFaceNormal(const Mesh::FaceId& inFaceId) const
{
  EXPECTS(inFaceId < mFacesData.size());
  return mFacesData.at(inFaceId).mNormal;
}

Triangle3f Mesh::GetFaceTriangle(const Mesh::FaceId& inFaceId) const
{
  EXPECTS(inFaceId < mFacesData.size());
  const auto& face_data = mFacesData.at(inFaceId);
  return Triangle3f(mVerticesData.at(face_data.mVerticesIds[0]).mPosition,
      mVerticesData.at(face_data.mVerticesIds[1]).mPosition,
      mVerticesData.at(face_data.mVerticesIds[2]).mPosition);
}

const Vec3f& Mesh::GetCornerNormal(const Mesh::CornerId& inCornerId) const
{
  EXPECTS(inCornerId < mCornersData.size());
  return mCornersData.at(inCornerId).mNormal;
}

const Vec2f& Mesh::GetCornerTextureCoordinates(const Mesh::CornerId& inCornerId) const
{
  EXPECTS(inCornerId < mCornersData.size());
  return mCornersData.at(inCornerId).mTextureCoordinates;
}

Mesh::VertexId Mesh::GetVertexIdFromCornerId(const Mesh::CornerId inCornerId) const
{
  EXPECTS(inCornerId < GetNumberOfCorners());
  const auto face_id = GetFaceIdFromCornerId(inCornerId);
  const auto internal_corner_id = (inCornerId % 3);
  return GetVertexIdFromFaceIdAndInternalCornerId(face_id, internal_corner_id);
}

Mesh::VertexId Mesh::GetVertexIdFromFaceIdAndInternalCornerId(const Mesh::FaceId inFaceId,
    const Mesh::InternalCornerId inInternalCornerId) const
{
  return mFacesData.at(inFaceId).mVerticesIds.at(inInternalCornerId);
}

Mesh::FaceId Mesh::GetFaceIdFromCornerId(const Mesh::CornerId inCornerId) const
{
  EXPECTS(inCornerId < GetNumberOfCorners());
  return inCornerId / 3;
}

Mesh::CornerId Mesh::GetOppositeCornerId(const Mesh::CornerId inCornerId) const
{
  EXPECTS(mCornerTableComputed);
  EXPECTS(inCornerId < mCornersData.size());
  return mCornersData.at(inCornerId).mOppositeCornerId;
}

Mesh::FaceId Mesh::GetOppositeFaceId(const Mesh::CornerId inCornerId) const
{
  const auto opposite_corner_id = GetOppositeCornerId(inCornerId);
  if (opposite_corner_id == Mesh::InvalidId)
    return Mesh::InvalidId;
  return GetFaceIdFromCornerId(opposite_corner_id);
}

std::array<Mesh::Edge, 3> Mesh::GetFaceEdges(const Mesh::FaceId inFaceId) const
{
  const auto& face_data = mFacesData.at(inFaceId);
  const auto& face_vertices_ids = face_data.mVerticesIds;
  const auto& face_vertex_id_0 = face_vertices_ids.at(0);
  const auto& face_vertex_id_1 = face_vertices_ids.at(1);
  const auto& face_vertex_id_2 = face_vertices_ids.at(2);

  const auto edge_01 = Edge(face_vertex_id_0, face_vertex_id_1);
  const auto edge_02 = Edge(face_vertex_id_0, face_vertex_id_2);
  const auto edge_12 = Edge(face_vertex_id_1, face_vertex_id_2);
  return { edge_01, edge_02, edge_12 };
}

void Mesh::Clear()
{
  mVerticesData.clear();
  mCornersData.clear();
  mFacesData.clear();
}

std::array<Mesh::CornerId, 3> Mesh::GetFaceCornersIds(const Mesh::FaceId inFaceId) const
{
  const auto base_corner_id = (inFaceId * 3);
  return { base_corner_id + 0, base_corner_id + 1, base_corner_id + 2 };
}

std::array<Mesh::VertexId, 3> Mesh::GetFaceVerticesIds(const Mesh::FaceId inFaceId) const
{
  const auto base_corner_id = (inFaceId * 3);
  return { GetVertexIdFromCornerId(base_corner_id + 0),
    GetVertexIdFromCornerId(base_corner_id + 1),
    GetVertexIdFromCornerId(base_corner_id + 2) };
}

std::array<Mesh::CornerId, 2> Mesh::GetFaceOtherCornerIds(const Mesh::FaceId inFaceId,
    const Mesh::CornerId inCornerId) const
{
  const auto face_corner_ids = GetFaceCornersIds(inFaceId);
  if (inCornerId == face_corner_ids[0])
    return { face_corner_ids[1], face_corner_ids[2] };
  else if (inCornerId == face_corner_ids[1])
    return { face_corner_ids[0], face_corner_ids[2] };
  return { face_corner_ids[0], face_corner_ids[1] };
}

std::array<Mesh::VertexId, 2> Mesh::GetFaceOtherVertexIds(const Mesh::FaceId inFaceId,
    const Mesh::VertexId inVertexId) const
{
  const auto face_vertex_ids = GetFaceVerticesIds(inFaceId);
  if (inVertexId == face_vertex_ids[0])
    return { face_vertex_ids[1], face_vertex_ids[2] };
  else if (inVertexId == face_vertex_ids[1])
    return { face_vertex_ids[0], face_vertex_ids[2] };
  return { face_vertex_ids[0], face_vertex_ids[1] };
}

Mesh::CornerId Mesh::GetFaceOtherCornerId(const Mesh::FaceId inFaceId,
    const Mesh::CornerId inCornerId0,
    const Mesh::CornerId inCornerId1) const
{
  const auto face_corners_ids = GetFaceCornersIds(inFaceId);
  EXPECTS(inCornerId0 != inCornerId1);
  EXPECTS(inCornerId0 == face_corners_ids.at(0) || inCornerId0 == face_corners_ids.at(1)
      || inCornerId0 == face_corners_ids.at(2));
  EXPECTS(inCornerId1 == face_corners_ids.at(0) || inCornerId1 == face_corners_ids.at(1)
      || inCornerId1 == face_corners_ids.at(2));

  if (face_corners_ids.at(0) != inCornerId0 && face_corners_ids.at(0) != inCornerId1)
    return face_corners_ids.at(0);
  if (face_corners_ids.at(1) != inCornerId0 && face_corners_ids.at(1) != inCornerId1)
    return face_corners_ids.at(1);
  return face_corners_ids.at(2);
}

Mesh::VertexId Mesh::GetFaceOtherVertexId(const Mesh::FaceId inFaceId,
    const Mesh::VertexId inVertexId0,
    const Mesh::VertexId inVertexId1) const
{
  const auto face_vertex_ids = GetFaceVerticesIds(inFaceId);
  EXPECTS(inVertexId0 != inVertexId1);
  EXPECTS(inVertexId0 == face_vertex_ids.at(0) || inVertexId0 == face_vertex_ids.at(1)
      || inVertexId0 == face_vertex_ids.at(2));
  EXPECTS(inVertexId1 == face_vertex_ids.at(0) || inVertexId1 == face_vertex_ids.at(1)
      || inVertexId1 == face_vertex_ids.at(2));

  if (face_vertex_ids.at(0) != inVertexId0 && face_vertex_ids.at(0) != inVertexId1)
    return face_vertex_ids.at(0);
  if (face_vertex_ids.at(1) != inVertexId0 && face_vertex_ids.at(1) != inVertexId1)
    return face_vertex_ids.at(1);
  return face_vertex_ids.at(2);
}

Mesh::CornerId Mesh::GetCornerIdFromFaceIdAndVertexId(const Mesh::FaceId inFaceId,
    const Mesh::VertexId inVertexId) const
{
  EXPECTS(inFaceId < GetNumberOfFaces());
  EXPECTS(inVertexId < GetNumberOfVertices());

  const auto& vertex_id_0 = mFacesData.at(inFaceId).mVerticesIds.at(0);
  if (inVertexId == vertex_id_0)
    return (inFaceId * 3 + 0);

  const auto& vertex_id_1 = mFacesData.at(inFaceId).mVerticesIds.at(1);
  if (inVertexId == vertex_id_1)
    return (inFaceId * 3 + 1);

  return (inFaceId * 3 + 2);
}

Mesh::CornerId Mesh::GetCornerIdFromFaceIdAndInternalCornerId(const Mesh::FaceId inFaceId,
    const Mesh::InternalCornerId inInternalCornerId) const
{
  return inFaceId * 3 + inInternalCornerId;
}

Mesh::CornerId Mesh::GetPreviousCornerId(const Mesh::CornerId inCornerId) const
{
  return (GetFaceIdFromCornerId(inCornerId) * 3) + ((inCornerId + 2) % 3);
}

Mesh::CornerId Mesh::GetNextCornerId(const Mesh::CornerId inCornerId) const
{
  return (GetFaceIdFromCornerId(inCornerId) * 3) + ((inCornerId + 1) % 3);
}

Mesh::CornerId Mesh::GetNextAdjacentCornerId(const Mesh::CornerId inCornerId) const
{
  EXPECTS(inCornerId < GetNumberOfCorners());
  auto opposite_of_next_corner_id = GetOppositeCornerId(GetNextCornerId(inCornerId));
  if (opposite_of_next_corner_id == Mesh::InvalidId)
    return Mesh::InvalidId;
  return GetNextCornerId(opposite_of_next_corner_id);
}

Mesh::CornerId Mesh::GetPreviousAdjacentCornerId(const Mesh::CornerId inCornerId) const
{
  EXPECTS(inCornerId < GetNumberOfCorners());
  auto opposite_of_previous_corner_id = GetOppositeCornerId(GetPreviousCornerId(inCornerId));
  if (opposite_of_previous_corner_id == Mesh::InvalidId)
    return Mesh::InvalidId;
  return GetPreviousCornerId(opposite_of_previous_corner_id);
}

Mesh::CornerId Mesh::GetNextAdjacentFaceId(const Mesh::CornerId inCornerId) const
{
  EXPECTS(inCornerId < GetNumberOfCorners());
  const auto next_adjacent_corner_id = GetNextAdjacentCornerId(inCornerId);
  if (next_adjacent_corner_id == Mesh::InvalidId)
    return Mesh::InvalidId;
  return GetFaceIdFromCornerId(next_adjacent_corner_id);
}

Mesh::CornerId Mesh::GetPreviousAdjacentFaceId(const Mesh::CornerId inCornerId) const
{
  EXPECTS(inCornerId < GetNumberOfCorners());
  const auto previous_adjacent_corner_id = GetPreviousAdjacentCornerId(inCornerId);
  if (previous_adjacent_corner_id == Mesh::InvalidId)
    return Mesh::InvalidId;
  return GetFaceIdFromCornerId(previous_adjacent_corner_id);
}

std::vector<Triangle3f> Mesh::GetTriangles() const
{
  std::vector<Triangle3f> mesh_triangles;
  mesh_triangles.reserve(GetNumberOfFaces());
  for (std::size_t face_id = 0; face_id < GetNumberOfFaces(); ++face_id)
  { mesh_triangles.push_back(GetFaceTriangle(face_id)); }
  return mesh_triangles;
}

void Mesh::SetVertexPosition(const Mesh::VertexId inVertexId, const Vec3f& inPosition)
{
  EXPECTS(inVertexId < GetNumberOfVertices());
  mVerticesData.at(inVertexId).mPosition = inPosition;
}

void Mesh::Transform(const Mat4f& inTransform)
{
  for (auto& vertex_data : mVerticesData) { vertex_data.mPosition = Transformed(vertex_data.mPosition, inTransform); }
}

/*
Mesh::CirculatorVertexNeighborFaceIds Mesh::GetVertexNeighborFaceIdsCirculatorBegin(
    const Mesh::VertexId inVertexId) const
{
  return Mesh::CirculatorVertexNeighborFaceIds(*this, inVertexId);
}

Mesh::CirculatorVertexNeighborFaceIds Mesh::GetVertexNeighborFaceIdsCirculatorEnd() const
{
  return Mesh::CirculatorVertexNeighborFaceIds(*this, Mesh::InvalidId);
}

Range<Mesh::CirculatorVertexNeighborFaceIds> Mesh::AllVertexNeighborFaceIds(const Mesh::VertexId inVertexId) const
{
  return { GetVertexNeighborFaceIdsCirculatorBegin(inVertexId), GetVertexNeighborFaceIdsCirculatorEnd() };
}
*/

std::vector<Mesh::CornerId> Mesh::GetNeighborCornersIds(const Mesh::VertexId inVertexId) const
{
  EXPECTS(inVertexId < GetNumberOfVertices());

  const auto corner_ids = GetVertexCornersIds(inVertexId);

  std::vector<Mesh::VertexId> neighbor_corners_ids;
  neighbor_corners_ids.reserve(corner_ids.size() * 2);

  for (const auto& corner_id : corner_ids)
  {
    const auto next_corner_id = GetNextCornerId(corner_id);
    const auto next_next_corner_id = GetNextCornerId(next_corner_id);
    neighbor_corners_ids.push_back(next_corner_id);
    neighbor_corners_ids.push_back(next_next_corner_id);
  }

  return neighbor_corners_ids;
}

std::vector<Mesh::VertexId> Mesh::GetNeighborVerticesIds(const Mesh::VertexId inVertexId) const
{
  EXPECTS(inVertexId < GetNumberOfVertices());

  const auto corner_ids = GetVertexCornersIds(inVertexId);

  std::vector<Mesh::VertexId> neighbor_vertices_ids;
  neighbor_vertices_ids.reserve(corner_ids.size());

  std::transform(corner_ids.cbegin(),
      corner_ids.cend(),
      std::back_inserter(neighbor_vertices_ids),
      [this](const Mesh::CornerId& inCornerId) {
        const auto next_corner_id = GetNextCornerId(inCornerId);
        const auto vertex_id = GetVertexIdFromCornerId(next_corner_id);
        return vertex_id;
      });

  return neighbor_vertices_ids;
}

std::vector<Mesh::FaceId> Mesh::GetNeighborFacesIds(const Mesh::VertexId inVertexId) const
{
  EXPECTS(inVertexId < GetNumberOfVertices());

  const auto corner_ids = GetVertexCornersIds(inVertexId);

  std::vector<Mesh::FaceId> neighbor_face_ids;
  neighbor_face_ids.reserve(corner_ids.size());

  std::transform(corner_ids.cbegin(),
      corner_ids.cend(),
      std::back_inserter(neighbor_face_ids),
      [this](const Mesh::CornerId& inCornerId) {
        const auto face_id = GetFaceIdFromCornerId(inCornerId);
        return face_id;
      });

  return neighbor_face_ids;
}

std::vector<Mesh::CornerId> Mesh::GetVertexCornersIds(const Mesh::VertexId inVertexId) const
{
  EXPECTS(inVertexId < GetNumberOfVertices());

  std::vector<Mesh::CornerId> corner_ids;
  corner_ids.reserve(6);

  const auto& vertex_data = mVerticesData.at(inVertexId);
  const auto start_corner_id = GetCornerIdFromFaceIdAndVertexId(vertex_data.mFaceId, inVertexId);

  auto corner_id = start_corner_id;
  do
  {
    corner_ids.push_back(corner_id);
    corner_id = GetNextAdjacentCornerId(corner_id);
  } while (corner_id != Mesh::InvalidId && corner_id != start_corner_id);

  return corner_ids;
}

bool Mesh::IsValid(const Mesh::Id inId) { return inId != Mesh::InvalidId; }

bool Mesh::IsValid(const std::optional<Mesh::Id> inOptionalId)
{
  return inOptionalId.has_value() ? Mesh::IsValid(*inOptionalId) : false;
}

const std::vector<Mesh::VertexData>& Mesh::GetVerticesData() const { return mVerticesData; }

const std::vector<Mesh::CornerData>& Mesh::GetCornersData() const { return mCornersData; }

const std::vector<Mesh::FaceData>& Mesh::GetFacesData() const { return mFacesData; }

std::size_t Mesh::GetNumberOfFaces() const { return mFacesData.size(); }

std::size_t Mesh::GetNumberOfVertices() const { return mVerticesData.size(); }

std::size_t Mesh::GetNumberOfCorners() const { return mCornersData.size(); }

void Mesh::ComputeFaceNormals()
{
  for (auto& face_data : mFacesData)
  {
    const auto vertex_position_0 = mVerticesData.at(face_data.mVerticesIds[0]).mPosition;
    const auto vertex_position_1 = mVerticesData.at(face_data.mVerticesIds[1]).mPosition;
    const auto vertex_position_2 = mVerticesData.at(face_data.mVerticesIds[2]).mPosition;
    const auto v1_v0 = (vertex_position_0 - vertex_position_1);
    const auto v1_v2 = (vertex_position_2 - vertex_position_1);
    const auto face_normal = NormalizedSafe(Cross(v1_v2, v1_v0));
    face_data.mNormal = face_normal;
  }
}

void Mesh::ComputeCornerNormals(const float inMinEdgeAngleToSmooth)
{
  const auto min_edge_dot_to_smooth = std::cos(inMinEdgeAngleToSmooth);
  for (Mesh::CornerId corner_id = 0; corner_id < mCornersData.size(); ++corner_id)
  {
    auto& corner_data = mCornersData.at(corner_id);
    const auto face_id = (corner_id / 3);
    const auto internal_corner_id = (corner_id % 3);
    const auto vertex_id = mFacesData.at(face_id).mVerticesIds.at(internal_corner_id);
    const auto& face_normal = mFacesData.at(face_id).mNormal;
    assert(IsNormalized(face_normal));

    const auto neighbor_face_ids = GetNeighborFacesIds(vertex_id);
    auto normal_sum = Zero<Vec3f>();
    for (const auto& neighbor_face_id : neighbor_face_ids)
    {
      const auto& neighbor_face_normal = mFacesData.at(neighbor_face_id).mNormal;
      assert(IsNormalized(neighbor_face_normal));

      const auto edge_dot = Dot(face_normal, neighbor_face_normal);
      if (edge_dot >= min_edge_dot_to_smooth)
        normal_sum += neighbor_face_normal;
    }

    const auto corner_normal = NormalizedSafe(normal_sum);
    corner_data.mNormal = corner_normal;
  }
}

void Mesh::ComputeNormals(const float inMinEdgeAngleToSmooth)
{
  ComputeFaceNormals();
  ComputeCornerNormals(inMinEdgeAngleToSmooth);
}

void Mesh::ComputeCornerTable()
{
  std::vector<std::vector<FaceId>> vertex_to_neighbor_faces_ids(GetNumberOfVertices());
  std::for_each(vertex_to_neighbor_faces_ids.begin(), vertex_to_neighbor_faces_ids.end(), [](auto& v) {
    v.reserve(6);
  });

  for (FaceId face_id = 0; face_id < GetNumberOfFaces(); ++face_id)
  {
    const auto face_vertices_ids = mFacesData.at(face_id).mVerticesIds;
    for (const auto& face_vertex_id : face_vertices_ids)
    { vertex_to_neighbor_faces_ids.at(face_vertex_id).push_back(face_id); }
  }

  for (VertexId vertex_id = 0; vertex_id < GetNumberOfVertices(); ++vertex_id)
  {
    const auto& vertex_neighbor_faces_ids = vertex_to_neighbor_faces_ids.at(vertex_id);
    for (const auto& vertex_neighbor_face_id : vertex_neighbor_faces_ids)
    {
      const auto face_other_vertices_id = GetFaceOtherVertexIds(vertex_neighbor_face_id, vertex_id);
      for (std::size_t i = 0; i <= 1; ++i)
      {
        for (const auto& other_vertex_neighbor_face_id : vertex_neighbor_faces_ids)
        {
          if (vertex_neighbor_face_id == other_vertex_neighbor_face_id)
            continue;

          const auto other_face_other_vertices_id = GetFaceOtherVertexIds(other_vertex_neighbor_face_id, vertex_id);
          auto matching_face_other_vertices_corner_i = Max<InternalCornerId>();
          auto matching_other_face_other_vertices_corner_i = Max<InternalCornerId>();
          for (std::size_t j = 0; j <= 1; ++j)
          {
            if (face_other_vertices_id[i] == other_face_other_vertices_id[j])
            {
              matching_face_other_vertices_corner_i = i;
              matching_other_face_other_vertices_corner_i = j;
              break;
            }
          }

          const auto found_matching_other_vertex_id = (matching_face_other_vertices_corner_i <= 1);
          if (!found_matching_other_vertex_id)
            continue;

          const auto corner_id = GetCornerIdFromFaceIdAndVertexId(vertex_neighbor_face_id,
              face_other_vertices_id[1 - matching_face_other_vertices_corner_i]);
          const auto other_corner_id = GetCornerIdFromFaceIdAndVertexId(other_vertex_neighbor_face_id,
              other_face_other_vertices_id[1 - matching_other_face_other_vertices_corner_i]);
          mCornersData.at(corner_id).mOppositeCornerId = other_corner_id;
          mCornersData.at(other_corner_id).mOppositeCornerId = corner_id;
          break;
        }
      }
    }
  }

  mCornerTableComputed = true;
}

#ifdef MESH_IO
void Mesh::Read(const std::filesystem::path& inMeshPath) { MeshIO::Read(inMeshPath, *this); }

void Mesh::Write(const std::filesystem::path& inMeshPath, const bool inPreserveVerticesIds) const
{
  MeshIO::Write(*this, inMeshPath, inPreserveVerticesIds);
}
#endif

std::ostream& operator<<(std::ostream& ioLHS, const Mesh::Edge& inRHS)
{
  ioLHS << "Edge<" << inRHS[0] << ", " << inRHS[1] << ">";
  return ioLHS;
}

std::ostream& operator<<(std::ostream& ioLHS, const Mesh::VertexData& inRHS)
{
  ioLHS << "VertexData<Position: " << inRHS.mPosition << ">";
  return ioLHS;
}
}