#pragma once

#include "core/base.h"
#include "core/resource_manager.h"
#include "core/window.h"
#include "render/renderer.h"
#include "world.h"

class Game {
public:
    Game(const WindowProps& winProps);
    ~Game();

    virtual void onStart();
    virtual void onUpdate(float deltaTime);
    virtual void onShutdown();

    inline void setRenderer(Ref<Renderer> renderer) { m_renderer = renderer; }
    inline void setResourceManager(Ref<ResourceManager> resMgr) { m_resMgr = resMgr; }

    WindowProps& getWinProps() { return m_winProps; }

protected:
    Ref<Renderer> m_renderer;
    Ref<ResourceManager> m_resMgr;

    WindowProps m_winProps;

    std::vector<Ref<World>> m_worlds;
};
