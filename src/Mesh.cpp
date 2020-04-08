#include "Mesh.h"

#include <cassert>
#include <unordered_map>
#include <utility>

#include "Macros.h"
#include "StreamOperators.h"
#include "MeshIterators.h"

namespace egl
{
void Mesh::ComputeNormals()
{
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

    // Add face
    mFacesData.emplace_back();
    const auto new_face_id = mFacesData.size() - 1;
    const std::array new_face_vertices_ids = { inFaceVertexId0, inFaceVertexId1, inFaceVertexId2 };
    mFacesData.at(new_face_id).mVerticesIds = new_face_vertices_ids;

    // For each vertex of the new face, update neighbor faces
    for (Mesh::InternalCornerId internal_corner_id = 0; internal_corner_id < 3; ++internal_corner_id)
    {
        const auto& new_face_vertex_id = new_face_vertices_ids[internal_corner_id];
        std::vector<Mesh::FaceId> new_vertex_ordered_neighbor_faces_ids; // Find the list of neighbor faces ordered in CCW
        {
            // First just build the list of neighbor faces but disordered
            auto vertex_neighbor_faces_ids_disordered = std::move(mVerticesData.at(new_face_vertex_id).mNeighborFacesId);
            vertex_neighbor_faces_ids_disordered.push_back(new_face_id);

            // Now build a map that relates every right vertex to left vertex of each neighbor face
            std::unordered_map<Mesh::VertexId, std::pair<Mesh::VertexId, Mesh::FaceId>> right_vertex_to_left_vertex_and_face_ids;
            for (const auto& vertex_neighbor_face_id : vertex_neighbor_faces_ids_disordered)
            {
                // Left vertex means the following vertex in CCW direction after the Right vertex.
                const auto& vertex_neighbor_face_vertex_id_0 = mFacesData.at(vertex_neighbor_face_id).mVerticesIds[0];
                const auto& vertex_neighbor_face_vertex_id_1 = mFacesData.at(vertex_neighbor_face_id).mVerticesIds[1];
                const auto& vertex_neighbor_face_vertex_id_2 = mFacesData.at(vertex_neighbor_face_id).mVerticesIds[2];
                auto right_and_left_vertex_neighbor_face_vertex_ids = std::make_pair(Mesh::InvalidId, Mesh::InvalidId);
                if (new_face_vertex_id == vertex_neighbor_face_vertex_id_0)
                    right_and_left_vertex_neighbor_face_vertex_ids = std::make_pair(vertex_neighbor_face_vertex_id_1, vertex_neighbor_face_vertex_id_2);
                else if (new_face_vertex_id == vertex_neighbor_face_vertex_id_1)
                    right_and_left_vertex_neighbor_face_vertex_ids = std::make_pair(vertex_neighbor_face_vertex_id_2, vertex_neighbor_face_vertex_id_0);
                else if (new_face_vertex_id == vertex_neighbor_face_vertex_id_2)
                    right_and_left_vertex_neighbor_face_vertex_ids = std::make_pair(vertex_neighbor_face_vertex_id_0, vertex_neighbor_face_vertex_id_1);
                else
                    THROW_EXCEPTION("Found vertex neighbors inconsistencies");

                const auto& right_vertex_id = right_and_left_vertex_neighbor_face_vertex_ids.first;
                const auto& left_vertex_id = right_and_left_vertex_neighbor_face_vertex_ids.second;
                if (right_vertex_to_left_vertex_and_face_ids.count(right_vertex_id) != 0)
                    THROW_EXCEPTION("Non-two manifold mesh detected because a T-junction was found at edge " << right_and_left_vertex_neighbor_face_vertex_ids);

                right_vertex_to_left_vertex_and_face_ids[right_vertex_id] = { left_vertex_id, vertex_neighbor_face_id };
            }

            // Then find the sequence of faces such that the vertices follow a sequence
            new_vertex_ordered_neighbor_faces_ids.reserve(vertex_neighbor_faces_ids_disordered.size());
            auto right_vertex_to_left_vertex_and_face_id_it = right_vertex_to_left_vertex_and_face_ids.cbegin();
            while (right_vertex_to_left_vertex_and_face_id_it != right_vertex_to_left_vertex_and_face_ids.cend())
            {
                const auto right_vertex_to_left_vertex_and_face_id = *(right_vertex_to_left_vertex_and_face_id_it); // Copy bc we erase it later
                const auto& right_vertex_id = right_vertex_to_left_vertex_and_face_id.first;
                const auto& left_vertex_id = right_vertex_to_left_vertex_and_face_id.second.first;
                const auto& face_id = right_vertex_to_left_vertex_and_face_id.second.second;

                // Push the ordered neighbor face, and remove it from our map so that this right-left vertices of the face are no longer processed
                new_vertex_ordered_neighbor_faces_ids.push_back(face_id);
                right_vertex_to_left_vertex_and_face_ids.erase(right_vertex_id);

                // Find next face whose left vertex id is this face right vertex id
                const auto following_right_vertex_id = left_vertex_id;
                right_vertex_to_left_vertex_and_face_id_it = right_vertex_to_left_vertex_and_face_ids.find(following_right_vertex_id);
                if (right_vertex_to_left_vertex_and_face_id_it == right_vertex_to_left_vertex_and_face_ids.cend())
                {
                    // We have finished a chunk of triangles (we have reached a boundary vertex. Start from the beginning with one of the remaining chunks (if any)
                    right_vertex_to_left_vertex_and_face_id_it = right_vertex_to_left_vertex_and_face_ids.cbegin();
                    continue;
                }
            }

            // Update the neighbor faces of our vertex with the CCW ordered list of neighbor faces
            mVerticesData.at(new_face_vertex_id).mNeighborFacesId = std::move(new_vertex_ordered_neighbor_faces_ids);
        }
    }
}

void Mesh::SetVertexPosition(const Mesh::VertexId inVertexId, const Vec3f& inPosition)
{
    mVerticesData.at(inVertexId).mPosition = inPosition;
}

Mesh::CirculatorVertexNeighborFaceIds Mesh::GetVertexNeighborFaceIdsCirculatorBegin(const Mesh::VertexId inVertexId) const
{
    return Mesh::CirculatorVertexNeighborFaceIds(*this, inVertexId);
}

Mesh::CirculatorVertexNeighborFaceIds Mesh::GetVertexNeighborFaceIdsCirculatorEnd() const
{
    return Mesh::CirculatorVertexNeighborFaceIds(*this, Mesh::InvalidId);
}

Range<Mesh::CirculatorVertexNeighborFaceIds> Mesh::AllVertexNeighborFaceIds(const Mesh::VertexId inVertexId) const
{
    return { GetVertexNeighborFaceIdsCirculatorBegin(inVertexId), GetVertexNeighborFaceIdsCirculatorEnd() };
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

std::size_t Mesh::GetNumberOfFaces() const
{
    return mFacesData.size();
}

std::size_t Mesh::GetNumberOfVertices() const
{
    return mVerticesData.size();
}
}