/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file Renderer.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Create IRenderer based on buid platform.
///

#pragma once

#include <memory>

namespace Graphite::Core::Renderer {

class IRenderable
{
public:
    virtual void Render() = 0;
    virtual ~IRenderable() = default;
};

class IRenderer
{
public:
    virtual void Init() = 0;
    virtual void Render(std::shared_ptr<IRenderable> user_interface) = 0;
    virtual void Cleanup() = 0;
    virtual ~IRenderer() = default;
};

std::unique_ptr<IRenderer> CreateRenderer();

} // namespace Graphite::Core::Renderer
