#include <SFML/Graphics.hpp>
#include "nivel.h"
using namespace std;
using namespace sf;

class Menu{
private:
    int selected;
    Font font;
public:
    Text mainmenu[3];
    Menu(float width, float height);
    void draw(RenderWindow& window);
    void MoveUp();
    void MoveDown();
    void setSelected(int n);
    int pressed(){
        return selected;
    }
    ~Menu();
};

/*------------------- menu.cpp --------------------*/

Menu::Menu(float width, float height){
    font.loadFromFile("font/fontMenu.ttf");

    mainmenu[0].setFont(font);
    mainmenu[0].setFillColor(Color{255,204,0}); 
    mainmenu[0].setString("Play");
    mainmenu[0].setCharacterSize(13);
    mainmenu[0].setPosition(Vector2f(25,height/(2)+30));

    mainmenu[1].setFont(font);
    mainmenu[1].setFillColor(Color{255,204,0});
    mainmenu[1].setString("Instruction");
    mainmenu[1].setCharacterSize(13);
    mainmenu[1].setPosition(Vector2f(25,height/(2)+50));

    mainmenu[2].setFont(font);
    mainmenu[2].setFillColor(Color{255,204,0});
    mainmenu[2].setString("Exit");
    mainmenu[2].setCharacterSize(13);
    mainmenu[2].setPosition(Vector2f(25,height/(2)+70));

    selected=0;
}

Menu::~Menu(){}

void Menu::setSelected(int n){
    selected=n;
}

void Menu::draw(RenderWindow &window){
    for(int i=0; i<3; i++){
        window.draw(mainmenu[i]);
    }
}

void Menu::MoveDown(){
    if(selected +1 <=3){ //not exit
        mainmenu[selected].setFillColor(Color::White);
        selected++;
        if(selected==3){
            selected = 0;
        }
        mainmenu[selected].setFillColor(Color{255, 204, 0});
    }
}

void Menu::MoveUp(){
    if(selected-1>=-1){ //not play
        mainmenu[selected].setFillColor(Color::White);
        selected--;
        if(selected==-1){
            selected=2;
        }
        mainmenu[selected].setFillColor(Color{255, 204, 0});
    }
}

