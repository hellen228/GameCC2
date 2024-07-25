#ifndef FONDO_H
#define FONDO_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Fondo {
private:
    sf::Sprite _sprite;
    sf::Texture _texture;
    float _scrollSpeed;

public:
    Fondo(const std::string& textureFile, float scrollSpeed) : _scrollSpeed(scrollSpeed) {
        if (!_texture.loadFromFile(textureFile)) {
            std::cerr << "Error cargando textura de fondo: " << textureFile << std::endl;
        }
        _texture.setRepeated(true);
        _sprite.setTexture(_texture);
        _sprite.setTextureRect(sf::IntRect(0, 0, 800, 224)); // Duplica el ancho para permitir el desplazamiento
    }

    void update() {
        _sprite.move(-_scrollSpeed, 0);
        if (_sprite.getPosition().x <= -400) {
            _sprite.setPosition(0, _sprite.getPosition().y);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(_sprite);
    }

    void cambiarFondo(const std::string& nuevoFondo) {
        if (!_texture.loadFromFile(nuevoFondo)) {
            std::cerr << "Error cargando nueva textura de fondo: " << nuevoFondo << std::endl;
        }
        _texture.setRepeated(true);
        _sprite.setTexture(_texture);
        _sprite.setTextureRect(sf::IntRect(0, 0, 800, 224));
    }
};
#endif
