//
// Created by Julian Kalb on 06/01/2026.
//
#pragma once
#include <nuklear_glfw_gl3.h>
#include "engine/Game.h"

namespace NK_WRAP {
    static bool button_label(nk_context* pCtx, std::string pLabel, std::unordered_map<std::string, bool>& HoverStates, gl3::engine::Game* pCore, uint8_t identifier = 0) {
        struct nk_rect bounds = nk_widget_bounds(pCtx);

        bool hovered = nk_input_is_mouse_hovering_rect(&pCtx->input, bounds);
        bool clicked = nk_input_is_mouse_click_in_rect(&pCtx->input, NK_BUTTON_LEFT, bounds);
        bool pressed = nk_button_label(pCtx, pLabel.c_str());
        std::string mapKey = pLabel;
        if (identifier != 0) {
            mapKey += std::to_string(identifier);
        }
        bool& bWasHovered = HoverStates[mapKey];

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
    static bool slider_int(nk_context* pCtx, int min, int* pVal, int max, int step, const char* pID,
        std::unordered_map<std::string, bool>& HoverStates, gl3::engine::Game* pCore) {
        struct nk_rect bounds = nk_widget_bounds(pCtx);
        bool hovered = nk_input_is_mouse_hovering_rect(&pCtx->input, bounds);
        bool clicked = nk_input_is_mouse_click_in_rect(&pCtx->input, NK_BUTTON_LEFT, bounds);
        bool changed = nk_slider_int(pCtx, min, pVal, max, step);
        bool& bWasHovered = HoverStates[pID];

        if (!pCore) return clicked;

        if ( hovered && !bWasHovered)
        {
            pCore->PlaySound("retro_ui_menu_simple_click_03.wav");
        }
        bWasHovered = hovered;
        if (clicked)
        {
            pCore->PlaySound("retro_ui_menu_simple_click_12.wav");
        }
        if (changed)
        {
            pCore->PlaySound("retro_ui_menu_blip_click_20.wav");
        }
        return changed;
    }
}
