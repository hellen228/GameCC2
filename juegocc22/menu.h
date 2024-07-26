#include <SFML/Graphics.hpp>
#include "nivel.h"

using namespace std;
using namespace sf;

const int NUM_OPTIONS = 2;
const Color SELECTED_COLOR = Color{255, 204, 0};
const Color DEFAULT_COLOR = Color::White;

class Menu {
private:
    int selected;
    Font font;
    Text mainmenu[NUM_OPTIONS];
public:
    Menu(float width, float height);
    void draw(RenderWindow& window);
    void MoveUp();
    void MoveDown();
    void setSelected(int n);
    int pressed() const {
        return selected;
    }
    ~Menu() = default;  // Puedes omitir el destructor si no tiene ninguna lógica
};

Menu::Menu(float width, float height) {
    if (!font.loadFromFile("font/fontMenu.ttf")) {
        // Manejo de error si la fuente no se carga
    }

    for (int i = 0; i < NUM_OPTIONS; ++i) {
        mainmenu[i].setFont(font);
        mainmenu[i].setFillColor(DEFAULT_COLOR);
        mainmenu[i].setCharacterSize(13);
    }

    mainmenu[0].setString("Play");
    mainmenu[0].setPosition(Vector2f(25, height / 2 + 30));

    mainmenu[1].setString("Exit");
    mainmenu[1].setPosition(Vector2f(25, height / 2 + 50));

    selected = 0;
    mainmenu[selected].setFillColor(SELECTED_COLOR);
}

void Menu::setSelected(int n) {
    selected = n;
}

void Menu::draw(RenderWindow &window) {
    for (int i = 0; i < NUM_OPTIONS; ++i) {
        window.draw(mainmenu[i]);
    }
}

void Menu::MoveDown() {
    mainmenu[selected].setFillColor(DEFAULT_COLOR);
    selected = (selected + 1) % NUM_OPTIONS;
    mainmenu[selected].setFillColor(SELECTED_COLOR);
}

void Menu::MoveUp() {
    mainmenu[selected].setFillColor(DEFAULT_COLOR);
    selected = (selected - 1 + NUM_OPTIONS) % NUM_OPTIONS;
    mainmenu[selected].setFillColor(SELECTED_COLOR);
}
