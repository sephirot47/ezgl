#pragma once

#include <optional>

namespace ez
{
template <typename TMesh>
class MeshCirculatorVertexNeighborFaceIds final
{
public:
  using MeshId = typename TMesh::Id;

  MeshCirculatorVertexNeighborFaceIds(const TMesh& inMesh, const MeshId inVertexId)
      : mMesh(inMesh), mVertexId(inVertexId)
  {
  }
  MeshId operator*() const
  {
    EXPECTS(IsValid());
    return *Get();
  }
  void operator++()
  {
    EXPECTS(IsValid());
    ++mCurrentNeighborFaceId;
  }
  void operator--()
  {
    EXPECTS(IsValid());
    --mCurrentNeighborFaceId;
  }
  bool operator==(const MeshCirculatorVertexNeighborFaceIds& inRHS) const { return (Get() == inRHS.Get()); }
  bool operator!=(const MeshCirculatorVertexNeighborFaceIds& inRHS) const { return !(*this == inRHS); }
  bool IsValid() const { return TMesh::IsValid(Get()); }

private:
  const TMesh& mMesh;
  const MeshId mVertexId = TMesh::InvalidId;
  MeshId mCurrentNeighborFaceId = 0;

  std::optional<MeshId> Get() const
  {
    if (!TMesh::IsValid(mVertexId))
      return std::nullopt;

    const auto& vertex_neighbor_faces_ids = mMesh.GetVerticesData().at(mVertexId).mNeighborFacesId;
    if (mCurrentNeighborFaceId >= vertex_neighbor_faces_ids.size())
      return std::nullopt;
    return vertex_neighbor_faces_ids.at(mCurrentNeighborFaceId);
  }
};

}