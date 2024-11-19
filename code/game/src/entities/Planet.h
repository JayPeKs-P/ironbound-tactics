//
// Created by julia on 29/10/2024.
//

#pragma once
#include "Entity.h"


namespace gl3 {

class Planet: public Entity {
    public:
        explicit Planet(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                float size = 1.0f,
                glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

};

} // gl3