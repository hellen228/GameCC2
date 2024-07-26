#ifndef FLOOR_H
#define FLOOR_H

#include <SFML/Graphics.hpp>
#include <string>

class Floor {
private:
    sf::RectangleShape _shape;
    sf::Texture _texture;
    bool _isCeiling;

public:
    Floor(float width, float height, float posX, float posY, const std::string& textureFile, bool isCeiling = false) 
        : _isCeiling(isCeiling) {
        _shape.setSize(sf::Vector2f(width, height));
        _shape.setPosition(posX, posY);
        if (_texture.loadFromFile(textureFile)) {
            _texture.setRepeated(true);
            _shape.setTexture(&_texture);
            
            _shape.setTextureRect(sf::IntRect(0, 0, width, height));
            
            if (_isCeiling) {
                _shape.setScale(1, -1);
                _shape.setPosition(posX, posY + height);
            }
        } else {
            _shape.setFillColor(sf::Color::White);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(_shape);
    }

    sf::FloatRect getGlobalBounds() const {
        return _shape.getGlobalBounds();
    }
};

#endif // FLOOR_H