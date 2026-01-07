//
// Created by Julian Kalb on 06/01/2026.
//
#pragma once
#include <nuklear_glfw_gl3.h>
#include "engine/Game.h"

namespace NK_WRAP {
    static bool button_label(nk_context* pCtx, const char* pLabel, std::unordered_map<std::string, bool>& HoverStates, gl3::engine::Game* pCore) {
        struct nk_rect bounds = nk_widget_bounds(pCtx);

        bool hovered = nk_input_is_mouse_hovering_rect(&pCtx->input, bounds);
        bool clicked = nk_input_is_mouse_click_in_rect(&pCtx->input, NK_BUTTON_LEFT, bounds);
        bool pressed = nk_button_label(pCtx, pLabel);
        bool& bWasHovered = HoverStates[pLabel];

        if (!pCore) return clicked;

        if (hovered && !bWasHovered) {
            pCore->PlaySound("retro_ui_menu_simple_click_03.wav");
        }
        bWasHovered = hovered;
        if (pressed) {
            pCore->PlaySound("retro_ui_menu_simple_click_12.wav");
        }
        return clicked;
    }
}
