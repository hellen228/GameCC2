#ifndef FONDO_H
#define FONDO_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Fondo {
private:
    sf::Sprite _sprite1;
    sf::Sprite _sprite2;
    sf::Texture _texture;
    float _scrollSpeed;
    const float SCREEN_WIDTH = 400.0f;
    const float SCREEN_HEIGHT = 224.0f;

public:
    Fondo(const std::string& textureFile, float scrollSpeed) : _scrollSpeed(scrollSpeed) {
        if (!_texture.loadFromFile(textureFile)) {
            std::cerr << "Error cargando textura de fondo: " << textureFile << std::endl;
        }
        _texture.setRepeated(true);
        
        _sprite1.setTexture(_texture);
        _sprite2.setTexture(_texture);
        
        configurarSprites();
    }

    void configurarSprites() {
        float scaleX = SCREEN_WIDTH / _texture.getSize().x;
        float scaleY = SCREEN_HEIGHT / _texture.getSize().y;
        float scale = std::max(scaleX, scaleY);
        
        _sprite1.setScale(scale, scale);
        _sprite2.setScale(scale, scale);
        
        int textureWidth = static_cast<int>(SCREEN_WIDTH / scale) + 1; // +1 para evitar huecos
        _sprite1.setTextureRect(sf::IntRect(0, 0, textureWidth, _texture.getSize().y));
        _sprite2.setTextureRect(sf::IntRect(0, 0, textureWidth, _texture.getSize().y));
        
        _sprite1.setPosition(0, 0);
        _sprite2.setPosition(SCREEN_WIDTH, 0);
    }
    void update(float deltaTime) {
        float movement = _scrollSpeed * deltaTime;
        _sprite1.move(-movement, 0);
        _sprite2.move(-movement, 0);

        if (_sprite1.getPosition().x <= -SCREEN_WIDTH) {
            _sprite1.setPosition(_sprite2.getPosition().x + SCREEN_WIDTH, 0);
        }
        
        if (_sprite2.getPosition().x <= -SCREEN_WIDTH) {
            _sprite2.setPosition(_sprite1.getPosition().x + SCREEN_WIDTH, 0);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(_sprite1);
        window.draw(_sprite2);
    }

    void cambiarFondo(const std::string& nuevoFondo) {
    if (!_texture.loadFromFile(nuevoFondo)) {
        std::cerr << "Error cargando nueva textura de fondo: " << nuevoFondo << std::endl;
    }
    _texture.setRepeated(true);
    
    float scale = SCREEN_HEIGHT / _texture.getSize().y;
    int textureWidth = static_cast<int>(SCREEN_WIDTH / scale);
    
    _sprite1.setTexture(_texture);
    _sprite2.setTexture(_texture);
    _sprite1.setScale(scale, scale);
    _sprite2.setScale(scale, scale);
    _sprite1.setTextureRect(sf::IntRect(0, 0, textureWidth, _texture.getSize().y));
    _sprite2.setTextureRect(sf::IntRect(0, 0, textureWidth, _texture.getSize().y));
    
    // Reiniciar las posiciones de los sprites
    _sprite1.setPosition(0, 0);
    _sprite2.setPosition(0, SCREEN_WIDTH);
    }
};

#endif