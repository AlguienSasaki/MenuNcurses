#ifndef INPUTNCURSES_H
#define INPUTNCURSES_H

#include "MenuNcurses.hpp"
#include <sstream> //convertir un tipo de dato a otro
#include <stdexcept> // try catch y eso. Deteccion de errores pues

using namespace std;

class InputNcurses : public MenuNcurses {
public:
    InputNcurses(const string &mensaje, const string &prompt = "Entrada: ");
    InputNcurses(const string &mensaje, const string &variable, const string &mensaje2, const string &prompt = "Entrada: ");

    template<typename T>
    T solicitarEntrada();

private:
    string mensaje;
    string prompt;
    string variable;
    string mensaje2;
};

// Implementación del constructor en línea
inline InputNcurses::InputNcurses(const string &mensaje, const string &prompt) //solo permite declararse en un .hpp o .h por el temita de los templates
    : MenuNcurses("", {}), mensaje(mensaje), prompt(prompt) {
    width = max((int)mensaje.length(), (int)prompt.length() + 80);
    construirVentana(false);  // ventana no interactiva
}

//InputNcurses::InputNcurses(const char [47], std::string&, const char [3])
inline InputNcurses::InputNcurses(const string &mensaje, const string &variable, const string &mensaje2, const string &prompt)
  : MenuNcurses("", {}), mensaje(mensaje), variable(variable), mensaje2(mensaje2), prompt(prompt) {
    width = max(
        static_cast<int>(mensaje.length() + variable.length() + mensaje2.length()), //converte a entero para pasarlo abajo a la longitud del menu
        static_cast<int>(prompt.length()) + 80
    );
    construirVentana(false);  // ventana no interactiva, lo mismo que en DetalleNcurses
}


template<typename T> 
T InputNcurses::solicitarEntrada() {  //define la funcion plantilla
    werase(win);
    box(win, 0, 0); //dibuja el marco
// comprobacion de si se usa variable y mensjae2
    string mensajeCompleto;
    if (!variable.empty() || !mensaje2.empty())
        mensajeCompleto = mensaje + variable + mensaje2;
    else
        mensajeCompleto = mensaje;
//imprime lo qu tenga
    if (!mensajeCompleto.empty())
        mvwprintw(win, 1, 2, mensajeCompleto.c_str());
// imprime el prompt como string en la linea 3 columna 2
    mvwprintw(win, 3, 2, prompt.c_str());
    wrefresh(win);

    char input[256]; //maximo de 256 caracteres
    echo(); //los caracteres se muestren en pantalla, si no sale como contraseña de terminal xd
    mvwgetnstr(win, 3, 2 + prompt.length(), input, 255); //imprime el promt + lo que le haya puesto
    noecho();//al picarle enter deja de mostrar lo que se escribe por pantalla

    werase(win);
    wrefresh(win);

    stringstream ss(input);

    //  Manejo especial si T es string
    if constexpr (is_same<T, string>::value) { //if en tiempo de compilacion que verifica si es string, si lo es ps retorna el valor
        return ss.str();  // devolvemos la cadena completa
    }

    T valor; //lo converte a string si o si jaja le valió lo de arriba xd
    ss >> noskipws >> valor; //hace que no se eviten los espacios

    if (ss.fail() || ss.peek() != EOF) {// el ss.fail devuelve si o no si es que fallo al convertir en string, el ss.peek busca que algo en lo dado lo tena, falla si hay algo mas que no sea string
        throw invalid_argument("Entrada inválida para el tipo solicitado.");
    }

    return valor;
}


#endif // INPUTNCURSES_H

