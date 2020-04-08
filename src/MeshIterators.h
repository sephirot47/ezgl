#pragma once

namespace egl
{
template <typename TMesh, typename TMeshCirculatorImplementation>
class MeshCirculatorBase
{
public:
    using MeshId = typename TMesh::Id;
    MeshCirculatorBase(const TMesh& inMesh, const MeshId inCirculatorId)
        : mImpl(inMesh, inCirculatorId)
    {
    }

    void operator++()
    {
        EXPECTS(IsValid());
        TMeshCirculatorImplementation::Increment(mImpl);
    }

    void operator--()
    {
        EXPECTS(IsValid());
        TMeshCirculatorImplementation::Decrement(mImpl);
    }

    MeshId operator*() const
    {
        EXPECTS(IsValid());
        return *(mImpl.Get());
    }

    bool operator==(const MeshCirculatorBase& inRHS) const
    {
        return (mImpl.Get() == inRHS.mImpl.Get());
    }

    bool operator!=(const MeshCirculatorBase& inRHS) const
    {
        return !(*this == inRHS);
    }

    bool IsValid() const
    {
        return TMesh::IsValid(mImpl.Get());
    }

private:
    TMeshCirculatorImplementation mImpl;
};

template <typename TMesh>
struct MeshCirculatorVertexNeighborFaceIdsImplementation
{
    using MeshId = typename TMesh::Id;

    const TMesh& mMesh;
    const MeshId mVertexId = TMesh::InvalidId;
    MeshId mCurrentNeighborFaceId = 0;

    MeshCirculatorVertexNeighborFaceIdsImplementation(const TMesh& inMesh, const MeshId inVertexId)
        : mMesh(inMesh)
        , mVertexId(inVertexId)
    {
    }

    static void Increment(MeshCirculatorVertexNeighborFaceIdsImplementation& ioCirculator)
    {
        ++ioCirculator.mCurrentNeighborFaceId;
    }

    static void Decrement(MeshCirculatorVertexNeighborFaceIdsImplementation& ioCirculator)
    {
        --ioCirculator.mCurrentNeighborFaceId;
    }

    std::optional<typename TMesh::Id> Get() const
    {
        if (!TMesh::IsValid(mVertexId))
            return std::nullopt;

        const auto& vertex_neighbor_faces_ids = mMesh.GetVerticesData().at(mVertexId).mNeighborFacesId;
        if (mCurrentNeighborFaceId >= vertex_neighbor_faces_ids.size())
            return std::nullopt;
        return std::make_optional(vertex_neighbor_faces_ids.at(mCurrentNeighborFaceId));
    }
};

template <typename TMesh>
using MeshCirculatorVertexNeighborFaceIds = MeshCirculatorBase<TMesh, MeshCirculatorVertexNeighborFaceIdsImplementation<TMesh>>;
}