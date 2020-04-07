#pragma once

namespace egl
{
template <typename TMesh, typename TMeshCirculatorImplementation>
class MeshCirculatorBase
{
public:
    using MeshId = typename TMesh::Id;
    static constexpr auto MeshInvalidId = TMesh::InvalidId;

    MeshCirculatorBase(const TMesh& inMesh, const MeshId inStartId)
        : mMesh(inMesh)
        , mBeginId(*TMeshCirculatorImplementation::GetFirstId(mMesh, inStartId))
        , mCurrentId(mBeginId)
    {
    }

    void operator++()
    {
        EXPECTS(TMesh::IsValid(mCurrentId));
        mCurrentId = TMeshCirculatorImplementation::Incremented(mMesh, *mCurrentId);
        ++mStepCounter;
    }

    void operator--()
    {
        EXPECTS(TMesh::IsValid(mCurrentId));
        mCurrentId = TMeshCirculatorImplementation::Decremented(mMesh, *mCurrentId);
        --mStepCounter;
    }

    MeshId operator*() const
    {
        EXPECTS(TMesh::IsValid(mCurrentId));
        return *mCurrentId;
    }

    bool operator==(const MeshCirculatorBase& inRHS) const
    {
        const bool both_are_invalid = (!IsValid() && !inRHS.IsValid());
        return (mCurrentId == inRHS.mCurrentId) || both_are_invalid;
    }

    bool operator!=(const MeshCirculatorBase& inRHS) const
    {
        return !(*this == inRHS);
    }

    bool IsValid() const
    {
        const bool is_invalid = !TMesh::IsValid(mCurrentId) || (mCurrentId == mBeginId && mStepCounter != 0);
        return !is_invalid;
    }

private:
    const TMesh& mMesh;
    int mStepCounter = 0;
    const MeshId mBeginId = MeshInvalidId;
    std::optional<MeshId> mCurrentId = MeshInvalidId;
};

template <typename TMesh>
struct MeshCirculatorVertexIncidentCornerIdsImplementation
{
    static std::optional<typename TMesh::Id> GetFirstId(const TMesh& inMesh, const typename TMesh::Id inBeginVertexId)
    {
        return inMesh.GetCornerIdFromFaceIdAndVertexId(inMesh.GetFaceIdFromVertexId(inBeginVertexId), inBeginVertexId);
    }

    static std::optional<typename TMesh::Id> Incremented(const TMesh& inMesh, const typename TMesh::Id inCurrentCornerId)
    {
        return inMesh.GetNextIncidentCornerId(inCurrentCornerId);
    }

    static std::optional<typename TMesh::Id> Decremented(const TMesh& inMesh, const typename TMesh::Id inCurrentCornerId)
    {
        return inMesh.GetPreviousIncidentCornerId(inCurrentCornerId);
    }
};

template <typename TMesh>
using MeshCirculatorVertexIncidentCornerIds = MeshCirculatorBase<TMesh, MeshCirculatorVertexIncidentCornerIdsImplementation<TMesh>>;
}