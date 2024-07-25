#include <SFML/Graphics.hpp>
#include "personaje.h"
#include "fondo.h"
#include "plataforma.h"
#include "floor.h"
#include "item.h"


using namespace std;
using namespace sf;

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 224), "Juego CC");
    const float FLOOR_HEIGHT = 10.0f;
    Fondo fondo("picture/fondo1.jpg", 0.1f); 
    
    Personaje personaje1("picture/sprite_sheet1.png", 0.1f,
                         sf::Keyboard::A, sf::Keyboard::D,
                         sf::Keyboard::W, sf::Keyboard::S,
                         false);  // Comienza en el suelo

    Personaje personaje2("picture/sprite_sheet2.png", 0.1f,
                         sf::Keyboard::Left, sf::Keyboard::Right,
                         sf::Keyboard::Up, sf::Keyboard::Down,
                         true);   // Comienza en el techo

    // Posicionar los personajes
    // Ajusta la posición inicial del personaje1 (en el suelo)
    personaje1.setPosition(50, 224 - personaje1.getGlobalBounds().height - FLOOR_HEIGHT);

    // Ajusta la posición inicial del personaje2 (en el techo)
    personaje2.setPosition(50, FLOOR_HEIGHT);
    Item llave("llave1.png");
    llave.setPosition(350, 100);

    Floor floor(400, FLOOR_HEIGHT, 0, 224 - FLOOR_HEIGHT, "floor_texture.png");
    Floor ceiling(400, FLOOR_HEIGHT, 0, 0, "floor_texture.png", true);
    // Guardar las posiciones iniciales de los personajes
    sf::Vector2f posInicialPersonaje1 = personaje1.getPosition();
    sf::Vector2f posInicialPersonaje2 = personaje2.getPosition();

    PlataformaFactory factory;
    std::vector<Plataforma> plataformas = factory.createInitialPlataformas(8, 400, 224);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        fondo.update();

        factory.updatePlataformas(plataformas, 400, 224);
        personaje1.update(plataformas, deltaTime.asSeconds());
        personaje2.update(plataformas, deltaTime.asSeconds());

        window.clear();
        fondo.draw(window);
        floor.draw(window);
        ceiling.draw(window);
        
        sf::FloatRect personaje1Bounds = personaje1.getGlobalBounds();
        sf::FloatRect personaje2Bounds = personaje2.getGlobalBounds();
        sf::FloatRect llaveBounds = llave.getGlobalBounds();

        if (personaje1Bounds.intersects(llaveBounds) || personaje2Bounds.intersects(llaveBounds)) {
            fondo.cambiarFondo("picture/fondo2.jpg");
            // Restablecer posiciones iniciales de los personajes
            personaje1.setPosition(50, 204 - personaje1.getGlobalBounds().height - FLOOR_HEIGHT); // 20 es la altura del piso
            personaje2.setPosition(50, FLOOR_HEIGHT); // 20 es la altura del techo
        }    

        for (auto& plataforma : plataformas) {
            plataforma.draw(window);
        }
        personaje1.draw(window);
        personaje2.draw(window);
        llave.draw(window);

        window.display();
    }

    return 0;
}