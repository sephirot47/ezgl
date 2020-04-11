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

private:
  static  void ConsolidateMesh(TMesh& ioMesh, const DrawableMesh::ENormalsType &inNormalsType);
};

using MeshFactory = GMeshFactory<Mesh>;
using DrawableMeshFactory = GMeshFactory<DrawableMesh>;

}

#include "MeshFactory.tcc"