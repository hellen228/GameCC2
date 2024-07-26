#ifndef NIVEL_H
#define NIVEL_H

#include <SFML/Graphics.hpp>
#include "fondo.h"
#include "plataforma.h"
#include "item.h"
#include "personaje.h"
#include "floor.h"
#include <vector>
#include <string>
#include <iostream>

class Nivel {
private:
    Fondo _fondo;
    std::vector<Plataforma> _plataformas;
    Item _llave;
    PlataformaFactory _plataformaFactory;
    int _nivelActual;
    std::vector<std::string> _fondos;
    int _llavesJugador1;
    int _llavesJugador2;
    sf::Font _font;
    const float SCREEN_WIDTH = 400.0f;
    const float SCREEN_HEIGHT = 224.0f;
    const float FLOOR_HEIGHT = 10.0f;
    Personaje* _personaje1;
    Personaje* _personaje2;
    Floor _piso;
    Floor _techo;
    sf::Text _marcadorText;
    sf::Text _marcadorText2;

public:
    Nivel(const std::string& fondoInicial, const std::string& texturaLlave, Personaje* p1, Personaje* p2)
        : _fondo(fondoInicial, 35.0f), _llave(texturaLlave), _nivelActual(1),
          _llavesJugador1(0), _llavesJugador2(0),
          _personaje1(p1), _personaje2(p2),
          _piso(SCREEN_WIDTH, FLOOR_HEIGHT, 0, SCREEN_HEIGHT - FLOOR_HEIGHT, "picture/floor_texture.png"),
          _techo(SCREEN_WIDTH, FLOOR_HEIGHT, 0, 0, "picture/floor_texture.png", true) {
        
        _fondos = {"picture/fondo1.jpg", "picture/fondo2.jpg", "picture/fondo3.jpg"};
        
        if (!_font.loadFromFile("font/fuente.ttf")) {
            throw std::runtime_error("Error cargando fuente");
        }
        
        configurarTexto(_marcadorText, 8, 10, 5);
        configurarTexto(_marcadorText2, 8, 10, 20);
        
        inicializarNivel();
    }

    void inicializarNivel() {
        _plataformas = _plataformaFactory.createPlataformas(5, _nivelActual, SCREEN_HEIGHT);
        _llave.reset();
        reiniciarPosicionesPersonajes();
    }

    void update(float deltaTime) {
        _fondo.update(deltaTime);
        _plataformaFactory.updatePlataformas(_plataformas, SCREEN_WIDTH, deltaTime);
        _llave.update(SCREEN_WIDTH, SCREEN_HEIGHT);

        if (_personaje1) _personaje1->update(_plataformas, deltaTime);
        if (_personaje2) _personaje2->update(_plataformas, deltaTime);

        checkLlaveCollision();
        actualizarMarcador();
    }

    void configurarTexto(sf::Text& texto, unsigned int tamanio, float x, float y) {
        texto.setFont(_font);
        texto.setCharacterSize(tamanio);
        texto.setPosition(x, y);
        
        const_cast<sf::Texture&>(texto.getFont()->getTexture(tamanio)).setSmooth(false);
    }

    void actualizarMarcador() {
        _marcadorText.setString("Jugador 1: " + std::to_string(_llavesJugador1));
        _marcadorText2.setString("Jugador 2: " + std::to_string(_llavesJugador2));
    }

    void draw(sf::RenderWindow& window) {
        _fondo.draw(window);
        for (auto& plataforma : _plataformas) {
            plataforma.draw(window);
        }
        _piso.draw(window);
        _techo.draw(window);
        _llave.draw(window);
        if (_personaje1) _personaje1->draw(window);
        if (_personaje2) _personaje2->draw(window);
        window.draw(_marcadorText);
        window.draw(_marcadorText2);

        // Debug: Imprimir número de plataformas
        //std::cout << "Número de plataformas: " << _plataformas.size() << std::endl;
    }

    void checkLlaveCollision() {
        if (_llave.isVisible()) {
            if (_personaje1 && _personaje1->getGlobalBounds().intersects(_llave.getGlobalBounds())) {
                _llavesJugador1++;
                avanzarNivel();
            } else if (_personaje2 && _personaje2->getGlobalBounds().intersects(_llave.getGlobalBounds())) {
                _llavesJugador2++;
                avanzarNivel();
            }
        }
    }

    void avanzarNivel() {
        _nivelActual++;
        if (_nivelActual > _fondos.size()) {
            _nivelActual = 1;
        }
        
        _fondo.cambiarFondo(_fondos[_nivelActual - 1]);
        inicializarNivel();
    }

    bool hayGanador() const {
        return _llavesJugador1 >= 3 || _llavesJugador2 >= 3;
    }

    int getGanador() const {
        if (_llavesJugador1 >= 3) return 1;
        if (_llavesJugador2 >= 3) return 2;
        return 0;
    }

    void reiniciarPosicionesPersonajes() {
        if (_personaje1) {
            _personaje1->setPosition(50, SCREEN_HEIGHT - _personaje1->getGlobalBounds().height - FLOOR_HEIGHT);
            _personaje1->setInverted(false);
        }
        if (_personaje2) {
            _personaje2->setPosition(50, 0);
            _personaje2->setInverted(true);
        }
    }
};

#endif // NIVEL_H