/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file Renderer.mm
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Implementation of @see Renderer.hpp::CreateRenderer.
///

#ifdef GRAPHITE_USE_METAL_API
#include "Renderer.hpp"

#include "core/application/renderer/backends/metal/MetalRenderer.hpp"

namespace Graphite::Core::Renderer {

std::unique_ptr<IRenderer> CreateRenderer()
{
    return std::make_unique<Backends::Metal::MetalRenderer>();
}

} // namespace Graphite::Core::Renderer

#endif
