//
// Created by Julian Kalb on 23/07/2025.
//
#include "Unit.h"
#include <json.hpp>
using json = nlohmann::json;

inline UnitCategory categoryFromString(const std::string& str) {
    if (str == "INFANTRY") return UnitCategory::INFANTRY;
    if (str == "ARCHER") return UnitCategory::ARCHER;
    if (str == "CATAPULT") return UnitCategory::CATAPULT;
    throw std::runtime_error("Unknown UnitCategory: " + str);
}
Unit::Unit(guid_t owner, const std::string& presetKey) : Component(owner)
{
    static json presets;

    if (presets.empty()) {
        std::ifstream f("assets/presets/Units.json");
        if (!f.is_open()) throw std::runtime_error("Could not open Units.json");
        f >> presets;
    }

    if (!presets.contains(presetKey)) {
        throw std::runtime_error("Preset not found: " + presetKey);
    }

    const json& key = presets[presetKey];

    category = categoryFromString(key.at("category"));
    hpValue = key.at("hpValue");
    armorValue = key.at("armorValue");
    attackValue = key.at("attackValue");
    accuracy = key.at("accuracyValue");
    critChance = key.at("critChance");
    critMultiplier = key.value("critMultiplier", 2.0f);
    lifetimeMaxAmount = key.value("lifeTimeMaxAmount", 0);
    totalAmount = key.value("totalAmount", 0);
    totalAmountLastFrame = key.value("totalAmountLastFrame", totalAmount);
    availableAmount = key.value("availableAmount", totalAmount);
    speed = key.at("speed");
    texturePath = key.at("texturePath");
}
