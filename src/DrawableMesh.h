#pragma once

#include <memory>

#include "EBO.h"
#include "Mesh.h"
#include "VAO.h"
#include "VBO.h"

namespace egl
{
class DrawableMesh : public Mesh
{
public:
    static constexpr GL::Id PositionAttribLocation() { return 0; }
    static constexpr GL::Id NormalAttribLocation() { return 1; }

    DrawableMesh() = default;
    DrawableMesh(const DrawableMesh&) = default;
    DrawableMesh& operator=(const DrawableMesh&) = default;
    DrawableMesh& operator=(DrawableMesh&&) = default;
    DrawableMesh(DrawableMesh&&) = default;
    virtual ~DrawableMesh() = default;

    void UpdateVAOs();
    void Draw();

    void Read(const std::filesystem::path& inMeshPath) final;

private:
    VAO mVAO;
};
}