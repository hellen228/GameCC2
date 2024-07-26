#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "plataforma.h"

class Personaje {
private:
    sf::Sprite _sprite;
    sf::Texture _texture;
    float _velocity;
    float _verticalSpeed;
    float _gravity;
    float _normalGravity;
    float _invertedGravity;
    bool _isGravityInverted;
    sf::Keyboard::Key _leftKey, _rightKey, _upKey, _downKey;
    float _frameTime;
    float _elapsedTime;
    bool _isMoving;
    bool _isFacingRight;
    std::vector<sf::IntRect> _frames;
    int _currentFrameIndex;
    bool _isInverted;
    bool _startInverted;
    static constexpr float FLOOR_HEIGHT = 10.0f;
    static constexpr float SCREEN_WIDTH = 400.0f;
    static constexpr float SCREEN_HEIGHT = 224.0f;

public:
    Personaje(const std::string& textureFile, float velocity,
              sf::Keyboard::Key left, sf::Keyboard::Key right,
              sf::Keyboard::Key up, sf::Keyboard::Key down,
              bool startInverted)
        : _velocity(velocity), _leftKey(left), _rightKey(right), _upKey(up), _downKey(down),
          _verticalSpeed(0.05f), _normalGravity(0.5f), _invertedGravity(-0.5f),
          _isGravityInverted(startInverted), _isInverted(startInverted),
          _startInverted(startInverted),
          _frameTime(0.05f), _elapsedTime(0.0f), _isMoving(false), _isFacingRight(true), _currentFrameIndex(0)
    {
        if (!_texture.loadFromFile(textureFile)) {
            throw std::runtime_error("Error cargando textura: " + textureFile);
        }
        _sprite.setTexture(_texture);
        loadFrames();
        _sprite.setTextureRect(_frames[0]);
        _sprite.setScale(1.0f, 1.0f);
        _gravity = _startInverted ? _invertedGravity : _normalGravity;
    }

    void update(const std::vector<Plataforma>& plataformas, float deltaTime) {
        sf::Vector2f oldPosition = _sprite.getPosition();
        _isMoving = false;

        if (sf::Keyboard::isKeyPressed(_leftKey)) {
            _sprite.move(-_velocity * deltaTime, 0.0f);
            _isMoving = true;
            _isFacingRight = false;
        }
        if (sf::Keyboard::isKeyPressed(_rightKey)) {
            _sprite.move(_velocity * deltaTime, 0.0f);
            _isMoving = true;
            _isFacingRight = true;
        }

        if (sf::Keyboard::isKeyPressed(_upKey)) {
            _gravity = _invertedGravity;
            _isGravityInverted = true;
            _isInverted = true;
        }
        if (sf::Keyboard::isKeyPressed(_downKey)) {
            _gravity = _normalGravity;
            _isGravityInverted = false;
            _isInverted = false;
        }

        _verticalSpeed += _gravity;
        _sprite.move(0.001f, _verticalSpeed);

        bool onGround = false;
        for (const auto& plataforma : plataformas) {
            if (checkCollision(plataforma)) {
                handleCollision(plataforma);
                onGround = true;
            }
        }

        sf::Vector2f position = _sprite.getPosition();
        sf::FloatRect bounds = _sprite.getGlobalBounds();

        if (position.x < 0) {
            position.x = 0;
        } else if (position.x > SCREEN_WIDTH - bounds.width) {
            position.x = SCREEN_WIDTH - bounds.width;
        }

        if (_isGravityInverted) {
            if (position.y <= 0) {
                position.y = 0;
                _verticalSpeed = 0;
                onGround = true;
            }
        } else {
            if (position.y >= SCREEN_HEIGHT - bounds.height - FLOOR_HEIGHT) {
                position.y = SCREEN_HEIGHT - bounds.height - FLOOR_HEIGHT;
                _verticalSpeed = 0;
                onGround = true;
            }
        }

        _sprite.setPosition(position);

        updateAnimation(deltaTime);
    }

    void draw(sf::RenderWindow& window) {
        sf::Sprite tempSprite = _sprite;
        if (_isInverted) {
            tempSprite.setScale(tempSprite.getScale().x, -std::abs(tempSprite.getScale().y));
            tempSprite.setPosition(tempSprite.getPosition().x, 
                                   tempSprite.getPosition().y + tempSprite.getGlobalBounds().height - FLOOR_HEIGHT);
        } else {
            tempSprite.setScale(tempSprite.getScale().x, std::abs(tempSprite.getScale().y));
        }
        window.draw(tempSprite);
    }

    sf::Vector2f getPosition() const { return _sprite.getPosition(); }
    sf::FloatRect getGlobalBounds() const { return _sprite.getGlobalBounds(); }
    void setPosition(float x, float y) { _sprite.setPosition(x, y); }
    bool checkCollision(const Plataforma& plataforma) { return _sprite.getGlobalBounds().intersects(plataforma.getGlobalBounds()); }
    void setInverted(bool inverted) {
        _isInverted = inverted;
        _isGravityInverted = inverted;
        _gravity = inverted ? _invertedGravity : _normalGravity;
    }

private:
    void loadFrames() {
        _frames = {
            sf::IntRect(0, 0, 16, 24),
            sf::IntRect(0, 24, 16, 24),
            sf::IntRect(16, 24, 16, 24),
            sf::IntRect(32, 24, 16, 24),
            sf::IntRect(48, 24, 16, 24),
            sf::IntRect(0, 48, 16, 24),
            sf::IntRect(16, 48, 16, 24),
            sf::IntRect(32, 48, 16, 24),
            sf::IntRect(48, 48, 16, 24)
        };
    }

    void updateAnimation(float deltaTime) {
        _elapsedTime += deltaTime;
        if (_elapsedTime >= _frameTime) {
            _elapsedTime = 0.0f;
            _currentFrameIndex = (_currentFrameIndex + 1) % _frames.size();
            _sprite.setTextureRect(_frames[_currentFrameIndex]);
        }

        float scaleX = std::abs(_sprite.getScale().x) * (_isFacingRight ? 1 : -1);
        float scaleY = std::abs(_sprite.getScale().y) * (_isInverted ? -1 : 1);
        _sprite.setScale(scaleX, scaleY);

        if (_isInverted) {
            _sprite.setOrigin(0, _sprite.getLocalBounds().height);
        } else {
            _sprite.setOrigin(0, 0);
        }
    }

    void handleCollision(const Plataforma& plataforma) {
        sf::FloatRect personajeBounds = _sprite.getGlobalBounds();
        sf::FloatRect plataformaBounds = plataforma.getGlobalBounds();

        if ((_verticalSpeed > 0 && !_isGravityInverted) || 
            (_verticalSpeed < 0 && _isGravityInverted)) {
            if (!_isGravityInverted) {
                _sprite.setPosition(personajeBounds.left, plataformaBounds.top - personajeBounds.height);
            } else {
                _sprite.setPosition(personajeBounds.left, plataformaBounds.top + plataformaBounds.height);
            }
            _verticalSpeed = 0;
        }
        else if (getPosition().x + personajeBounds.width <= plataformaBounds.left) {
            _sprite.setPosition(plataformaBounds.left - personajeBounds.width, personajeBounds.top);
        }
        else if (getPosition().x >= plataformaBounds.left + plataformaBounds.width) {
            _sprite.setPosition(plataformaBounds.left + plataformaBounds.width, personajeBounds.top);
        }
    }
};

#endif // PERSONAJE_H