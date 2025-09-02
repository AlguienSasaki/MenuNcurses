#include "MenuNcurses.hpp"

MenuNcurses::MenuNcurses(const string &title, const vector<string> &options) //constructor
    : title(title), options(options) {
    construirVentana();
}

void MenuNcurses::construirVentana(bool interactivo) { // Parametros de la ventana a crear y su creacion
    int height = options.size() + (interactivo ? 2 : 4);
    //startY = 40;
    //startX = 80;
    startY = (LINES - height) / 2;
    startX = (COLS - width) / 2;
    win = newwin(height, width, startY, startX);
    keypad(win, interactivo);
}

void MenuNcurses::dibujar(int highlight, bool interactivo) { //se encarga de hacer los cambios de colores en el resaltado
    werase(win);
    box(win, 0, 0); //dibuja el marco
    if (!title.empty())
        mvwprintw(win, 0, 2, (" " + title + " ").c_str());

    for (int i = 0; i < options.size(); ++i) {
        if (interactivo && i == highlight)
            wattron(win, A_REVERSE);
        mvwprintw(win, i + 1, 2, options[i].c_str());
        if (interactivo && i == highlight)
            wattroff(win, A_REVERSE);
    }
    if (!interactivo)
        mvwprintw(win, options.size() + 2, 2, "Presiona una tecla para continuar...");
    wrefresh(win);
    clear();
}

MenuNcurses::~MenuNcurses() {
    delwin(win);
}

int MenuNcurses::show() { //se encarga del manejo de output
    int highlight = 0;
    int choice = -1;
    int input;

    while (true) {
        dibujar(highlight, true);

        input = wgetch(win);
        switch (input) {
            case KEY_UP:
                highlight = (highlight == 0) ? options.size() - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == options.size() - 1) ? 0 : highlight + 1;
                break;
            case 10:  // ENTER
                choice = highlight;
                return choice;
        }
    }
}

string MenuNcurses::getOption(int index) const {// evitar que me salgan opciones fantasma y las pueda seleccionar y no me arrojen nada y el programa se suicide xd
    if (index >= 0 && index < options.size())
        return options[index];
    return "";
}

string MenuNcurses::handle() { //retornar la opción seleccionada y ejecutar la ventana
    clear();
    refresh();

    int choice = show();
    string opcion = getOption(choice);
    if (opcion != "Salir") {
        mvprintw(LINES - 2, 2, ("Elegiste: " + opcion).c_str());
        getch();
    }
    clear();
    return opcion;
}

