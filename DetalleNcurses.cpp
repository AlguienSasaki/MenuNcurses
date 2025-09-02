// DetalleNcurses.cpp
#include "DetalleNcurses.hpp"

DetalleNcurses::DetalleNcurses(const string &title, const vector<string> &lineas) //lo mismo que catalogo xd
    : MenuNcurses(title, lineas) {
    construirVentana(false);
    dibujar(-1, false);
}

void DetalleNcurses::mostrar() {
//limpia pantalla y refresca
    wgetch(win);
    werase(win);
    wrefresh(win);
}

//DetalleNcurses::~DetalleNcurses(){
//}
