#pragma once

#include "Macros.h"
#include "Math.h"
#include "MeshIterators.h"
#include "Range.h"
#include "Vec.h"
#include <algorithm>
#include <array>
#include <filesystem>
#include <optional>
#include <utility>
#include <vector>

namespace egl
{
class Mesh
{
public:
  using Id = uint32_t;
  using CornerId = Mesh::Id;
  using FaceId = Mesh::Id;
  using VertexId = Mesh::Id;
  using InternalCornerId = uint8_t; // [0, 2];
  using FaceVerticesIds = Vec3<Mesh::FaceId>;
  static constexpr VertexId InvalidId = static_cast<VertexId>(-1);

  using CirculatorVertexNeighborFaceIds = MeshCirculatorVertexNeighborFaceIds<Mesh>;

  struct Edge
  {
    Edge(const Mesh::VertexId& inVertexId0, const Mesh::VertexId& inVertexId1)
        : mVerticesIds(std::minmax(inVertexId0, inVertexId1))
    {
    }

    bool operator==(const Edge& inRHS) const { return (((*this)[0] == inRHS[0]) && ((*this)[1] == inRHS[1])); }
    bool operator!=(const Edge& inRHS) const { return !(*this == inRHS); }
    const Mesh::VertexId& operator[](std::size_t inVertexInternalIdFrom0To1) const
    {
      EXPECTS(inVertexInternalIdFrom0To1 == 0 || inVertexInternalIdFrom0To1 == 1);
      return inVertexInternalIdFrom0To1 == 0 ? mVerticesIds.first : mVerticesIds.second;
    }

  private:
    const std::pair<Mesh::VertexId, Mesh::VertexId> mVerticesIds = std::make_pair(Mesh::InvalidId, Mesh::InvalidId);
  };

  struct VertexData
  {
    std::vector<Mesh::FaceId> mNeighborFacesId;
    Vec3f mPosition = Zero<Vec3f>();
  };

  struct CornerData
  {
    Vec3f mNormal = Zero<Vec3f>();
  };

  struct FaceData
  {
    std::array<Mesh::VertexId, 3> mVerticesIds;
    Vec3f mNormal = Zero<Vec3f>();
  };

  Mesh() = default;
  Mesh(const Mesh& inRHS) = default;
  Mesh& operator=(const Mesh& inRHS) = default;
  Mesh(Mesh&& inRHS) = default;
  Mesh& operator=(Mesh&& inRHS) = default;
  virtual ~Mesh() = default;

  void AddVertex(const Vec3f& inPosition);
  void AddFace(const Mesh::VertexId& inFaceVertexId0,
      const Mesh::VertexId& inFaceVertexId1,
      const Mesh::VertexId& inFaceVertexId2);
  void ComputeFaceNormals();
  void ComputeCornerNormals(const float inMinEdgeAngleToSmooth);
  void ComputeNormals(const float inMinEdgeAngleToSmooth);
  void Clear();

  void SetFaceNormal(const Mesh::FaceId inFaceId, const Vec3f &inFaceNormal);
  void SetCornerNormal(const Mesh::CornerId inCornerId, const Vec3f &inCornerNormal);
  const Vec3f& GetFaceNormal(const Mesh::CornerId &inCornerId);
  const Vec3f& GetCornerNormal(const Mesh::FaceId &inFaceId);

  void SetVertexPosition(const Mesh::VertexId inVertexId, const Vec3f& inPosition);

  static bool IsValid(const Mesh::Id inId);
  static bool IsValid(const std::optional<Mesh::Id> inOptionalId);
  const std::vector<Mesh::VertexData>& GetVerticesData() const;
  const std::vector<Mesh::CornerData>& GetCornersData() const;
  const std::vector<Mesh::FaceData>& GetFacesData() const;
  std::size_t GetNumberOfFaces() const;
  std::size_t GetNumberOfVertices() const;
  std::size_t GetNumberOfCorners() const;

  // Circulators
  CirculatorVertexNeighborFaceIds GetVertexNeighborFaceIdsCirculatorBegin(const Mesh::VertexId inVertexId) const;
  CirculatorVertexNeighborFaceIds GetVertexNeighborFaceIdsCirculatorEnd() const;
  Range<CirculatorVertexNeighborFaceIds> AllVertexNeighborFaceIds(const Mesh::VertexId inVertexId) const;

  virtual void Read(const std::filesystem::path& inMeshPath);
  void Write(const std::filesystem::path& inMeshPath) const;

private:
  std::vector<Mesh::VertexData> mVerticesData;
  std::vector<Mesh::CornerData> mCornersData;
  std::vector<Mesh::FaceData> mFacesData;
};

std::ostream& operator<<(std::ostream& ioLHS, const Mesh::Edge& inRHS);
std::ostream& operator<<(std::ostream& ioLHS, const Mesh::VertexData& inRHS);
}