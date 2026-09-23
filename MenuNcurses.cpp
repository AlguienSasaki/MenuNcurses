#include "MenuNcurses.hpp"

void imprimirConFormato(WINDOW *win, int &y, int x_inicio, int max_x,
                        const std::string &texto) {
  int x = x_inicio;
  wmove(win, y, x);
  size_t i = 0;

  while (i < texto.length()) {
    if (texto[i] == '{') {
      size_t fin = texto.find('}', i);
      if (fin != std::string::npos) {
        std::string tag = texto.substr(i, fin - i + 1);

        if (tag == "{ROJO}")
          wattron(win, COLOR_PAIR(1));
        else if (tag == "{VERDE}")
          wattron(win, COLOR_PAIR(2));
        else if (tag == "{AMARILLO}")
          wattron(win, COLOR_PAIR(3));
        else if (tag == "{AZUL}")
          wattron(win, COLOR_PAIR(4));
        else if (tag == "{CIAN}")
          wattron(win, COLOR_PAIR(5));
        else if (tag == "{BOLD}")
          wattron(win, A_BOLD);
        else if (tag == "{RESET}") {
          wattroff(win, COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3) |
                            COLOR_PAIR(4) | COLOR_PAIR(5) | A_BOLD);
        } else {
          // Si no es un tag reconocido, lo dibuja como caracter
          if (x >= max_x) {
            y++;
            x = x_inicio;
            wmove(win, y, x);
          }
          waddch(win, texto[i]);
          x++;
          i++;
          continue;
        }

        i = fin + 1;
        continue;
      }
    }

    // para q no se rompa la wbada e intenté escribir en los bordes >:V
    if (x >= max_x) {
      y++;
      x = x_inicio;
      wmove(win, y, x);
    }

    waddch(win, texto[i]);
    x++;
    i++;
  }

  // Limpieza
  wattroff(win, COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3) | COLOR_PAIR(4) |
                    COLOR_PAIR(5) | A_BOLD);
}

MenuNcurses::MenuNcurses(const string &title,
                         const vector<string> &options) // constructor
    : title(title), options(options) {
  construirVentana();
}

void MenuNcurses::construirVentana(bool interactivo) {
  int lineasTotales = 0;
  int anchoUtil = width - 4;

  for (const auto &opt : options) {
    if (opt.empty()) {
      lineasTotales++;
    } else {
      lineasTotales += (opt.length() + anchoUtil - 1) / anchoUtil;
    }
  }

  int height = lineasTotales + (interactivo ? 2 : 4);
  startY = (LINES - height) / 2;
  startX = (COLS - width) / 2;
  win = newwin(height, width, startY, startX);
  keypad(win, interactivo);
}

void MenuNcurses::dibujar(int highlight, bool interactivo) {
  werase(win);

  // Borde en color amarillo/cian/nose, esto me tocará reescribirlo para que
  // pueda poner los colores q quiera y no limitarme, pero de mientras se queda
  // hardcodeado :"V
  wattron(win, COLOR_PAIR(3));
  box(win, 0, 0);
  wattroff(win, COLOR_PAIR(3));

  // Título.
  if (!title.empty()) {
    wattron(win, COLOR_PAIR(4) | A_BOLD);
    mvwprintw(win, 0, 2, (" " + title + " ").c_str());
    wattroff(win, COLOR_PAIR(4) | A_BOLD);
  }

  int currY = 1;
  int maxX = width -
             3; // Límite derecho antes de tocar el marco vertical, lo mismo,
                // para hacer q no se sobreescriba en el borde la wbada esa >:\/

  for (size_t i = 0; i < options.size(); ++i) {
    if (interactivo && (int)i == highlight)
      wattron(win, A_REVERSE);

    // Se imprime respetando los margenes.
    imprimirConFormato(win, currY, 2, maxX, options[i]);
    currY++; // Pasa a la siguiente fila para el siguiente elemento

    if (interactivo && (int)i == highlight)
      wattroff(win, A_REVERSE);
  }

  if (!interactivo) {
    wattron(win, A_DIM);
    mvwprintw(
        win, currY + 1, 2,
        "Presiona una tecla para continuar..."); // Igual esto, no me gusta q
                                                 // esté hardcodeado, pero de
                                                 // mientras jala.
    wattroff(win, A_DIM);
  }

  wrefresh(win);
  clear();
}

MenuNcurses::~MenuNcurses() { delwin(win); }

int MenuNcurses::show() { // se encarga del manejo de output
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
    case 10: // ENTER
      choice = highlight;
      return choice;
    }
  }
}

string MenuNcurses::getOption(int index)
    const { // evitar que me salgan opciones fantasma y las pueda seleccionar y
            // no me arrojen nada y el programa se suicide xd
  if (index >= 0 && index < options.size())
    return options[index];
  return "";
}

string
MenuNcurses::handle() { // retornar la opción seleccionada y ejecutar la ventana
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
