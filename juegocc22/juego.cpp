#include <SFML/Graphics.hpp>
#include "personaje.h"
#include "fondo.h"
#include "plataforma.h"
#include "floor.h"
#include "item.h"
#include "nivel.h"
#include "menu.h"
#include <iostream>

int pagenum = 1000;  // Controla la página actual del menú
sf::Sprite bg;
bool juegoTerminado = false; // Variable para controlar el estado del juego
bool shouldExit = false; // Controla si el juego debería cerrarse
int ganador = -1; // Variable para almacenar el ganador

void handleMenu(sf::RenderWindow& window, Menu& menu, Nivel& nivel);
void mostrarGanador(sf::RenderWindow& window, int ganador);

int main() {
    try {
        sf::RenderWindow window(sf::VideoMode(400, 224), "Juego CC");
        sf::Texture mianmenubg;
        if (!mianmenubg.loadFromFile("picture/background-menu.jpg")) {
            std::cerr << "Error cargando la imagen de fondo del menú" << std::endl;
            return 1;
        }
        bg.setTexture(mianmenubg);

        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(60);

        Menu menu(window.getSize().x, window.getSize().y);

        Personaje personaje1("picture/sprite_sheet1.png", 50.0f,
                             sf::Keyboard::A, sf::Keyboard::D,
                             sf::Keyboard::W, sf::Keyboard::S,
                             false);

        Personaje personaje2("picture/sprite_sheet2.png", 50.0f,
                             sf::Keyboard::Left, sf::Keyboard::Right,
                             sf::Keyboard::Up, sf::Keyboard::Down,
                             true);

        Nivel nivel("picture/fondo1.jpg", "picture/llave1.png", &personaje1, &personaje2);
        nivel.reiniciarPosicionesPersonajes();

        while (window.isOpen()) {
            if (pagenum == 1000) {
                handleMenu(window, menu, nivel);
            } else if (pagenum == 0) {
                sf::Clock clock;
                while (window.isOpen()) {
                    sf::Time deltaTime = clock.restart();

                    sf::Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                    }

                    nivel.update(deltaTime.asSeconds());

                    if (nivel.hayGanador()) {
                        ganador = nivel.getGanador();
                        // Muestra el mensaje en el hilo principal
                        mostrarGanador(window, ganador);

                        // Sal del bucle principal
                        break;
                    }

                    window.clear();
                    nivel.draw(window);
                    window.display();
                }
            }

            if (shouldExit) {
                break; // Sal del bucle principal si shouldExit es verdadero
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

void handleMenu(sf::RenderWindow& window, Menu& menu, Nivel& nivel) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return; // Termina la función handleMenu si se cierra la ventana
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    menu.MoveUp();
                }
                if (event.key.code == sf::Keyboard::Down) {
                    menu.MoveDown();
                }
                if (event.key.code == sf::Keyboard::Return) {
                    if (menu.pressed() == 0) {
                        pagenum = 0; // Cambia a la página de juego
                        return; // Sal de la función handleMenu después de seleccionar "Play"
                    }
                    if (menu.pressed() == 1) {
                        shouldExit = true; // Marca que el juego debe cerrarse
                        window.close(); // Cierra la ventana
                        return; // Termina la función handleMenu
                    }
                }
            }
        }

        window.clear();
        if (pagenum != 1000) {
            break; // Sal de la función handleMenu si pagenum no es 1000
        }
        window.draw(bg);
        menu.draw(window);
        window.display();
    }
}

void mostrarGanador(sf::RenderWindow& window, int ganador) {
    sf::Font font;
    if (!font.loadFromFile("font/fontMenu.ttf")) {
        std::cerr << "Error cargando la fuente" << std::endl;
        return;
    }

    sf::Text texto1;
    texto1.setFont(font);
    texto1.setString("Felicidades ");
    texto1.setCharacterSize(9); // Tamaño de fuente para la primera línea
    texto1.setFillColor(sf::Color::White);
    texto1.setStyle(sf::Text::Bold);
    texto1.setPosition(143, 60); // Posición en la pantalla (arriba)

    // Segunda línea de texto
    sf::Text texto2;
    texto2.setFont(font);
    texto2.setString("El Jugador " + std::to_string(ganador) + " ha escapado.");
    texto2.setCharacterSize(9); // Tamaño de fuente para la segunda línea
    texto2.setFillColor(sf::Color::White);
    texto2.setStyle(sf::Text::Bold);
    texto2.setPosition(70, 100);

    sf::Clock clock;
    while (clock.getElapsedTime().asSeconds() < 10) {
        window.clear();
        window.draw(texto1);
        window.draw(texto2);
        window.display();
        sf::sleep(sf::milliseconds(100)); // Permite a la ventana procesar eventos
    }

    window.close();
}
