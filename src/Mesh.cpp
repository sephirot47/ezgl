#include "Mesh.h"

#include <cassert>

#include "Macros.h"
#include "StreamOperators.h"
#include "MeshIterators.h"

namespace egl
{
void Mesh::ComputeNormals()
{
    /*
    for (Mesh::FaceId face_id = 0; face_id < mFacesData.size(); ++face_id)
    {
        const auto& face_vertices_ids = mVerticesData.at(face_id);

        const auto& v_id_0 = face_vertices_ids.at(0);
        const auto& v_id_1 = face_vertices_ids.at(1);
        const auto& v_id_2 = face_vertices_ids.at(2);
        const auto v0 = mVerticesData.at(v_id_0).mPosition;
        const auto v1 = mVerticesData.at(v_id_1).mPosition;
        const auto v2 = mVerticesData.at(v_id_2).mPosition;
        const auto normal = Cross(Normalized(v0 - v1), Normalized(v2 - v1));
        mFacesData.at(face_id).mNormal = normal;
    }
    */
}

void Mesh::AddVertex(const Mesh::VertexData& inVertexData)
{
    mVerticesData.emplace_back(inVertexData);
}

void Mesh::AddFace(const Mesh::VertexId& inFaceVertexId0, const Mesh::VertexId& inFaceVertexId1, const Mesh::VertexId& inFaceVertexId2)
{
    // Vertices must have been added before
    EXPECTS(inFaceVertexId0 < mVerticesData.size());
    EXPECTS(inFaceVertexId1 < mVerticesData.size());
    EXPECTS(inFaceVertexId2 < mVerticesData.size());

    mFacesData.emplace_back();
    const auto new_face_id = mFacesData.size() - 1;
    const std::array new_face_vertices_ids = { inFaceVertexId0, inFaceVertexId1, inFaceVertexId2 };
    for (const auto& new_face_vertex_id : new_face_vertices_ids)
    {
        auto& vertex_incident_face_id = mVerticesData.at(new_face_vertex_id).mIncidentFaceId;
        if (!Mesh::IsValid(vertex_incident_face_id))
            vertex_incident_face_id = new_face_id;
    }

    // Add the 3 corners data
    for (Mesh::InternalCornerId internal_corner_id = 0; internal_corner_id < 3; ++internal_corner_id)
    {
        assert(mCornersData.size() == GetCornerIdFromFaceIdAndInternalCornerId(new_face_id, internal_corner_id));

        Mesh::CornerData corner_data;
        corner_data.mVertexId = new_face_vertices_ids.at(internal_corner_id);
        corner_data.mFaceId = new_face_id;
        mCornersData.push_back(corner_data);
    }

    // Fill opposite corner ids information
    for (Mesh::InternalCornerId internal_corner_id = 0; internal_corner_id < 3; ++internal_corner_id)
    {
        const auto new_corner_id = GetCornerIdFromFaceIdAndInternalCornerId(new_face_id, internal_corner_id);
        auto& corner_data = mCornersData.at(new_corner_id);
        const auto vertex_id_of_new_corner = corner_data.mVertexId;

        const auto new_face_next_corner_id = GetNextCornerId(new_corner_id);
        const auto new_face_next_vertex_id = GetVertexId(new_face_next_corner_id);
        const auto new_face_previous_corner_id = GetPreviousCornerId(new_corner_id);
        const auto new_face_previous_vertex_id = GetVertexId(new_face_next_corner_id);
        const auto new_face_face_id_incident_to_vertex = GetFaceIdFromVertexId(new_face_next_vertex_id);

        // Look for the opposite corner in an face incident of the current corner
        Mesh::CornerId opposite_corner_id = Mesh::InvalidId;
        {
            // Look for the opposite edge to the current corner in a face incident to the current vertex
            // If we find it, we know that the remaining corner id of such incident face is the opposite corner id of the current corner.
            const auto other_same_face_vertex_id_0 = new_face_vertices_ids.at((internal_corner_id + 1) % 3);
            const auto other_same_face_vertex_id_1 = new_face_vertices_ids.at((internal_corner_id + 2) % 3);
            const auto opposite_edge_in_same_face = Edge { other_same_face_vertex_id_0, other_same_face_vertex_id_1 };
            for (const bool go_forward : { true, false })
            {
                for (const auto search_vertex_id : { other_same_face_vertex_id_0, other_same_face_vertex_id_1 })
                {
                    // for (auto vertex_corners_it = GetVertexIncidentCornerIdsCirculatorBegin(vertex_id_of_new_corner);
                    //      vertex_corners_it.IsValid() && vertex_corners_it != GetVertexIncidentCornerIdsCirculatorEnd();
                    //      (go_forward ? ++vertex_corners_it : --vertex_corners_it))
                    const auto begin_search_face_id = GetFaceIdFromVertexId(search_vertex_id);
                    const auto begin_search_corner_id_opt = GetCornerIdFromFaceIdAndVertexId(begin_search_face_id, search_vertex_id); // Begin at some already registered corner
                    assert(begin_search_corner_id_opt.has_value());
                    const auto begin_search_corner_id = *begin_search_corner_id_opt;
                    for (auto current_corner_id = std::make_optional(begin_search_corner_id);
                         Mesh::IsValid(current_corner_id);
                         current_corner_id = (go_forward ? GetNextIncidentCornerId(*current_corner_id) : GetPreviousIncidentCornerId(*current_corner_id)))
                    {
                        const auto search_corner_id = *current_corner_id;
                        if (GetFaceIdFromCornerId(search_corner_id) == new_face_id) // Only search for a matching edge in other faces
                            continue;

                        assert(search_vertex_id == GetVertexId(search_corner_id));

                        const auto search_face_id = GetFaceIdFromCornerId(search_corner_id);
                        const auto search_face_corner_id_0 = GetCornerIdFromFaceIdAndInternalCornerId(search_face_id, 0);
                        const auto search_face_corner_id_1 = GetCornerIdFromFaceIdAndInternalCornerId(search_face_id, 1);
                        const auto search_face_corner_id_2 = GetCornerIdFromFaceIdAndInternalCornerId(search_face_id, 2);
                        assert(search_corner_id == search_face_corner_id_0 || search_corner_id == search_face_corner_id_1 || search_corner_id == search_face_corner_id_2);
                        assert(GetFaceIdFromCornerId(search_face_corner_id_0) == GetFaceIdFromCornerId(search_face_corner_id_1));
                        assert(GetFaceIdFromCornerId(search_face_corner_id_0) == GetFaceIdFromCornerId(search_face_corner_id_2));
                        assert(GetFaceIdFromCornerId(search_face_corner_id_1) == GetFaceIdFromCornerId(search_face_corner_id_2));

                        const auto search_face_vertex_id_0 = GetVertexId(search_face_corner_id_0);
                        const auto search_face_vertex_id_1 = GetVertexId(search_face_corner_id_1);
                        const auto search_face_vertex_id_2 = GetVertexId(search_face_corner_id_2);
                        const auto search_face_edge_0_1 = Edge { search_face_vertex_id_0, search_face_vertex_id_1 };
                        const auto search_face_edge_0_2 = Edge { search_face_vertex_id_0, search_face_vertex_id_2 };
                        const auto search_face_edge_1_2 = Edge { search_face_vertex_id_1, search_face_vertex_id_2 };

                        if (opposite_edge_in_same_face == search_face_edge_0_1)
                            opposite_corner_id = GetCornerIdFromFaceIdAndInternalCornerId(search_face_id, 2);
                        else if (opposite_edge_in_same_face == search_face_edge_0_2)
                            opposite_corner_id = GetCornerIdFromFaceIdAndInternalCornerId(search_face_id, 1);
                        else if (opposite_edge_in_same_face == search_face_edge_1_2)
                            opposite_corner_id = GetCornerIdFromFaceIdAndInternalCornerId(search_face_id, 0);

                        if (Mesh::IsValid(opposite_corner_id))
                            break;
                    }

                    if (Mesh::IsValid(opposite_corner_id))
                        break;
                }
            }

            assert(opposite_corner_id != new_corner_id);
            assert(GetFaceIdFromCornerId(opposite_corner_id) != new_face_id);

            // Co-update opposite corner ids
            if (Mesh::IsValid(opposite_corner_id))
            {
                corner_data.mOppositeCornerId = opposite_corner_id;
                mCornersData.at(opposite_corner_id).mOppositeCornerId = new_corner_id;
            }
        }
    }
}

void Mesh::SetVertexPosition(const Mesh::VertexId inVertexId, const Vec3f& inPosition)
{
    mVerticesData.at(inVertexId).mPosition = inPosition;
}

Mesh::CirculatorVertexIncidentCornerIds Mesh::GetVertexIncidentCornerIdsCirculatorBegin(const Mesh::VertexId inVertexId) const
{
    return Mesh::CirculatorVertexIncidentCornerIds { *this, inVertexId };
}

Mesh::CirculatorVertexIncidentCornerIds Mesh::GetVertexIncidentCornerIdsCirculatorEnd() const
{
    return Mesh::CirculatorVertexIncidentCornerIds { *this, Mesh::InvalidId };
}

std::optional<Mesh::CornerId> Mesh::GetOppositeCornerId(const Mesh::CornerId inCornerId) const
{
    EXPECTS(inCornerId < mCornersData.size());
    const auto opposite_corner_id = mCornersData.at(inCornerId).mOppositeCornerId;
    return Mesh::IsValid(opposite_corner_id) ? std::make_optional(opposite_corner_id) : std::nullopt;
}

Mesh::VertexId Mesh::GetVertexId(const Mesh::CornerId inCornerId) const
{
    EXPECTS(inCornerId < mCornersData.size());
    return mCornersData.at(inCornerId).mVertexId;
}

Mesh::CornerId Mesh::GetPreviousCornerId(const Mesh::CornerId inCornerId) const
{
    return (GetFaceIdFromCornerId(inCornerId) * 3 + ((inCornerId + 2) % 3)); // Dont use -1 to avoid mod of negative number
}

Mesh::CornerId Mesh::GetNextCornerId(const Mesh::CornerId inCornerId) const
{
    return (GetFaceIdFromCornerId(inCornerId) * 3 + ((inCornerId + 1) % 3));
}

std::optional<Mesh::CornerId> Mesh::GetPreviousIncidentCornerId(const Mesh::CornerId inCornerId) const
{
    const auto previous_corner_id = GetPreviousCornerId(inCornerId);
    const auto opt_previous_opposite_corner_id = GetOppositeCornerId(previous_corner_id);
    if (!Mesh::IsValid(opt_previous_opposite_corner_id))
        return std::nullopt;
    const auto opt_opposite_corner_id = GetPreviousCornerId(*opt_previous_opposite_corner_id);
    return opt_opposite_corner_id;
}

std::optional<Mesh::CornerId> Mesh::GetNextIncidentCornerId(const Mesh::CornerId inCornerId) const
{
    const auto next_corner_id = GetNextCornerId(inCornerId);
    const auto opt_next_opposite_corner_id = GetOppositeCornerId(next_corner_id);
    if (!Mesh::IsValid(opt_next_opposite_corner_id))
        return std::nullopt;
    const auto opt_opposite_corner_id = GetNextCornerId(*opt_next_opposite_corner_id);
    return opt_opposite_corner_id;
}

std::array<Mesh::CornerId, 3> Mesh::GetFaceCornerIds(const Mesh::FaceId inFaceId) const
{
    return { (inFaceId * 3 + 0), (inFaceId * 3 + 1), (inFaceId * 3 + 2) };
}

std::array<Mesh::VertexId, 3> Mesh::GetFaceVerticesIds(const Mesh::FaceId inFaceId) const
{
    const auto face_corner_ids = GetFaceCornerIds(inFaceId);
    return { mCornersData.at(face_corner_ids.at(0)).mVertexId,
        mCornersData.at(face_corner_ids.at(1)).mVertexId,
        mCornersData.at(face_corner_ids.at(2)).mVertexId };
}

bool Mesh::IsValid(const Mesh::Id inId)
{
    return inId != Mesh::InvalidId;
}

bool Mesh::IsValid(const std::optional<Mesh::Id> inOptionalId)
{
    return inOptionalId.has_value() ? Mesh::IsValid(*inOptionalId) : false;
}

const std::vector<Mesh::VertexData>& Mesh::GetVerticesData() const
{
    return mVerticesData;
}

const std::vector<Mesh::FaceData>& Mesh::GetFacesData() const
{
    return mFacesData;
}

const std::vector<Mesh::CornerData>& Mesh::GetCornersData() const
{
    return mCornersData;
}

Mesh::FaceId Mesh::GetFaceIdFromCornerId(const Mesh::CornerId inCornerId) const
{
    return inCornerId / 3;
}

Mesh::FaceId Mesh::GetFaceIdFromVertexId(const Mesh::VertexId inVertexId) const
{
    EXPECTS(inVertexId < mVerticesData.size());
    return mVerticesData.at(inVertexId).mIncidentFaceId;
}

std::optional<Mesh::CornerId> Mesh::GetCornerIdFromFaceIdAndVertexId(const Mesh::FaceId inFaceId, const Mesh::VertexId inVertexId) const
{
    for (Mesh::InternalCornerId internal_corner_id = 0; internal_corner_id < 3; ++internal_corner_id)
    {
        const auto corner_id = GetCornerIdFromFaceIdAndInternalCornerId(inFaceId, internal_corner_id);
        const auto vertex_id_of_corner = GetVertexId(corner_id);
        if (vertex_id_of_corner == inVertexId)
            return std::make_optional(corner_id);
    }
    return std::nullopt;
}

Mesh::CornerId Mesh::GetCornerIdFromFaceIdAndInternalCornerId(const Mesh::FaceId inFaceId, const Mesh::InternalCornerId inInternalCornerId) const
{
    EXPECTS(inInternalCornerId >= 0 && inInternalCornerId <= 2);
    return inFaceId * 3 + inInternalCornerId;
}

std::size_t Mesh::GetNumberOfFaces() const
{
    return mFacesData.size();
}

std::size_t Mesh::GetNumberOfVertices() const
{
    return mVerticesData.size();
}

std::size_t Mesh::GetNumberOfCorners() const
{
    return mCornersData.size();
}
}