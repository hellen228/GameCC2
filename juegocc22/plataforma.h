#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <SFML/Graphics.hpp>
#include <string>

enum class TipoPlataforma {
    NORMAL,
    CUADRADA,
    PINCHOS_CORTOS,
    PINCHOS_LARGOS
};

class Plataforma {
private:
    sf::RectangleShape _shape;
    sf::Texture _texture;
    TipoPlataforma _tipo;
    float _velocidad;
    bool _esPeligrosa;

public:
    Plataforma(float x, float y, float width, float height, TipoPlataforma tipo, const std::string& textureFile)
        : _tipo(tipo), _velocidad(0.0f), _esPeligrosa(false) {
        _shape.setPosition(x, y);
        _shape.setSize(sf::Vector2f(width, height));
        if (_texture.loadFromFile(textureFile)) {
            _shape.setTexture(&_texture);
        } else {
            _shape.setFillColor(sf::Color::Black);
        }

        _esPeligrosa = (_tipo == TipoPlataforma::PINCHOS_CORTOS || _tipo == TipoPlataforma::PINCHOS_LARGOS);
    }

    void draw(sf::RenderWindow& window) { window.draw(_shape); }
    void move(float offsetX, float offsetY) { _shape.move(offsetX, offsetY); }
    sf::FloatRect getGlobalBounds() const { return _shape.getGlobalBounds(); }
    TipoPlataforma getTipo() const { return _tipo; }
    void setVelocidad(float velocidad) { _velocidad = velocidad; }
    void update(float deltaTime) { move(-_velocidad * deltaTime, 0); }
    bool esPeligrosa() const { return _esPeligrosa; }
    void setPosition(float x, float y) { _shape.setPosition(x, y); }

    bool colisionPeligrosa(const sf::FloatRect& objetoBounds) const {
        if (!_esPeligrosa) return false;

        sf::FloatRect pinchosBounds = _shape.getGlobalBounds();
        float zonaPeligrosa = pinchosBounds.height * 0.3f;

        return objetoBounds.intersects(sf::FloatRect(
            pinchosBounds.left, pinchosBounds.top,
            pinchosBounds.width, zonaPeligrosa
        ));
    }
};

class PlataformaFactory {
private:
    float _velocidadBase;
    const float PLATFORM_HEIGHT = 10.0f;
    const float MIN_PLATFORM_WIDTH = 40.0f;
    const float MAX_PLATFORM_WIDTH = 80.0f;
    const float MIN_PLATFORM_Y = 50.0f;
    const float MAX_PLATFORM_Y = 174.0f;
    const float MIN_HORIZONTAL_DISTANCE = 70.0f;
    const float SQUARE_PLATFORM_SIZE = 50.0f;
    const float CHARACTER_HEIGHT = 24.0f;
    int _nivelActual;

public:
    void incrementarVelocidad() {
        _velocidadBase += 10.0f; // Increase speed by 10, adjust as needed
    }
    PlataformaFactory() : _velocidadBase(50.0f), _nivelActual(1) {}

    std::vector<Plataforma> createPlataformas(int count, int nivel, float screenHeight) {
        std::vector<Plataforma> plataformas;
        float lastX = 0.0f;

        for (int i = 0; i < count; ++i) {
            TipoPlataforma tipo;
            std::string textura;
            float width, height, y;

            int randPlatform = rand() % (nivel + 1);
            switch (randPlatform) {
                case 0:
                    tipo = TipoPlataforma::NORMAL;
                    textura = "picture/plataforma_normal.png";
                    width = randomFloat(MIN_PLATFORM_WIDTH, MAX_PLATFORM_WIDTH);
                    height = PLATFORM_HEIGHT;
                    y = randomFloat(CHARACTER_HEIGHT / 2, screenHeight - CHARACTER_HEIGHT * 1.5);
                    break;
                case 1:
                    tipo = TipoPlataforma::CUADRADA;
                    textura = "picture/plataforma_cuadrada.png";
                    width = height = SQUARE_PLATFORM_SIZE;
                    y = (rand() % 2 == 0) ? 0 : screenHeight - SQUARE_PLATFORM_SIZE;
                    break;
                case 2:
                    if (nivel >= 2) {
                        tipo = TipoPlataforma::PINCHOS_CORTOS;
                        textura = "picture/plataforma_pinchos_cortos.png";
                        width = randomFloat(MIN_PLATFORM_WIDTH, MAX_PLATFORM_WIDTH);
                        height = PLATFORM_HEIGHT;
                        y = (rand() % 2 == 0) ? 0 : screenHeight - PLATFORM_HEIGHT - CHARACTER_HEIGHT / 2;
                    } else {
                        i--;
                        continue;
                    }
                    break;
                case 3:
                    if (nivel >= 3) {
                        tipo = TipoPlataforma::PINCHOS_LARGOS;
                        textura = "picture/plataforma_pinchos_largos.png";
                        width = randomFloat(MIN_PLATFORM_WIDTH, MAX_PLATFORM_WIDTH);
                        height = PLATFORM_HEIGHT;
                        y = randomFloat(screenHeight / 3, screenHeight * 2 / 3);
                    } else {
                        i--;
                        continue;
                    }
                    break;
            }
            
            float x = lastX + randomFloat(MIN_HORIZONTAL_DISTANCE, MIN_HORIZONTAL_DISTANCE * 1.5f);

            bool superpuesto = false;
            for (const auto& plataforma : plataformas) {
                if (std::abs(x - plataforma.getGlobalBounds().left) < MIN_HORIZONTAL_DISTANCE) {
                    superpuesto = true;
                    break;
                }
            }
            if (superpuesto) {
                i--;
                continue;
            }

            plataformas.emplace_back(x, y, width, height, tipo, textura);
            plataformas.back().setVelocidad(_velocidadBase);
            lastX = x + width;
        }

        return plataformas;
    }


void updatePlataformas(std::vector<Plataforma>& plataformas, float maxWidth, float deltaTime) {    const int MAX_VISIBLE_PLATFORMS = 10;
    int visiblePlatforms = 0;

    // Eliminar plataformas fuera de pantalla y contar las visibles
    plataformas.erase(
        std::remove_if(plataformas.begin(), plataformas.end(),
            [&visiblePlatforms, maxWidth](const Plataforma& p) {
                if (p.getGlobalBounds().left + p.getGlobalBounds().width >= 0 && p.getGlobalBounds().left <= maxWidth) {
                    visiblePlatforms++;
                    return false;
                }
                return p.getGlobalBounds().left + p.getGlobalBounds().width < 0;
            }),
        plataformas.end());

    // Añadir una nueva plataforma si es necesario
    if (visiblePlatforms < MAX_VISIBLE_PLATFORMS) {
        float lastX = (plataformas.empty()) ? maxWidth : 
            plataformas.back().getGlobalBounds().left + plataformas.back().getGlobalBounds().width;
        auto nuevasPlataformas = createPlataformas(1, _nivelActual, 224.0f);
        if (!nuevasPlataformas.empty()) {
            nuevasPlataformas[0].setPosition(lastX, nuevasPlataformas[0].getGlobalBounds().top);
            plataformas.push_back(nuevasPlataformas[0]);
        }
    }

    // Actualizar posición de plataformas existentes
    for (auto& plataforma : plataformas) {
        plataforma.update(deltaTime);
    }
}
private:
    float randomFloat(float min, float max) {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }
};

#endif // PLATAFORMA_H