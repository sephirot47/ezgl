#pragma once

#ifdef MESH_IO
#include <filesystem>

namespace ez
{
class Mesh;

class MeshIO final
{
public:
  static void Read(const std::filesystem::path& inMeshPath, Mesh& ioMesh);
  static void Write(const Mesh& inMesh, const std::filesystem::path& inMeshPath, const bool inPreserveVerticesIds);

  MeshIO() = delete;
};
}
#endif