#pragma once

#include <list>
#include <map>
#include <ostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Mesh.h"

namespace egl
{
template <typename TFirst, typename TSecond>
std::ostream& operator<<(std::ostream& ioLHS, const std::pair<TFirst, TSecond>& inRHS)
{
    ioLHS << "<" << inRHS.first << ", " << inRHS.second << ">";
    return ioLHS;
}

template <typename T>
std::ostream& operator<<(std::ostream& ioLHS, const std::vector<T>& inRHS)
{
    ioLHS << "(";
    for (auto it = inRHS.cbegin(); it != inRHS.cend(); ++it)
        ioLHS << (it == inRHS.cbegin() ? "" : ", ") << (*it);
    ioLHS << ")";
    return ioLHS;
}

template <typename T>
std::ostream& operator<<(std::ostream& ioLHS, const std::list<T>& inRHS)
{
    ioLHS << "(";
    for (auto it = inRHS.cbegin(); it != inRHS.cend(); ++it)
        ioLHS << (it == inRHS.cbegin() ? "" : ", ") << (*it);
    ioLHS << ")";
    return ioLHS;
}

template <typename TKey, typename TValue>
std::ostream& operator<<(std::ostream& ioLHS, const std::map<TKey, TValue>& inRHS)
{
    ioLHS << "{";
    for (auto it = inRHS.cbegin(); it != inRHS.cend(); ++it)
        ioLHS << (it == inRHS.cbegin() ? "" : ", ") << (*it);
    ioLHS << "}";
    return ioLHS;
}

template <typename TKey, typename TValue>
std::ostream& operator<<(std::ostream& ioLHS, const std::unordered_map<TKey, TValue>& inRHS)
{
    ioLHS << "{";
    for (auto it = inRHS.cbegin(); it != inRHS.cend(); ++it)
        ioLHS << (it == inRHS.cbegin() ? "" : ", ") << (*it);
    ioLHS << "}";
    return ioLHS;
}

template <typename T>
std::ostream& operator<<(std::ostream& ioLHS, const std::set<T>& inRHS)
{
    ioLHS << "{";
    for (auto it = inRHS.cbegin(); it != inRHS.cend(); ++it)
        ioLHS << (it == inRHS.cbegin() ? "" : ", ") << (*it);
    ioLHS << "}";
    return ioLHS;
}

template <typename T>
std::ostream& operator<<(std::ostream& ioLHS, const std::unordered_set<T>& inRHS)
{
    ioLHS << "{";
    for (auto it = inRHS.cbegin(); it != inRHS.cend(); ++it)
        ioLHS << (it == inRHS.cbegin() ? "" : ", ") << (*it);
    ioLHS << "}";
    return ioLHS;
}

template <typename T>
std::ostream& operator<<(std::ostream& ioLHS, const std::optional<T>& inRHS)
{
    ioLHS << "OPT<";
    if (inRHS.has_value())
        ioLHS << *inRHS;
    else
        ioLHS << "null";
    ioLHS << ">";
    return ioLHS;
}

std::ostream& operator<<(std::ostream& ioLHS, const Mesh::Edge& inRHS);
std::ostream& operator<<(std::ostream& ioLHS, const Mesh::VertexData& inRHS);
}