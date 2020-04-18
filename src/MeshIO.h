#pragma once

#include <filesystem>

namespace egl
{
class Mesh;

class MeshIO
{
public:
  static void Read(const std::filesystem::path& inMeshPath, Mesh& ioMesh);
  static void Write(const Mesh& inMesh, const std::filesystem::path& inMeshPath, const bool inPreserveVerticesIds);

  MeshIO() = delete;
};
}