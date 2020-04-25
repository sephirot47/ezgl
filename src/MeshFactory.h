#pragma once

#include "Mesh.h"

namespace egl
{
class MeshFactory
{
public:
  MeshFactory() = delete;

  // 3D
  static Mesh GetCube();
  static Mesh GetSphere(const std::size_t inNumLatitudes, const std::size_t inNumLongitudes);
  static Mesh GetHemisphere(const std::size_t inNumLatitudes, const std::size_t inNumLongitudes);
  static Mesh GetCone(const std::size_t inNumLatitudes);
  static Mesh GetCylinder(const std::size_t inNumLatitudes);
  static Mesh
  GetTorus(const std::size_t inNumLatitudes, const std::size_t inNumLongitudes, const float inHoleSize = 0.5f);

  // 2D (z = 0.0)
  static Mesh GetPlane(const std::size_t inNumVerticesX = 2, const std::size_t inNumVerticesY = 2);
  static Mesh GetCircleSection(const std::size_t inNumVertices, const float inSectionAngleRads);
  static Mesh GetCircle(const std::size_t inNumVertices);

private:
  static Mesh GetSphere(const std::size_t inNumLatitudes, const std::size_t inNumLongitudes, const bool inIsHemisphere);
  static void ConsolidateMesh(Mesh& ioMesh, const bool inUpdateNormals = true);
};

}