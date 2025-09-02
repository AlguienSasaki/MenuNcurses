// MenuNcurses.hpp
#ifndef MENUNCURSES_H 
#define MENUNCURSES_H  

#include <ncurses.h>
#include <vector>
#include <string>

using namespace std;

class MenuNcurses {
protected:
    vector<string> options;
    string title;
    int width = 130;
    int startY, startX;
    WINDOW *win = nullptr;

public:
    MenuNcurses(const string &title, const vector<string> &options);
    virtual ~MenuNcurses();
    virtual int show(); //manerjar inputs
    string getOption(int index) const;
    virtual string handle(); //retornar valor seleccionado

protected:
    void construirVentana(bool interactivo = true);
    void dibujar(int highlight = -1, bool interactivo = true);
};

#endif // MENUNCURSES_H
