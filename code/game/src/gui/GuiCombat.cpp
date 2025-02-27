//
// Created by julia on 25/02/2025.
//

#include "GuiCombat.h"

using namespace gui;

GuiCombat::GuiCombat(struct nk_context* ctx):ctx(ctx)
{
}

void GuiCombat::setStyleWindow(nk_style* style)
{
    style->window.fixed_background = nk_style_item_image(getTileImage(2, 54, 1, 1, 3072, 3072));
    style->window.border = 10.0f;
    style->window.border_color = nk_rgba(120, 58, 58, 163);
    style->window.header.label_normal = nk_rgba(120, 58, 58, 210);
    style->window.header.label_hover = nk_rgba(120, 58, 58, 210);
    style->window.header.label_active = nk_rgba(120, 58, 58, 210);
    style->window.header.normal = nk_style_item_image(getTileImage(22, 1, 1, 1, 3072, 3072));
    style->window.header.hover = nk_style_item_image(getTileImage(22, 1, 1, 1, 3072, 3072));
    style->window.header.active = nk_style_item_image(getTileImage(22, 1, 1, 1, 3072, 3072));
}

void GuiCombat::setStyleButton(nk_style* style)
{
    style->button.normal = nk_style_item_image(getTileImage(0, 11, 3, 1, 3072, 3072));
    style->button.hover = nk_style_item_image(getTileImage(3, 11, 3, 1, 3072, 3072));
    style->button.active = nk_style_item_image(getTileImage(6, 11, 3, 1, 3072, 3072));
    style->button.text_normal = nk_rgba(255, 255, 255, 255);
    style->button.text_hover = nk_rgba(255, 250, 200, 255);
}

void GuiCombat::setStyleOptionLabel(nk_style* style)
{
    style->option.normal = nk_style_item_image(getTileImage(21, 11, 1, 1, 3072, 3072));
    style->option.hover = nk_style_item_image(getTileImage(22, 11, 1, 1, 3072, 3072));
    style->option.active = nk_style_item_image(getTileImage(23, 11, 1, 1, 3072, 3072));
    style->option.cursor_normal = nk_style_item_image(getTileImage(24, 11, 1, 1, 3072, 3072));
    style->option.cursor_hover = nk_style_item_image(getTileImage(25, 11, 1, 1, 3072, 3072));
    style->option.text_normal = nk_rgba(255, 255, 255, 255);
    style->option.text_hover = nk_rgba(255, 250, 200, 255);
}

void GuiCombat::setStyleProgressBar(nk_style* style)
{
    style->progress.normal = nk_style_item_image(getTileImage(0, 14, 3, 1, 3072, 3072));
    style->progress.cursor_normal = nk_style_item_image(getTileImage(6, 14, 3, 1, 3072, 3072));
}

void GuiCombat::setStyleSlider(nk_style* style)
{
    style->slider.normal = nk_style_item_image(getTileImage(0, 6, 3, 1, 3072, 3072));
    style->slider.hover = nk_style_item_image(getTileImage(0, 6, 3, 1, 3072, 3072));
    style->slider.active = nk_style_item_image(getTileImage(0, 6, 3, 1, 3072, 3072));
    style->slider.cursor_normal = nk_style_item_image(getTileImage(21, 12, 1, 1, 3072, 3072));
    style->slider.cursor_hover = nk_style_item_image(getTileImage(22, 12, 1, 1, 3072, 3072));
    style->slider.cursor_active = nk_style_item_image(getTileImage(23, 12, 1, 1, 3072, 3072));
    style->slider.bar_filled = nk_rgba(100, 100, 200, 200);
    style->slider.bar_normal = nk_rgba(92, 58, 58, 163);
    style->slider.bar_hover = nk_rgba(92, 58, 58, 163);
    style->slider.bar_active = nk_rgba(92, 58, 58, 163);
}

void GuiCombat::setStyleText(nk_style* style)
{
    style->text.color = nk_rgba(255, 255, 255, 255);
}
