// DetalleNcurses.hpp
#ifndef DETALLENCURSES_H
#define DETALLENCURSES_H

#include "MenuNcurses.hpp"

class DetalleNcurses : public MenuNcurses {
public:
    DetalleNcurses(const string &title, const vector<string> &lineas);
    void mostrar();
    //virtual ~DetallesNcurses();
};

#endif // DETALLENCURSES_H
