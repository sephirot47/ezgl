#include "Mesh.h"
#include "Macros.h"
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
}

void Mesh::AddFace(const Mesh::VertexId& inFaceVertexId0,
    const Mesh::VertexId& inFaceVertexId1,
    const Mesh::VertexId& inFaceVertexId2)
{
  // Vertices must have been added before
  if (inFaceVertexId0 >= mVerticesData.size())
    THROW_EXCEPTION("Trying to add a face with non-existent vertex id " << inFaceVertexId0);
  if (inFaceVertexId1 >= mVerticesData.size())
    THROW_EXCEPTION("Trying to add a face with non-existent vertex id " << inFaceVertexId1);
  if (inFaceVertexId2 >= mVerticesData.size())
    THROW_EXCEPTION("Trying to add a face with non-existent vertex id " << inFaceVertexId2);

  // Add face
  mFacesData.emplace_back();
  const auto new_face_id = mFacesData.size() - 1;
  const std::array new_face_vertices_ids = { inFaceVertexId0, inFaceVertexId1, inFaceVertexId2 };
  mFacesData.at(new_face_id).mVerticesIds = new_face_vertices_ids;

  // For each vertex of the new face, update neighbor faces
  for (Mesh::InternalCornerId internal_corner_id = 0; internal_corner_id < 3; ++internal_corner_id)
  {
    const auto& new_face_vertex_id = new_face_vertices_ids[internal_corner_id];
    std::vector<Mesh::FaceId> new_vertex_ordered_neighbor_faces_ids; // Find the list of neighbor faces ordered in CCW
    {
      // First just build the list of neighbor faces but disordered
      auto vertex_neighbor_faces_ids_disordered = std::move(mVerticesData.at(new_face_vertex_id).mNeighborFacesId);
      vertex_neighbor_faces_ids_disordered.push_back(new_face_id);

      // Now build a map that relates every right vertex to left vertex of each neighbor face
      std::unordered_map<Mesh::VertexId, std::pair<Mesh::VertexId, Mesh::FaceId>>
          right_vertex_to_left_vertex_and_face_ids;
      for (const auto& vertex_neighbor_face_id : vertex_neighbor_faces_ids_disordered)
      {
        // Left vertex means the following vertex in CCW direction after the Right vertex.
        const auto& vertex_neighbor_face_vertex_id_0 = mFacesData.at(vertex_neighbor_face_id).mVerticesIds[0];
        const auto& vertex_neighbor_face_vertex_id_1 = mFacesData.at(vertex_neighbor_face_id).mVerticesIds[1];
        const auto& vertex_neighbor_face_vertex_id_2 = mFacesData.at(vertex_neighbor_face_id).mVerticesIds[2];
        auto right_and_left_vertex_neighbor_face_vertex_ids = std::make_pair(Mesh::InvalidId, Mesh::InvalidId);
        if (new_face_vertex_id == vertex_neighbor_face_vertex_id_0)
          right_and_left_vertex_neighbor_face_vertex_ids
              = std::make_pair(vertex_neighbor_face_vertex_id_1, vertex_neighbor_face_vertex_id_2);
        else if (new_face_vertex_id == vertex_neighbor_face_vertex_id_1)
          right_and_left_vertex_neighbor_face_vertex_ids
              = std::make_pair(vertex_neighbor_face_vertex_id_2, vertex_neighbor_face_vertex_id_0);
        else if (new_face_vertex_id == vertex_neighbor_face_vertex_id_2)
          right_and_left_vertex_neighbor_face_vertex_ids
              = std::make_pair(vertex_neighbor_face_vertex_id_0, vertex_neighbor_face_vertex_id_1);
        else
          THROW_EXCEPTION("Found vertex neighbors inconsistencies");

        const auto& right_vertex_id = right_and_left_vertex_neighbor_face_vertex_ids.first;
        const auto& left_vertex_id = right_and_left_vertex_neighbor_face_vertex_ids.second;
        if (right_vertex_to_left_vertex_and_face_ids.count(right_vertex_id) != 0)
          THROW_EXCEPTION("Non-two manifold mesh detected because a T-junction was found at edge "
              << right_and_left_vertex_neighbor_face_vertex_ids << " when adding face with id " << new_face_id
              << " with vertices ids " << new_face_vertices_ids);

        right_vertex_to_left_vertex_and_face_ids[right_vertex_id] = { left_vertex_id, vertex_neighbor_face_id };
      }

      // Then find the sequence of faces such that the vertices follow a sequence
      new_vertex_ordered_neighbor_faces_ids.reserve(vertex_neighbor_faces_ids_disordered.size());
      auto right_vertex_to_left_vertex_and_face_id_it = right_vertex_to_left_vertex_and_face_ids.cbegin();
      while (right_vertex_to_left_vertex_and_face_id_it != right_vertex_to_left_vertex_and_face_ids.cend())
      {
        const auto right_vertex_to_left_vertex_and_face_id
            = *(right_vertex_to_left_vertex_and_face_id_it); // Copy bc we erase it later
        const auto& right_vertex_id = right_vertex_to_left_vertex_and_face_id.first;
        const auto& left_vertex_id = right_vertex_to_left_vertex_and_face_id.second.first;
        const auto& face_id = right_vertex_to_left_vertex_and_face_id.second.second;

        // Push the ordered neighbor face, and remove it from our map so that this right-left vertices of the face are
        // no longer processed
        new_vertex_ordered_neighbor_faces_ids.push_back(face_id);
        right_vertex_to_left_vertex_and_face_ids.erase(right_vertex_id);

        // Find next face whose left vertex id is this face right vertex id
        const auto following_right_vertex_id = left_vertex_id;
        right_vertex_to_left_vertex_and_face_id_it
            = right_vertex_to_left_vertex_and_face_ids.find(following_right_vertex_id);
        if (right_vertex_to_left_vertex_and_face_id_it == right_vertex_to_left_vertex_and_face_ids.cend())
        {
          // We have finished a chunk of triangles (we have reached a boundary vertex. Start from the beginning with one
          // of the remaining chunks (if any)
          right_vertex_to_left_vertex_and_face_id_it = right_vertex_to_left_vertex_and_face_ids.cbegin();
          continue;
        }
      }

      // Update the neighbor faces of our vertex with the CCW ordered list of neighbor faces
      mVerticesData.at(new_face_vertex_id).mNeighborFacesId = std::move(new_vertex_ordered_neighbor_faces_ids);
    }
  }
}

Vec3f Mesh::ComputeFaceNormal(const Mesh::FaceId& inFaceId)
{
  const auto& face_data = mFacesData.at(inFaceId);
  const auto vertex_position_0 = mVerticesData.at(face_data.mVerticesIds[0]).mPosition;
  const auto vertex_position_1 = mVerticesData.at(face_data.mVerticesIds[1]).mPosition;
  const auto vertex_position_2 = mVerticesData.at(face_data.mVerticesIds[2]).mPosition;
  const auto v1_v0 = (vertex_position_0 - vertex_position_1);
  const auto v1_v2 = (vertex_position_2 - vertex_position_1);
  const auto face_normal = NormalizedSafe(Cross(v1_v2, v1_v0));
  return face_normal;
}

Vec3f Mesh::ComputeVertexSmoothNormal(const Mesh::VertexId& inVertexId)
{
  auto normal_sum = Zero<Vec3f>();
  {
    const auto& vertex_data = mVerticesData.at(inVertexId);
    for (const auto& neighbor_face_id : vertex_data.mNeighborFacesId)
    {
      const auto neighbor_face_normal = ComputeFaceNormal(neighbor_face_id);
      assert(IsNormalized(neighbor_face_normal));
      normal_sum += ComputeFaceNormal(neighbor_face_id);
    }
  }

  const auto vertex_smooth_normal = NormalizedSafe(normal_sum);
  return vertex_smooth_normal;
}

void Mesh::Clear()
{
  mVerticesData.clear();
  mFacesData.clear();
}

void Mesh::SetVertexPosition(const Mesh::VertexId inVertexId, const Vec3f& inPosition)
{
  mVerticesData.at(inVertexId).mPosition = inPosition;
}

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

bool Mesh::IsValid(const Mesh::Id inId) { return inId != Mesh::InvalidId; }

bool Mesh::IsValid(const std::optional<Mesh::Id> inOptionalId)
{
  return inOptionalId.has_value() ? Mesh::IsValid(*inOptionalId) : false;
}

const std::vector<Mesh::VertexData>& Mesh::GetVerticesData() const { return mVerticesData; }

const std::vector<Mesh::FaceData>& Mesh::GetFacesData() const { return mFacesData; }

std::size_t Mesh::GetNumberOfFaces() const { return mFacesData.size(); }

std::size_t Mesh::GetNumberOfVertices() const { return mVerticesData.size(); }

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

  const auto& aiMesh = *(scene.mMeshes[0]);
  Clear();

  // Create vertices
  for (std::size_t vertex_id = 0; vertex_id < aiMesh.mNumVertices; ++vertex_id)
  {
    const auto ai_vertex_position = aiMesh.mVertices[vertex_id];
    const auto vertex_position = AiVector3DToVec3f(ai_vertex_position);
    AddVertex(vertex_position);
  }

  // Create faces
  for (std::size_t face_id = 0; face_id < aiMesh.mNumFaces; ++face_id)
  {
    const auto ai_face = aiMesh.mFaces[face_id];
    EXPECTS(ai_face.mNumIndices == 3);
    AddFace(ai_face.mIndices[0], ai_face.mIndices[1], ai_face.mIndices[2]);
  }
}

void Mesh::Write(const std::filesystem::path& inMeshPath) const
{
  EXPECTS(!inMeshPath.extension().string().empty());

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

    ai_mesh.mMaterialIndex = 0;

    ai_mesh.mNumVertices = GetNumberOfVertices();
    ai_mesh.mVertices = new aiVector3D[ai_mesh.mNumVertices];
    for (std::size_t vertex_id = 0; vertex_id < GetNumberOfVertices(); ++vertex_id)
    {
      ai_mesh.mVertices[vertex_id] = Vec3fToAiVector3D(GetVerticesData().at(vertex_id).mPosition);
    }

    ai_mesh.mNumFaces = GetNumberOfFaces();
    ai_mesh.mFaces = new aiFace[ai_mesh.mNumFaces];
    for (std::size_t face_id = 0; face_id < GetNumberOfFaces(); ++face_id)
    {
      aiFace& ai_face = ai_mesh.mFaces[face_id];
      ai_face.mNumIndices = 3;
      ai_face.mIndices = new unsigned int[ai_face.mNumIndices];

      const auto& face_data = GetFacesData()[face_id];
      ai_face.mIndices[0] = face_data.mVerticesIds[0];
      ai_face.mIndices[1] = face_data.mVerticesIds[1];
      ai_face.mIndices[2] = face_data.mVerticesIds[2];
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
  ioLHS << "VertexData<NeighborFacesIds: " << inRHS.mNeighborFacesId << ", Position: " << inRHS.mPosition << ">";
  return ioLHS;
}
}