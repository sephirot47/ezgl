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

  cube.AddFace(0, 4, 1);
  cube.AddFace(1, 4, 5);
  cube.AddFace(1, 5, 3);
  cube.AddFace(3, 5, 7);
  cube.AddFace(1, 3, 2);
  cube.AddFace(0, 1, 2);
  cube.AddFace(3, 7, 2);
  cube.AddFace(7, 6, 2);
  cube.AddFace(4, 6, 5);
  cube.AddFace(5, 6, 7);
  cube.AddFace(0, 6, 4);
  cube.AddFace(0, 2, 6);

  ConsolidateMesh(cube, DrawableMesh::ENormalsType::FLAT);

  return cube;
}

template <typename TMesh>
void GMeshFactory<TMesh>::ConsolidateMesh(TMesh& ioMesh, const DrawableMesh::ENormalsType &inNormalsType)
{
  if constexpr (std::is_same_v<TMesh, DrawableMesh>)
  {
    ioMesh.UpdateVAOs(inNormalsType);
  }
}
}