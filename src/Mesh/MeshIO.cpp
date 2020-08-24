#ifdef MESH_IO

#include <ez/MeshIO.h>
#include <ez/Macros.h>
#include <ez/Math.h>
#include <ez/Mesh.h>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace ez
{
void MeshIO::Read(const std::filesystem::path& inMeshPath, Mesh& ioMesh)
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
  ioMesh.Clear();

  // Create vertices
  for (Mesh::VertexId vertex_id = 0; vertex_id < ai_mesh.mNumVertices; ++vertex_id)
  {
    const auto ai_vertex_position = ai_mesh.mVertices[vertex_id];
    const auto vertex_position = AiVector3DToVec3f(ai_vertex_position);
    ioMesh.AddVertex(vertex_position);
  }

  // Create faces
  for (Mesh::FaceId face_id = 0; face_id < ai_mesh.mNumFaces; ++face_id)
  {
    const auto ai_face = ai_mesh.mFaces[face_id];
    EXPECTS(ai_face.mNumIndices == 3);
    ioMesh.AddFace(ai_face.mIndices[0], ai_face.mIndices[1], ai_face.mIndices[2]);
  }

  // Assign corner properties
  for (Mesh::CornerId corner_id = 0; corner_id < ioMesh.GetNumberOfCorners(); ++corner_id)
  {
    const auto face_id = (corner_id / 3);
    const auto internal_corner_id = (corner_id % 3);
    const auto vertex_id = ioMesh.GetFacesData().at(face_id).mVerticesIds.at(internal_corner_id);

    // Normal
    if (ai_mesh.mNormals != nullptr)
    {
      const auto ai_corner_normal = ai_mesh.mNormals[vertex_id];
      const auto corner_normal = NormalizedSafe(AiVector3DToVec3f(ai_corner_normal));
      ioMesh.SetCornerNormal(corner_id, corner_normal);
    }

    // Texture coordinate
    if (ai_mesh.mTextureCoords[0] != nullptr)
    {
      const auto ai_corner_texture_coordinates = ai_mesh.mTextureCoords[0][vertex_id];
      const auto corner_texture_coordinates = XY(AiVector3DToVec3f(ai_corner_texture_coordinates));
      ioMesh.SetCornerTextureCoordinates(corner_id, corner_texture_coordinates);
    }
  }
}

void MeshIO::Write(const Mesh& inMesh, const std::filesystem::path& inMeshPath, const bool inPreserveVerticesIds)
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
    const auto Vec2fToAiVector3D = [](const Vec2f& inVector) { return aiVector3D(inVector[0], inVector[1], 0); };

    ai_mesh.mMaterialIndex = 0;

    ai_mesh.mNumFaces = inMesh.GetNumberOfFaces();
    ai_mesh.mFaces = new aiFace[ai_mesh.mNumFaces];
    for (Mesh::FaceId face_id = 0; face_id < inMesh.GetNumberOfFaces(); ++face_id)
    {
      aiFace& ai_face = ai_mesh.mFaces[face_id];
      ai_face.mNumIndices = 3;
      ai_face.mIndices = new unsigned int[ai_face.mNumIndices];

      const auto& face_data = inMesh.GetFacesData().at(face_id);
      if (inPreserveVerticesIds)
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

    if (inPreserveVerticesIds)
    {
      ai_mesh.mNumVertices = inMesh.GetNumberOfVertices();
      ai_mesh.mVertices = new aiVector3D[inMesh.GetNumberOfVertices()];
      for (Mesh::VertexId vertex_id = 0; vertex_id < inMesh.GetNumberOfVertices(); ++vertex_id)
      { ai_mesh.mVertices[vertex_id] = Vec3fToAiVector3D(inMesh.GetVerticesData().at(vertex_id).mPosition); }
    }
    else
    {
      ai_mesh.mNumVertices = inMesh.GetNumberOfCorners();
      ai_mesh.mVertices = new aiVector3D[inMesh.GetNumberOfCorners()];

      ai_mesh.mNumUVComponents[0] = 2;
      ai_mesh.mTextureCoords[0] = new aiVector3D[inMesh.GetNumberOfCorners()];
      ai_mesh.mNormals = new aiVector3D[inMesh.GetNumberOfCorners()];

      for (Mesh::CornerId corner_id = 0; corner_id < inMesh.GetNumberOfCorners(); ++corner_id)
      {
        const auto vertex_id = inMesh.GetVertexIdFromCornerId(corner_id);
        ai_mesh.mVertices[corner_id] = Vec3fToAiVector3D(inMesh.GetVerticesData().at(vertex_id).mPosition);
        ai_mesh.mNormals[corner_id] = Vec3fToAiVector3D(inMesh.GetCornersData().at(corner_id).mNormal);
        ai_mesh.mTextureCoords[0][corner_id]
            = Vec2fToAiVector3D(inMesh.GetCornersData().at(corner_id).mTextureCoordinates);
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
}

#endif