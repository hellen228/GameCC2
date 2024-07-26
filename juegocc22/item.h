#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Item {
private:
    sf::Sprite _sprite;
    sf::Texture _texture;
    bool _isVisible;
    sf::Clock _appearanceClock;
    const float SCALE_FACTOR = 0.8f; // Factor de escala para hacer la imagen más pequeña

public:
    Item(const std::string& textureFile) : _isVisible(false) {
        if (!_texture.loadFromFile(textureFile)) {
            std::cerr << "Error cargando textura de item: " << textureFile << std::endl;
            std::exit(EXIT_FAILURE);
        }

        _sprite.setTexture(_texture);
        _sprite.setScale(SCALE_FACTOR, SCALE_FACTOR); // Aplica el factor de escala
        _appearanceClock.restart();
    }

    void update(float screenWidth, float screenHeight) {
        if (!_isVisible && _appearanceClock.getElapsedTime().asSeconds() >= 8.0f) {
            _isVisible = true;
            float itemWidth = _sprite.getGlobalBounds().width;
            float itemHeight = _sprite.getGlobalBounds().height;
            float x = screenWidth - itemWidth - 30; // 30 píxeles desde el borde derecho
            float y = (screenHeight - itemHeight) / 2; // Centrado verticalmente
            setPosition(x, y);
        }
    }

    void setPosition(float x, float y) {
        _sprite.setPosition(x, y);
    }

    sf::Vector2f getPosition() const {
        return _sprite.getPosition();
    }

    sf::FloatRect getGlobalBounds() const {
        return _sprite.getGlobalBounds();
    }

    void draw(sf::RenderWindow& window) {
        if (_isVisible) {
            window.draw(_sprite);
        }
    }

    bool isVisible() const {
        return _isVisible;
    }

    void reset() {
        _isVisible = false;
        _appearanceClock.restart();
    }
};

#endif