//
// Created by Julian Kalb on 06/01/2026.
//
#pragma once
#include <nuklear_glfw_gl3.h>
#include "engine/Game.h"
#include "engine/SoundSystem.h"

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

        auto pSoundSystem = gl3::engine::SoundSystem::GetInstance();
        if (hovered && !bWasHovered) {
            pSoundSystem->PlaySound(gl3::engine::UI_BUTTON_HOVER);
        }
        bWasHovered = hovered;
        if (pressed) {
            pSoundSystem->PlaySound(gl3::engine::UI_BUTTON_PRESS);
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

        auto pSoundSystem = gl3::engine::SoundSystem::GetInstance();
        if ( hovered && !bWasHovered)
        {
            pSoundSystem->PlaySound(gl3::engine::UI_BUTTON_HOVER);
        }
        bWasHovered = hovered;
        if (clicked)
        {
            pSoundSystem->PlaySound(gl3::engine::UI_BUTTON_PRESS);
        }
        if (changed)
        {
            pSoundSystem->PlaySound(gl3::engine::UI_DRAG_SLIDER);
        }
        return changed;
    }
    static float slider_float(nk_context* pCtx, float min, float fVal, float max, float step, const char* pID,
        std::unordered_map<std::string, bool>& HoverStates, gl3::engine::Game* pCore) {
        struct nk_rect bounds = nk_widget_bounds(pCtx);
        bool hovered = nk_input_is_mouse_hovering_rect(&pCtx->input, bounds);
        bool clicked = nk_input_is_mouse_click_in_rect(&pCtx->input, NK_BUTTON_LEFT, bounds);
        float newValue = nk_slide_float(pCtx, min, fVal, max, step);
        bool& bWasHovered = HoverStates[pID];

        if (!pCore) return clicked;

        auto pSoundSystem = gl3::engine::SoundSystem::GetInstance();
        if ( hovered && !bWasHovered)
        {
            pSoundSystem->PlaySound(gl3::engine::UI_BUTTON_HOVER);
        }
        bWasHovered = hovered;
        if (clicked)
        {
            pSoundSystem->PlaySound(gl3::engine::UI_BUTTON_PRESS);
        }
        if (newValue != fVal)
        {
            pSoundSystem->PlaySound(gl3::engine::UI_DRAG_SLIDER);
        }
        return newValue;
    }
}
