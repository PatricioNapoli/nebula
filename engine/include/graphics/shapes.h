#pragma once

#include "scene/render_component.h"

namespace nebula::shapes {
    inline GLfloat SkyboxVertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    inline RenderComponent::Mesh sphere(int xSegments, int ySegments) {
        NEBULA_PROFILE;

        vector<RenderComponent::Vertex> vertices{};
        vector<unsigned int> indices{};

        const float PI = 3.14159265359;
        for (unsigned int y = 0; y <= ySegments; ++y)
        {
            for (unsigned int x = 0; x <= xSegments; ++x)
            {
                float xSegment = (float)x / (float)xSegments;
                float ySegment = (float)y / (float)ySegments;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                vertices.emplace_back(
                        glm::vec3(xPos, yPos, zPos),
                        glm::vec3(xPos, yPos, zPos),
                        glm::vec2(xSegment, ySegment)
                        );
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < ySegments; ++y)
        {
            if (!oddRow)
            {
                for (unsigned int x = 0; x <= xSegments; ++x)
                {
                    indices.push_back(y       * (xSegments + 1) + x);
                    indices.push_back((y + 1) * (xSegments + 1) + x);
                }
            }
            else
            {
                for (int x = xSegments; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (xSegments + 1) + x);
                    indices.push_back(y       * (xSegments + 1) + x);
                }
            }
            oddRow = !oddRow;
        }

        RenderComponent::Mesh mesh{vertices, indices, Material("main")};
        return mesh;
    }
}
