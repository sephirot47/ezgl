#include "StreamOperators.h"

#include "Mesh.h"

namespace egl
{

std::ostream& operator<<(std::ostream& ioLHS, const Mesh::Edge& inRHS)
{
    ioLHS << "Edge<" << inRHS[0] << ", " << inRHS[1] << ">";
    return ioLHS;
}

std::ostream& operator<<(std::ostream& ioLHS, const Mesh::VertexData& inRHS)
{
    ioLHS << "VertexData<FaceId: " << inRHS.mIncidentFaceId << ", Position: " << inRHS.mPosition << ", Normal: " << inRHS.mNormal << ">";
    return ioLHS;
}

std::ostream& operator<<(std::ostream& ioLHS, const Mesh::CornerData& inRHS)
{
    ioLHS << "CornerData<VertexId: " << inRHS.mVertexId << ", FaceId: " << inRHS.mFaceId << ", OppositeCornerId: " << inRHS.mOppositeCornerId << ">";
    return ioLHS;
}
}