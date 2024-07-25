#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include <string>

class Item {
private:
    sf::Sprite _sprite;
    sf::Texture _texture;

public:
    Item(const std::string& textureFile) {
        if (!_texture.loadFromFile(textureFile)) {
            std::cerr << "Error cargando textura de item: " << textureFile << std::endl;
            std::exit(EXIT_FAILURE);
        }

        _sprite.setTexture(_texture);
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
        window.draw(_sprite);
    }
};

#endif 
