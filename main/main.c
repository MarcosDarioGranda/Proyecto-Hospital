#include <stdio.h>
#include <stdlib.h>
#include "menus.h"

int main(void) {
    while (menuInicioSesion()) {  // Mientras el usuario inicie sesión, entra a MENU2
        menuOpciones();
    }
    return 0;
}
