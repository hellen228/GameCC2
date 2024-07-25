#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "plataforma.h"

class Personaje {
private:
    sf::Sprite _sprite;
    sf::Texture _texture;
    sf::IntRect _frameRect;
    sf::Vector2u _sheetSize;
    sf::Vector2u _frameSize;
    float _velocity;
    float _verticalSpeed;
    float _gravity;
    float _normalGravity;
    float _invertedGravity;
    bool _isGravityInverted;
    sf::Keyboard::Key _leftKey, _rightKey, _upKey, _downKey;
    bool _perdio = false;
    bool _fueraDeLimites = false;
    std::string _mensajePerdio;
    int _currentFrame;
    float _frameTime;
    float _elapsedTime;
    bool _isMoving;
    bool _isFacingRight;
    std::vector<sf::IntRect> _frames;
    int _currentFrameIndex;
    bool _isInverted;
    bool _startInverted;
    const float FLOOR_HEIGHT = 10.0f;
public:
    Personaje(const std::string& textureFile, float velocity,
              sf::Keyboard::Key left, sf::Keyboard::Key right,
              sf::Keyboard::Key up, sf::Keyboard::Key down,
              bool startInverted);
    void update(const std::vector<Plataforma>& plataformas, float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;
    void setPosition(float x, float y);
    bool checkCollision(const Plataforma& plataforma);
    bool isPerdido() const { return _perdio; }
    void updateAnimation(float deltaTime);
    void loadFrames();
};
Personaje::Personaje(const std::string& textureFile, float velocity,
                     sf::Keyboard::Key left, sf::Keyboard::Key right,
                     sf::Keyboard::Key up, sf::Keyboard::Key down,
                     bool startInverted)
    : _velocity(velocity), _leftKey(left), _rightKey(right), _upKey(up), _downKey(down),
      _verticalSpeed(0.0f), _normalGravity(0.5f), _invertedGravity(-0.5f),
      _isGravityInverted(startInverted), _isInverted(startInverted),
      _startInverted(startInverted),
      _frameTime(0.1f), _elapsedTime(0.0f), _isMoving(false), _isFacingRight(true), _currentFrameIndex(0)
{
    if (!_texture.loadFromFile(textureFile)) {
        std::cerr << "Error cargando textura: " << textureFile << std::endl;
    }
    _sprite.setTexture(_texture);
    loadFrames();
    _sprite.setTextureRect(_frames[0]);
    _sprite.setScale(1.0f, 1.0f); // Ajusta este valor según necesites
    if (_startInverted) {
        _gravity = _invertedGravity;
    } else {
        _gravity = _normalGravity;
    }
}
void Personaje::loadFrames() {
    // Definir las coordenadas de cada frame en el sprite sheet
    _frames = {
        sf::IntRect(0, 0, 16, 24),    // Frame 1 (arriba izquierda)
        sf::IntRect(0, 24, 16, 24),   // Frame 2
        sf::IntRect(16, 24, 16, 24),   // Frame 3
        sf::IntRect(32, 24, 16, 24),   // Frame 4
        sf::IntRect(48, 24, 16, 24),   // Frame 5
        sf::IntRect(0, 48, 16, 24),  // Frame 6
        sf::IntRect(16, 48, 16, 24),  // Frame 7
        sf::IntRect(32, 48, 16, 24),  // Frame 8
        sf::IntRect(48, 48, 16, 24)    // Frame 9
    };
    
    // Ajustar el tamaño del sprite al tamaño de un frame individual
    _sprite.setTextureRect(_frames[0]);
    // Puedes ajustar la escala si los sprites son muy pequeños
    _sprite.setScale(1.0f, 1.0f); // Esto duplicará el tamaño del sprite
}
void Personaje::updateAnimation(float deltaTime) {
    _elapsedTime += deltaTime;
    if (_elapsedTime >= _frameTime) {
        _elapsedTime = 0.0f;
        _currentFrameIndex = (_currentFrameIndex + 1) % _frames.size();
        _sprite.setTextureRect(_frames[_currentFrameIndex]);
    }

    float scaleX = std::abs(_sprite.getScale().x) * (_isFacingRight ? 1 : -1);
    float scaleY = std::abs(_sprite.getScale().y) * (_isInverted ? -1 : 1);
    _sprite.setScale(scaleX, scaleY);

    // Ajustar la posición del sprite cuando está invertido para que los pies sigan en la misma posición
    if (_isInverted) {
        _sprite.setOrigin(0, _sprite.getLocalBounds().height);
    } else {
        _sprite.setOrigin(0, 0);
    }
}
void Personaje::update(const std::vector<Plataforma>& plataformas, float deltaTime) {

    if (!_perdio) {
        sf::Vector2f oldPosition = _sprite.getPosition();
        _isMoving = false;

        if (sf::Keyboard::isKeyPressed(_leftKey)) {
            _sprite.move(-_velocity, 0.0f);
            _isMoving = true;
            _isFacingRight = false;
        }
        if (sf::Keyboard::isKeyPressed(_rightKey)) {
            _sprite.move(_velocity, 0.0f);
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

        // Aplicar gravedad
        _verticalSpeed += _gravity;
        _sprite.move(0.001f, _verticalSpeed);

        // Comprobar colisiones con plataformas
        bool onGround = false;
        for (const auto& plataforma : plataformas) {
            if (checkCollision(plataforma)) {
                // Colisión detectada
                sf::FloatRect personajeBounds = _sprite.getGlobalBounds();
                sf::FloatRect plataformaBounds = plataforma.getGlobalBounds();

                // Colisión vertical
                if ((_verticalSpeed > 0 && !_isGravityInverted) || 
                    (_verticalSpeed < 0 && _isGravityInverted)) {
                    if (!_isGravityInverted) {
                        _sprite.setPosition(personajeBounds.left, plataformaBounds.top - personajeBounds.height);
                    } else {
                        _sprite.setPosition(personajeBounds.left, plataformaBounds.top + plataformaBounds.height);
                    }
                    _verticalSpeed = 0;
                    onGround = true;
                }
                // Colisión horizontal (opcional)
                else if (oldPosition.x + personajeBounds.width <= plataformaBounds.left) {
                    _sprite.setPosition(plataformaBounds.left - personajeBounds.width, personajeBounds.top);
                }
                else if (oldPosition.x >= plataformaBounds.left + plataformaBounds.width) {
                    _sprite.setPosition(plataformaBounds.left + plataformaBounds.width, personajeBounds.top);
                }
            }
        }
    // Comprobar colisión con el techo
        if (_isGravityInverted && _sprite.getPosition().y <= FLOOR_HEIGHT) {
            _sprite.setPosition(_sprite.getPosition().x, FLOOR_HEIGHT);
            _verticalSpeed = 0;
            onGround = true;
        }

        // Comprobar colisión con el suelo
        if (!_isGravityInverted && _sprite.getPosition().y + _sprite.getGlobalBounds().height >= 224 - FLOOR_HEIGHT) {
            _sprite.setPosition(_sprite.getPosition().x, 224 - _sprite.getGlobalBounds().height - FLOOR_HEIGHT);
            _verticalSpeed = 0;
            onGround = true;
        }
        
        if (_isGravityInverted) {
            if (_sprite.getPosition().y < FLOOR_HEIGHT) {
                _sprite.setPosition(_sprite.getPosition().x, FLOOR_HEIGHT);
                _verticalSpeed = 0;
            }
        } else {
            if (_sprite.getPosition().y + _sprite.getGlobalBounds().height > 224 - FLOOR_HEIGHT) {
                _sprite.setPosition(_sprite.getPosition().x, 224 - _sprite.getGlobalBounds().height - FLOOR_HEIGHT);
                _verticalSpeed = 0;
            }
        }
        // Límites de la pantalla
        sf::Vector2f position = _sprite.getPosition();
    sf::FloatRect bounds = _sprite.getGlobalBounds();

    if (position.x < 0) {
        position.x = 0;
    } else if (position.x > 400 - bounds.width) {
        position.x = 400 - bounds.width;
    }
    if (_isGravityInverted) {
        if (position.y < FLOOR_HEIGHT) {
            position.y = FLOOR_HEIGHT;
        }
    } else {
        if (position.y > 224 - bounds.height - FLOOR_HEIGHT) {
            position.y = 224 - bounds.height - FLOOR_HEIGHT;
        }
    }

    _sprite.setPosition(position);

        // Actualizar estado de fuera de límites
        if (position.x == 0 || position.x == 400 - bounds.width ||
            position.y == 0 || position.y == 224 - bounds.height) {
            if (!_fueraDeLimites) {
                _fueraDeLimites = true;
                _mensajePerdio = "Jugador perdio por tocar el limite de la pantalla.";
                std::cout << _mensajePerdio << std::endl;
            }
        } else {
            _fueraDeLimites = false;
        }

        updateAnimation(deltaTime);
    }
}

void Personaje::draw(sf::RenderWindow& window) {
    if (!_perdio) {
        sf::Sprite tempSprite = _sprite;
        if (_isInverted) {
            tempSprite.setScale(tempSprite.getScale().x, -std::abs(tempSprite.getScale().y));
            tempSprite.setPosition(tempSprite.getPosition().x, 
                                   tempSprite.getPosition().y + tempSprite.getGlobalBounds().height);
        } else {
            tempSprite.setScale(tempSprite.getScale().x, std::abs(tempSprite.getScale().y));
        }
        window.draw(tempSprite);
    }
}
sf::Vector2f Personaje::getPosition() const {
    return _sprite.getPosition();
}

sf::FloatRect Personaje::getGlobalBounds() const {
    return _sprite.getGlobalBounds();
}

void Personaje::setPosition(float x, float y) {
    _sprite.setPosition(x, y);
}

bool Personaje::checkCollision(const Plataforma& plataforma) {
    return _sprite.getGlobalBounds().intersects(plataforma.getGlobalBounds());
}

#endif // PERSONAJE_H