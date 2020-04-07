#pragma once

#include <array>
#include <algorithm>
#include <optional>
#include <utility>
#include <vector>

#include "Math.h"
#include "MeshIterators.h"
#include "Vec.h"

namespace egl
{

class Mesh
{
public:
    using Id = std::size_t;
    using FaceId = Mesh::Id;
    using VertexId = Mesh::Id;
    using CornerId = Mesh::Id;
    using InternalCornerId = uint8_t; // [0, 2];
    using FaceVerticesIds = Vec3<Mesh::FaceId>;
    static constexpr VertexId InvalidId = static_cast<VertexId>(-1);

    using CirculatorVertexIncidentCornerIds = MeshCirculatorVertexIncidentCornerIds<Mesh>;

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
        Mesh::FaceId mIncidentFaceId = Mesh::InvalidId;
        Vec3f mPosition = Zero<Vec3f>();
        Vec3f mNormal = Zero<Vec3f>();
    };

    struct FaceData
    {
        Vec3f mNormal = Zero<Vec3f>();
    };

    struct CornerData
    {
        Mesh::VertexId mVertexId = Mesh::InvalidId; // Vertex id to which this corner is adjacent
        Mesh::FaceId mFaceId = Mesh::InvalidId; // Face id to which this corner belongs to
        Mesh::CornerId mOppositeCornerId = Mesh::InvalidId; // Opposite corner id (in another face)
    };

    Mesh() = default;
    Mesh(const Mesh& inRHS) = default;
    Mesh& operator=(const Mesh& inRHS) = default;
    Mesh(Mesh&& inRHS) = default;
    Mesh& operator=(Mesh&& inRHS) = default;
    ~Mesh() = default;

    void AddVertex(const Mesh::VertexData& inVertexData);
    void AddFace(const Mesh::VertexId& inFaceVertexId0, const Mesh::VertexId& inFaceVertexId1, const Mesh::VertexId& inFaceVertexId2);
    void ComputeNormals();

    void SetVertexPosition(const Mesh::VertexId inVertexId, const Vec3f& inPosition);

    std::array<Mesh::CornerId, 3> GetFaceCornerIds(const Mesh::FaceId inFaceId) const;
    std::array<Mesh::VertexId, 3> GetFaceVerticesIds(const Mesh::FaceId inFaceId) const;

    Mesh::CirculatorVertexIncidentCornerIds GetVertexIncidentCornerIdsCirculatorBegin(const Mesh::VertexId inVertexId) const;
    Mesh::CirculatorVertexIncidentCornerIds GetVertexIncidentCornerIdsCirculatorEnd() const;

    static bool IsValid(const Mesh::Id inId);
    static bool IsValid(const std::optional<Mesh::Id> inOptionalId);
    const std::vector<Mesh::VertexData>& GetVerticesData() const;
    const std::vector<Mesh::FaceData>& GetFacesData() const;
    const std::vector<Mesh::CornerData>& GetCornersData() const;
    std::optional<Mesh::CornerId> GetOppositeCornerId(const Mesh::CornerId inCornerId) const;
    Mesh::VertexId GetVertexId(const Mesh::CornerId inCornerId) const;
    Mesh::CornerId GetPreviousCornerId(const Mesh::CornerId inCornerId) const;
    Mesh::CornerId GetNextCornerId(const Mesh::CornerId inCornerId) const;
    std::optional<Mesh::CornerId> GetPreviousIncidentCornerId(const Mesh::CornerId inCornerId) const;
    std::optional<Mesh::CornerId> GetNextIncidentCornerId(const Mesh::CornerId inCornerId) const;
    Mesh::FaceId GetFaceIdFromCornerId(const Mesh::CornerId inCornerId) const;
    Mesh::FaceId GetFaceIdFromVertexId(const Mesh::VertexId inVertexId) const;
    std::optional<Mesh::CornerId> GetCornerIdFromFaceIdAndVertexId(const Mesh::FaceId inFaceId, const Mesh::VertexId inVertexId) const;
    Mesh::CornerId GetCornerIdFromFaceIdAndInternalCornerId(const Mesh::FaceId inFaceId, const Mesh::InternalCornerId inInternalCornerId) const;
    std::size_t GetNumberOfFaces() const;
    std::size_t GetNumberOfVertices() const;
    std::size_t GetNumberOfCorners() const;

private:
    std::vector<Mesh::CornerData> mCornersData;
    std::vector<Mesh::VertexData> mVerticesData;
    std::vector<Mesh::FaceData> mFacesData;
};
}