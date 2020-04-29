#pragma once

#include "ezgl/Macros.h"
#include "ezgl/Mat.h"
#include "ezgl/Vec.h"
#include "ezgl/MathInitializers.h"
#include "ezgl/MeshIterators.h"
#include "ezgl/Range.h"
#include "ezgl/Vec.h"
#include <algorithm>
#include <array>
#include <filesystem>
#include <optional>
#include <utility>
#include <vector>

namespace egl
{
class Mesh final
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
    bool operator<(const Edge& inRHS) const { return (*this)[0] < inRHS[0] || ((*this)[0] == inRHS[0] && (*this)[1] < inRHS[1]);  }
    const Mesh::VertexId& operator[](std::size_t inVertexInternalIdFrom0To1) const
    {
      EXPECTS(inVertexInternalIdFrom0To1 == 0 || inVertexInternalIdFrom0To1 == 1);
      return inVertexInternalIdFrom0To1 == 0 ? mVerticesIds.first : mVerticesIds.second;
    }

    struct Hash
    {
      std::size_t operator()(const Edge& inEdge) const
      {
        return std::hash<Mesh::VertexId>()(inEdge[0]) ^ std::hash<Mesh::VertexId>()(inEdge[1]);
      }
    };

  private:
    const std::pair<Mesh::VertexId, Mesh::VertexId> mVerticesIds = std::make_pair(Mesh::InvalidId, Mesh::InvalidId);
  };

  struct VertexData
  {
    Mesh::FaceId mFaceId = Mesh::InvalidId;
    Vec3f mPosition = Zero<Vec3f>();
  };

  struct CornerData
  {
    Mesh::CornerId mOppositeCornedId = Mesh::InvalidId;
    Vec3f mNormal = Zero<Vec3f>();
    Vec2f mTextureCoordinates = Zero<Vec2f>();
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
  void ComputeOppositeCornerIds();
  void Clear();

  void SetVertexPosition(const Mesh::VertexId inVertexId, const Vec3f& inPosition);
  void SetFaceNormal(const Mesh::FaceId inFaceId, const Vec3f& inFaceNormal);
  void SetCornerNormal(const Mesh::CornerId inCornerId, const Vec3f& inCornerNormal);
  void SetCornerTextureCoordinates(const Mesh::CornerId& inCornerId, const Vec2f& inTextureCoordinates);
  const Vec3f& GetVertexPosition(const Mesh::VertexId& inVertexId) const;
  const Vec3f& GetFaceNormal(const Mesh::CornerId& inCornerId) const;
  const Vec3f& GetCornerNormal(const Mesh::FaceId& inFaceId) const;
  const Vec2f& GetCornerTextureCoordinates(const Mesh::CornerId& inCornerId) const;
  Mesh::VertexId GetVertexIdFromCornerId(const Mesh::CornerId inCornerId) const;
  Mesh::VertexId GetVertexIdFromFaceIdAndInternalCornerId(const Mesh::FaceId inFaceId,
      const Mesh::InternalCornerId inInternalCornerId) const;
  Mesh::FaceId GetFaceIdFromCornerId(const Mesh::CornerId inCornerId) const;
  Mesh::CornerId GetOppositeCornerId(const Mesh::CornerId inCornerId) const;
  Mesh::FaceId GetOppositeFaceId(const Mesh::CornerId inCornerId) const;
  std::array<Mesh::Edge, 3> GetFaceEdges(const Mesh::FaceId inFaceId) const;
  std::array<Mesh::CornerId, 3> GetFaceCornersIds(const Mesh::FaceId inFaceId) const;
  std::array<Mesh::VertexId, 3> GetFaceVerticesIds(const Mesh::FaceId inFaceId) const;
  std::array<Mesh::CornerId, 2> GetFaceOtherCornerIds(const Mesh::FaceId inFaceId,
      const Mesh::CornerId inCornerId) const;
  std::array<Mesh::VertexId, 2> GetFaceOtherVertexIds(const Mesh::FaceId inFaceId,
      const Mesh::VertexId inVertexId) const;
  Mesh::CornerId GetFaceOtherCornerId(const Mesh::FaceId inFaceId,
      const Mesh::CornerId inCornerId0,
      const Mesh::CornerId inCornerId1) const;
  Mesh::VertexId GetFaceOtherVertexId(const Mesh::FaceId inFaceId,
      const Mesh::VertexId inVertexId0,
      const Mesh::VertexId inVertexId1) const;
  Mesh::CornerId GetCornerIdFromFaceIdAndVertexId(const Mesh::FaceId inFaceId, const Mesh::VertexId inVertexId) const;
  Mesh::CornerId GetPreviousCornerId(const Mesh::CornerId inCornerId) const;
  Mesh::CornerId GetNextCornerId(const Mesh::CornerId inCornerId) const;
  Mesh::CornerId GetNextAdjacentCornerId(const Mesh::CornerId inCornerId) const;
  Mesh::CornerId GetPreviousAdjacentCornerId(const Mesh::CornerId inCornerId) const;
  Mesh::CornerId GetNextAdjacentFaceId(const Mesh::CornerId inCornerId) const;
  Mesh::CornerId GetPreviousAdjacentFaceId(const Mesh::CornerId inCornerId) const;

  void Transform(const Mat4f& inTransform);

  static bool IsValid(const Mesh::Id inId);
  static bool IsValid(const std::optional<Mesh::Id> inOptionalId);
  const std::vector<Mesh::VertexData>& GetVerticesData() const;
  const std::vector<Mesh::CornerData>& GetCornersData() const;
  const std::vector<Mesh::FaceData>& GetFacesData() const;
  std::size_t GetNumberOfFaces() const;
  std::size_t GetNumberOfVertices() const;
  std::size_t GetNumberOfCorners() const;

  // Circulators
  // CirculatorVertexNeighborFaceIds GetVertexNeighborFaceIdsCirculatorBegin(const Mesh::VertexId inVertexId) const;
  // CirculatorVertexNeighborFaceIds GetVertexNeighborFaceIdsCirculatorEnd() const;
  // Range<CirculatorVertexNeighborFaceIds> AllVertexNeighborFaceIds(const Mesh::VertexId inVertexId) const;

  std::vector<Mesh::CornerId> GetNeighborCornersIds(const Mesh::VertexId inVertexId) const;
  std::vector<Mesh::VertexId> GetNeighborVerticesIds(const Mesh::VertexId inVertexId) const;
  std::vector<Mesh::FaceId> GetNeighborFacesIds(const Mesh::VertexId inVertexId) const;
  std::vector<Mesh::CornerId> GetVertexCornersIds(const Mesh::VertexId inVertexId) const;

  virtual void Read(const std::filesystem::path& inMeshPath);
  void Write(const std::filesystem::path& inMeshPath, const bool inPreserveVerticesIds = false) const;

private:
  bool mOppositeCornerIdsComputed = false;
  std::vector<Mesh::VertexData> mVerticesData;
  std::vector<Mesh::CornerData> mCornersData;
  std::vector<Mesh::FaceData> mFacesData;
};

std::ostream& operator<<(std::ostream& ioLHS, const Mesh::Edge& inRHS);
std::ostream& operator<<(std::ostream& ioLHS, const Mesh::VertexData& inRHS);
}