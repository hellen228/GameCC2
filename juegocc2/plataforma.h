#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

class Plataforma {
private:
    sf::RectangleShape _shape;
    sf::Texture _texture;
public:
    Plataforma(float x, float y, float width, float height, const std::string& textureFile) {
        _shape.setPosition(x, y);
        _shape.setSize(sf::Vector2f(width, height));
        if (_texture.loadFromFile(textureFile)) {
            _shape.setTexture(&_texture);
        } else {
            _shape.setFillColor(sf::Color::White);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(_shape);
    }

    void move(float offsetX, float offsetY) {
        _shape.move(offsetX, offsetY);
    }

    sf::FloatRect getGlobalBounds() const {
        return _shape.getGlobalBounds();
    }
};
class PlataformaFactory {
private:
    const float PLATFORM_SPEED = 0.04f;
    const float PLATFORM_HEIGHT = 10.0f;
    const float MIN_PLATFORM_WIDTH = 40.0f;
    const float MAX_PLATFORM_WIDTH = 80.0f;
    const float MIN_PLATFORM_Y = 100.0f;
    const float MAX_PLATFORM_Y = 200.0f;
    const float PLATFORM_VERTICAL_GAP = 50.0f;
    const float MIN_HORIZONTAL_DISTANCE = 70.0f; // Distancia mínima entre plataformas
    float lastPlatformRight = 0.0f; // Posición x del borde derecho de la última plataforma

public:
    PlataformaFactory() {
        srand(static_cast<unsigned>(time(0)));
    }

    std::vector<Plataforma> createInitialPlataformas(int count, float maxWidth, float maxHeight) {
        std::vector<Plataforma> plataformas;
        float lastY = MAX_PLATFORM_Y;
        lastPlatformRight = 0.0f;

        for (int i = 0; i < count; ++i) {
            float width = randomFloat(MIN_PLATFORM_WIDTH, MAX_PLATFORM_WIDTH);
            float x = lastPlatformRight + randomFloat(MIN_HORIZONTAL_DISTANCE, MIN_HORIZONTAL_DISTANCE * 1.5f);
            float y = std::max(MIN_PLATFORM_Y, lastY - randomFloat(0, PLATFORM_VERTICAL_GAP));

            plataformas.emplace_back(x, y, width, PLATFORM_HEIGHT, "floor_texture.png");
            lastY = y;
            lastPlatformRight = x + width;
        }

        return plataformas;
    }

    void updatePlataformas(std::vector<Plataforma>& plataformas, float maxWidth, float maxHeight) {
        // Mover plataformas existentes
        for (auto& plataforma : plataformas) {
            plataforma.move(-PLATFORM_SPEED, 0.0f);
        }

        // Actualizar la posición de la última plataforma
        if (!plataformas.empty()) {
            lastPlatformRight = plataformas.back().getGlobalBounds().left + plataformas.back().getGlobalBounds().width - PLATFORM_SPEED;
        }

        // Eliminar plataformas fuera de la pantalla
        plataformas.erase(
            std::remove_if(plataformas.begin(), plataformas.end(),
                [](const Plataforma& p) { return p.getGlobalBounds().left + p.getGlobalBounds().width < 0; }),
            plataformas.end());

        // Generar nuevas plataformas
        if (lastPlatformRight < maxWidth) {
            float width = randomFloat(MIN_PLATFORM_WIDTH, MAX_PLATFORM_WIDTH);
            float x = maxWidth + randomFloat(MIN_HORIZONTAL_DISTANCE, MIN_HORIZONTAL_DISTANCE * 0.5f);
            float y;

            if (!plataformas.empty()) {
                float lastY = plataformas.back().getGlobalBounds().top;
                y = std::max(MIN_PLATFORM_Y, std::min(MAX_PLATFORM_Y, lastY + randomFloat(-PLATFORM_VERTICAL_GAP, PLATFORM_VERTICAL_GAP)));
            } else {
                y = randomFloat(MIN_PLATFORM_Y, MAX_PLATFORM_Y);
            }

            plataformas.emplace_back(x, y, width, PLATFORM_HEIGHT, "floor_texture.png");
            lastPlatformRight = x + width;
        }
    }

private:
    float randomFloat(float min, float max) {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }
};

#endif // PLATAFORMA_H