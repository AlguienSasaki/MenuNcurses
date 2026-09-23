// MenuNcurses.hpp
#ifndef MENUNCURSES_H
#define MENUNCURSES_H

#pragma once
#include <ncurses.h>
#include <string>
#include <vector>

const std::string C_RESET = "{RESET}";
const std::string C_ROJO = "{ROJO}";
const std::string C_VERDE = "{VERDE}";
const std::string C_AMARILLO = "{AMARILLO}";
const std::string C_AZUL = "{AZUL}";
const std::string C_CIAN = "{CIAN}";
const std::string C_BOLD = "{BOLD}";

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
  virtual int show(); // manerjar inputs
  string getOption(int index) const;
  virtual string handle(); // retornar valor seleccionado

protected:
  void construirVentana(bool interactivo = true);
  void dibujar(int highlight = -1, bool interactivo = true);
};

#endif // MENUNCURSES_H
