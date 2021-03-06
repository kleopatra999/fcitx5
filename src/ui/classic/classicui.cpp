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

#include "classicui.h"
#include "fcitx/instance.h"
#include "fcitx/userinterfacemanager.h"
#include "waylandui.h"
#include "xcbui.h"

namespace fcitx {
namespace classicui {
ClassicUI::ClassicUI(Instance *instance)
    : UserInterface(), instance_(instance) {
    xcbCreatedCallback_.reset(
        xcb()->call<IXCBModule::addConnectionCreatedCallback>(
            [this](const std::string &name, xcb_connection_t *conn, int screen,
                   FocusGroup *) {
                uis_["x11:" + name].reset(new XCBUI(this, name, conn, screen));
            }));
    xcbClosedCallback_.reset(
        xcb()->call<IXCBModule::addConnectionClosedCallback>(
            [this](const std::string &name, xcb_connection_t *) {
                uis_.erase("x11:" + name);
            }));
    waylandCreatedCallback_.reset(
        wayland()->call<IWaylandModule::addConnectionCreatedCallback>(
            [this](const std::string &name, wl_display *display, FocusGroup *) {
                try {
                    uis_["wayland:" + name].reset(
                        new WaylandUI(this, name, display));
                } catch (std::runtime_error) {
                }
            }));
    waylandClosedCallback_.reset(
        wayland()->call<IWaylandModule::addConnectionClosedCallback>(
            [this](const std::string &name, wl_display *) {
                uis_.erase("wayland:" + name);
            }));
}

ClassicUI::~ClassicUI() {}

AddonInstance *ClassicUI::xcb() {
    auto &addonManager = instance_->addonManager();
    return addonManager.addon("xcb");
}

AddonInstance *ClassicUI::wayland() {
    auto &addonManager = instance_->addonManager();
    return addonManager.addon("wayland");
}

void ClassicUI::suspend() {}

void ClassicUI::resume() {}

void ClassicUI::update(UserInterfaceComponent component,
                       InputContext *inputContext) {}

class ClassicUIFactory : public AddonFactory {
public:
    AddonInstance *create(AddonManager *manager) override {
        return new ClassicUI(manager->instance());
    }
};
}
}

FCITX_ADDON_FACTORY(fcitx::classicui::ClassicUIFactory);
