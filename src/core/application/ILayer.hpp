/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file ILayer.hpp
/// @author Alexandru Delegeanu
/// @version 0.3
/// @brief App layer.
///

#pragma once

#include <memory>
#include <string_view>

#include "core/utils/UniqueID.hpp"

namespace Graphite::Core::Application {

template <typename ApplicationState>
class TGraphiteApplication;

template <typename ApplicationState>
class ILayer
{
public:
    using Ptr = std::unique_ptr<ILayer<ApplicationState>>;

    ILayer(std::shared_ptr<Graphite::Core::Application::TGraphiteApplication<ApplicationState>> application)
        : m_application{std::move(application)}
        , m_layer_uid{Graphite::Core::Utils::UniqueID::generate()} {};
    virtual ~ILayer() = default;

    inline Graphite::Core::Utils::UniqueID const& GetUID() const noexcept { return m_layer_uid; }

private:
    friend class Graphite::Core::Application::TGraphiteApplication<ApplicationState>;

    virtual std::string_view GetName() const noexcept = 0;

    virtual void OnPush() = 0;
    virtual void OnRender() = 0;
    virtual void OnPop() = 0;

protected:
    std::shared_ptr<TGraphiteApplication<ApplicationState>> m_application{nullptr};
    Graphite::Core::Utils::UniqueID m_layer_uid{};
};

} // namespace Graphite::Core::Application
