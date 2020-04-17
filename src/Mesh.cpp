#include "Mesh.h"
#include "Macros.h"
#include "Math.h"
#include "MeshIterators.h"
#include "StreamOperators.h"
#include <algorithm>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cassert>
#include <unordered_map>
#include <utility>

namespace egl
{
void Mesh::AddVertex(const Vec3f& inPosition)
{
  Mesh::VertexData vertex_data;
  vertex_data.mPosition = inPosition;
  mVerticesData.push_back(std::move(vertex_data));
  mOppositeCornerIdsComputed = false;
}

void Mesh::AddFace(const Mesh::VertexId& inFaceVertexId0,
    const Mesh::VertexId& inFaceVertexId1,
    const Mesh::VertexId& inFaceVertexId2)
{
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
  {
    mCornersData.emplace_back();
  }
  mOppositeCornerIdsComputed = false;
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

const Vec3f& Mesh::GetVertexPosition(const Mesh::VertexId& inVertexId)
{
  EXPECTS(inVertexId < mVerticesData.size());
  return mVerticesData.at(inVertexId).mPosition;
}

const Vec3f& Mesh::GetFaceNormal(const Mesh::FaceId& inFaceId)
{
  EXPECTS(inFaceId < mFacesData.size());
  return mFacesData.at(inFaceId).mNormal;
}

const Vec3f& Mesh::GetCornerNormal(const Mesh::CornerId& inCornerId)
{
  EXPECTS(inCornerId < mCornersData.size());
  return mCornersData.at(inCornerId).mNormal;
}

const Vec2f& Mesh::GetCornerTextureCoordinates(const Mesh::CornerId& inCornerId)
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
  EXPECTS(mOppositeCornerIdsComputed);
  EXPECTS(inCornerId < mCornersData.size());
  return mCornersData.at(inCornerId).mOppositeCornedId;
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

Mesh::CornerId Mesh::GetPreviousCornerId(const Mesh::CornerId inCornerId) const
{
  return GetFaceIdFromCornerId(inCornerId) * 3 + ((inCornerId + 2) % 3);
}

Mesh::CornerId Mesh::GetNextCornerId(const Mesh::CornerId inCornerId) const
{
  return GetFaceIdFromCornerId(inCornerId) * 3 + ((inCornerId + 1) % 3);
}

Mesh::CornerId Mesh::GetNextAdjacentCornerId(const Mesh::CornerId inCornerId) const
{
  return GetOppositeCornerId(GetNextCornerId(inCornerId));
}

Mesh::CornerId Mesh::GetPreviousAdjacentCornerId(const Mesh::CornerId inCornerId) const
{
  return GetOppositeCornerId(GetPreviousCornerId(inCornerId));
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

void Mesh::SetVertexPosition(const Mesh::VertexId inVertexId, const Vec3f& inPosition)
{
  EXPECTS(inVertexId < GetNumberOfVertices());
  mVerticesData.at(inVertexId).mPosition = inPosition;
}

void Mesh::Transform(const Mat4f& inTransform)
{
  for (auto& vertex_data : mVerticesData)
  {
    vertex_data.mPosition = XYZ(inTransform * XYZ1(vertex_data.mPosition));
  }
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

std::vector<Mesh::VertexId> Mesh::GetNeighborVerticesIds(const Mesh::VertexId inVertexId) const
{
  EXPECTS(inVertexId < GetNumberOfVertices());

  std::vector<Mesh::VertexId> neighbor_vertices;
  neighbor_vertices.reserve(6);

  const auto& vertex_data = mVerticesData.at(inVertexId);
  auto neighbor_face_id = vertex_data.mFaceId;
  do
  {
    const auto neighbor_corner_id = GetCornerIdFromFaceIdAndVertexId(neighbor_face_id, inVertexId);
    neighbor_face_id = GetNextAdjacentFaceId(neighbor_corner_id);
    const auto neighbor_vertex_id = GetVertexIdFromCornerId(GetPreviousCornerId(neighbor_corner_id));
    neighbor_vertices.push_back(neighbor_vertex_id);
  } while (neighbor_face_id != Mesh::InvalidId && neighbor_face_id != vertex_data.mFaceId);

  return neighbor_vertices;
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

    auto normal_sum = Zero<Vec3f>();
    {
      const auto& vertex_data = mVerticesData.at(vertex_id);
      auto neighbor_face_id = vertex_data.mFaceId;
      do
      {
        const auto neighbor_corner_id = GetCornerIdFromFaceIdAndVertexId(neighbor_face_id, vertex_id);
        if (face_id == neighbor_face_id)
        {
          normal_sum += face_normal;
        }
        else
        {
          const auto neighbor_face_normal = mFacesData.at(neighbor_face_id).mNormal;
          assert(IsNormalized(neighbor_face_normal));

          const auto edge_dot = Dot(face_normal, neighbor_face_normal);
          if (edge_dot >= min_edge_dot_to_smooth)
            normal_sum += neighbor_face_normal;
        }
        neighbor_face_id = GetNextAdjacentFaceId(neighbor_corner_id);

      } while (neighbor_face_id != Mesh::InvalidId && neighbor_face_id != vertex_data.mFaceId);
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

void Mesh::ComputeOppositeCornerIds()
{
  // Create auxiliar map that maps each edge to its two adjacent faces
  std::unordered_map<Edge, std::pair<FaceId, FaceId>, Edge::Hash> edge_to_adjacent_faces;
  for (Mesh::FaceId face_id = 0; face_id < GetNumberOfFaces(); ++face_id)
  {
    const auto face_edges = GetFaceEdges(face_id);
    for (const auto& face_edge : face_edges)
    {
      assert(face_edge[0] < face_edge[1]);
      const auto it = edge_to_adjacent_faces.find(face_edge);
      if (it != edge_to_adjacent_faces.end())
      {
        auto& adjacent_faces_pair = it->second;
        assert(adjacent_faces_pair.first != Mesh::InvalidId);

        if (adjacent_faces_pair.second != Mesh::InvalidId)
          THROW_EXCEPTION("Found non-manifold edge " << face_edge << " with at least these faces ("
                                                     << adjacent_faces_pair.first << ", " << adjacent_faces_pair.second
                                                     << ", " << face_id << ").");
        adjacent_faces_pair.second = face_id;
      }
      else
      {
        auto& adjacent_faces = edge_to_adjacent_faces[face_edge];
        adjacent_faces.first = face_id;
        adjacent_faces.second = Mesh::InvalidId;
      }
    }
  }

  // Update corner opposites
  for (const auto& edge_to_adjacent_faces_kv_pair : edge_to_adjacent_faces)
  {
    const auto& edge = edge_to_adjacent_faces_kv_pair.first;
    const auto& adjacent_faces_pair = edge_to_adjacent_faces_kv_pair.second;
    const auto& first_adjacent_face = adjacent_faces_pair.first;
    const auto& second_adjacent_face = adjacent_faces_pair.second;
    assert(first_adjacent_face != Mesh::InvalidId && second_adjacent_face != Mesh::InvalidId);

    if (first_adjacent_face == Mesh::InvalidId || second_adjacent_face == Mesh::InvalidId)
      continue;

    const auto first_other_vertex_id = GetFaceOtherVertexId(first_adjacent_face, edge[0], edge[1]);
    const auto second_other_vertex_id = GetFaceOtherVertexId(second_adjacent_face, edge[0], edge[1]);
    assert(first_other_vertex_id < GetNumberOfVertices());
    assert(second_other_vertex_id < GetNumberOfVertices());
    const auto first_corner_id = GetCornerIdFromFaceIdAndVertexId(first_adjacent_face, first_other_vertex_id);
    const auto second_corner_id = GetCornerIdFromFaceIdAndVertexId(second_adjacent_face, second_other_vertex_id);
    assert(first_corner_id < GetNumberOfCorners());
    assert(second_corner_id < GetNumberOfCorners());
    assert(first_corner_id != second_corner_id);

    mCornersData.at(first_corner_id).mOppositeCornedId = second_corner_id;
    mCornersData.at(second_corner_id).mOppositeCornedId = first_corner_id;
  }

  mOppositeCornerIdsComputed = true;

  // Set all mFaceId in VertexData so that they point to the most CW face in the boundary (if boundary)
  // This is so that we can use the circulators using next->next->next...and make sure we traverse all faces
  for (const auto& edge_to_adjacent_faces_kv_pair : edge_to_adjacent_faces)
  {
    const auto& edge = edge_to_adjacent_faces_kv_pair.first;
    const auto& adjacent_faces_pair = edge_to_adjacent_faces_kv_pair.second;
    const auto& first_adjacent_face = adjacent_faces_pair.first;
    const auto& second_adjacent_face = adjacent_faces_pair.second;

    auto boundary_face_id = Mesh::InvalidId;
    if (first_adjacent_face == Mesh::InvalidId)
      boundary_face_id = second_adjacent_face;
    else if (second_adjacent_face == Mesh::InvalidId)
      boundary_face_id = first_adjacent_face;

    if (boundary_face_id == Mesh::InvalidId)
      continue;

    const auto boundary_face_vertices_id = GetFaceVerticesIds(boundary_face_id);
    for (const auto& boundary_face_vertex_id : { edge[0], edge[1] })
    {
      const auto boundary_face_corner_id = GetCornerIdFromFaceIdAndVertexId(boundary_face_id, boundary_face_vertex_id);
      if (GetPreviousAdjacentFaceId(boundary_face_corner_id) == Mesh::InvalidId)
      {
        mVerticesData.at(boundary_face_vertex_id).mFaceId = boundary_face_id;
        break;
      }
    }
  }
}

void Mesh::Read(const std::filesystem::path& inMeshPath)
{
  Assimp::Importer importer;
  const auto* scene_ptr
      = importer.ReadFile(inMeshPath.string(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

  if (!scene_ptr)
    THROW_EXCEPTION("Error loading mesh from '" << inMeshPath.string() << "': " << importer.GetErrorString());

  const auto& scene = *scene_ptr;

  if (scene.mNumMeshes == 0)
    THROW_EXCEPTION("Error loading mesh from '" << inMeshPath.string() << "': the file contains no mesh.");

  const auto AiVector3DToVec3f
      = [](const aiVector3D& inAiVector3D) { return Vec3f(inAiVector3D.x, inAiVector3D.y, inAiVector3D.z); };

  const auto& ai_mesh = *(scene.mMeshes[0]);
  Clear();

  // Create vertices
  for (Mesh::VertexId vertex_id = 0; vertex_id < ai_mesh.mNumVertices; ++vertex_id)
  {
    const auto ai_vertex_position = ai_mesh.mVertices[vertex_id];
    const auto vertex_position = AiVector3DToVec3f(ai_vertex_position);
    AddVertex(vertex_position);
  }

  // Create faces
  for (Mesh::FaceId face_id = 0; face_id < ai_mesh.mNumFaces; ++face_id)
  {
    const auto ai_face = ai_mesh.mFaces[face_id];
    EXPECTS(ai_face.mNumIndices == 3);
    AddFace(ai_face.mIndices[0], ai_face.mIndices[1], ai_face.mIndices[2]);
  }

  // Assign corner properties
  for (Mesh::CornerId corner_id = 0; corner_id < GetNumberOfCorners(); ++corner_id)
  {
    const auto face_id = (corner_id / 3);
    const auto internal_corner_id = (corner_id % 3);
    const auto vertex_id = mFacesData.at(face_id).mVerticesIds.at(internal_corner_id);

    // Normal
    if (ai_mesh.mNormals != nullptr)
    {
      const auto ai_corner_normal = ai_mesh.mNormals[vertex_id];
      const auto corner_normal = NormalizedSafe(AiVector3DToVec3f(ai_corner_normal));
      SetCornerNormal(corner_id, corner_normal);
    }

    // Texture coordinate
    if (ai_mesh.mTextureCoords[0] != nullptr)
    {
      const auto ai_corner_texture_coordinates = ai_mesh.mTextureCoords[0][vertex_id];
      const auto corner_texture_coordinates = XY(AiVector3DToVec3f(ai_corner_texture_coordinates));
      SetCornerTextureCoordinates(corner_id, corner_texture_coordinates);
    }
  }

  ComputeOppositeCornerIds();
}

void Mesh::Write(const std::filesystem::path& inMeshPath) const
{
  EXPECTS(!inMeshPath.extension().string().empty());

  constexpr auto preserve_vertices_ids = true;

  aiScene ai_scene;

  ai_scene.mNumMaterials = 1;
  ai_scene.mMaterials = new aiMaterial*[ai_scene.mNumMaterials];
  ai_scene.mMaterials[0] = new aiMaterial();

  ai_scene.mNumMeshes = 1;
  ai_scene.mMeshes = new aiMesh*[ai_scene.mNumMeshes];
  ai_scene.mMeshes[0] = new aiMesh();
  auto& ai_mesh = *ai_scene.mMeshes[0];
  {
    const auto Vec3fToAiVector3D
        = [](const Vec3f& inVector) { return aiVector3D(inVector[0], inVector[1], inVector[2]); };
    const auto Vec2fToAiVector3D = [](const Vec2f& inVector) { return aiVector3D(inVector[0], inVector[1], 0); };

    ai_mesh.mMaterialIndex = 0;

    ai_mesh.mNumFaces = GetNumberOfFaces();
    ai_mesh.mFaces = new aiFace[ai_mesh.mNumFaces];
    for (Mesh::FaceId face_id = 0; face_id < GetNumberOfFaces(); ++face_id)
    {
      aiFace& ai_face = ai_mesh.mFaces[face_id];
      ai_face.mNumIndices = 3;
      ai_face.mIndices = new unsigned int[ai_face.mNumIndices];

      const auto& face_data = mFacesData.at(face_id);
      if (preserve_vertices_ids)
      {
        ai_face.mIndices[0] = face_data.mVerticesIds[0];
        ai_face.mIndices[1] = face_data.mVerticesIds[1];
        ai_face.mIndices[2] = face_data.mVerticesIds[2];
      }
      else
      {
        ai_face.mIndices[0] = face_id * 3 + 0;
        ai_face.mIndices[1] = face_id * 3 + 1;
        ai_face.mIndices[2] = face_id * 3 + 2;
      }
    }

    if (preserve_vertices_ids)
    {
      ai_mesh.mNumVertices = GetNumberOfVertices();
      ai_mesh.mVertices = new aiVector3D[GetNumberOfVertices()];
      for (Mesh::VertexId vertex_id = 0; vertex_id < GetNumberOfVertices(); ++vertex_id)
      {
        ai_mesh.mVertices[vertex_id] = Vec3fToAiVector3D(GetVerticesData().at(vertex_id).mPosition);
      }
    }
    else
    {
      ai_mesh.mNumVertices = GetNumberOfCorners();
      ai_mesh.mVertices = new aiVector3D[GetNumberOfCorners()];

      ai_mesh.mNumUVComponents[0] = 2;
      ai_mesh.mTextureCoords[0] = new aiVector3D[GetNumberOfCorners()];
      ai_mesh.mNormals = new aiVector3D[GetNumberOfCorners()];

      for (Mesh::CornerId corner_id = 0; corner_id < GetNumberOfCorners(); ++corner_id)
      {
        const auto vertex_id = GetVertexIdFromCornerId(corner_id);
        ai_mesh.mVertices[corner_id] = Vec3fToAiVector3D(GetVerticesData().at(vertex_id).mPosition);
        ai_mesh.mNormals[corner_id] = Vec3fToAiVector3D(GetCornersData().at(corner_id).mNormal);
        ai_mesh.mTextureCoords[0][corner_id] = Vec2fToAiVector3D(GetCornersData().at(corner_id).mTextureCoordinates);
      }
    }
  }

  ai_scene.mRootNode = new aiNode();
  {
    ai_scene.mRootNode->mNumMeshes = 1;
    ai_scene.mRootNode->mMeshes = new unsigned int[ai_scene.mRootNode->mNumMeshes];
    ai_scene.mRootNode->mMeshes[0] = 0;
  }

  Assimp::Exporter exporter;

  const auto get_format_id_from_extension = [&exporter](const std::string_view extension) {
    std::string lower_case_extension;
    std::transform(extension.cbegin(), extension.cend(), std::back_inserter(lower_case_extension), ::tolower);

    for (std::size_t i = 0; i < exporter.GetExportFormatCount(); ++i)
    {
      const auto format_description = exporter.GetExportFormatDescription(i);
      if (lower_case_extension == format_description->fileExtension)
        return format_description->id;
    }

    THROW_EXCEPTION("Can't export mesh with file extension '" << lower_case_extension << "'");
  };

  const auto extension = inMeshPath.extension().string().substr(1); // Get extension without "."
  const auto format_id = get_format_id_from_extension(extension);

  const auto export_result = exporter.Export(&ai_scene, format_id, inMeshPath.string());
  if (export_result != aiReturn_SUCCESS)
    THROW_EXCEPTION("Error when exporting mesh to " << inMeshPath << ": " << exporter.GetErrorString());
}

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