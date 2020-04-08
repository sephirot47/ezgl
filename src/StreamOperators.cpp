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
    ioLHS << "VertexData<NeighborFacesIds: " << inRHS.mNeighborFacesId << ", Position: " << inRHS.mPosition << ", Normal: " << inRHS.mNormal << ">";
    return ioLHS;
}
}