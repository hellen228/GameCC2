#ifndef COLISION_H
#define COLISION_H
#include <SFML/Graphics.hpp>
#include "obstaculo.h"
#include "personaje.h"

class ColisionObserver {
public:
    virtual void onColision(Personaje* jugador, Obstaculo* obstaculo) = 0;
};

class ColisionManager : public ColisionObserver {
public:
    void onColision(Personaje* jugador, Obstaculo* obstaculo) override {
        obstaculo->interactuar(jugador);
    }
};

#endif