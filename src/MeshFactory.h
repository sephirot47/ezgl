#pragma once

#include "DrawableMesh.h"
#include "Mesh.h"

namespace egl
{
template <typename TMesh>
class GMeshFactory
{
public:
  GMeshFactory() = delete;

  static TMesh GetCube();
  static TMesh GetSphere(const std::size_t inNumLatitudes, const std::size_t inNumLongitudes);
  static TMesh GetHemisphere(const std::size_t inNumLatitudes, const std::size_t inNumLongitudes);
  static TMesh GetCone(const std::size_t inNumLatitudes);
  static TMesh GetCylinder(const std::size_t inNumLatitudes);
  static TMesh GetPlane(const std::size_t inNumVerticesX = 2, const std::size_t inNumVerticesY = 2);

private:
  static TMesh GetSphere(const std::size_t inNumLatitudes, const std::size_t inNumLongitudes, const bool inIsHemisphere);
  static  void ConsolidateMesh(TMesh& ioMesh, const bool inUpdateNormals = true);
};

using MeshFactory = GMeshFactory<Mesh>;
using DrawableMeshFactory = GMeshFactory<DrawableMesh>;

}

#include "MeshFactory.tcc"