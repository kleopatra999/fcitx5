/*
 * Copyright (C) 2016~2016 by CSSlayer
 * wengxt@gmail.com
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see the file COPYING. If not,
 * see <http://www.gnu.org/licenses/>.
 */
#ifndef _FCITX_UI_CLASSIC_WAYLANDUI_H_
#define _FCITX_UI_CLASSIC_WAYLANDUI_H_

#include "classicui.h"
#include "display.h"
#include <EGL/egl.h>
#include <cairo/cairo.h>
#include <wayland-egl.h>

namespace fcitx {
namespace classicui {

class WaylandUI : public UIInterface {
public:
    WaylandUI(ClassicUI *parent, const std::string &name, wl_display *conn);
    ~WaylandUI();

    bool initEGL();
    EGLSurface createEGLSurface(wl_egl_window *window,
                                const EGLint *attrib_list);
    void destroyEGLSurface(EGLSurface surface);

    cairo_surface_t *createEGLCairoSurface(EGLSurface surface, int width,
                                           int height);

    ClassicUI *parent() const { return parent_; }
    const std::string &name() const { return name_; }
    wayland::Display *display() const { return display_; }

private:
    static const struct wl_registry_listener registryListener;
    ClassicUI *parent_;
    std::string name_;
    wayland::Display *display_;

    // EGL stuff
    bool hasEgl_;
    EGLDisplay eglDisplay_ = nullptr;
    EGLConfig argbConfig_ = nullptr;
    EGLContext argbCtx_ = nullptr;
    cairo_device_t *argbDevice_ = nullptr;
};
}
}

#endif // _FCITX_UI_CLASSIC_WAYLANDUI_H_
