#pragma once
#include <string>
#include <vector>

enum Command {
    CMD_LOGIN,
    CMD_CONSULTA_HISTORIAL,
    CMD_AGREGAR_HISTORIAL,
    CMD_SALIR,
    CMD_UNKNOWN
};

// Separa la línea por '|' y devuelve vectores de campos
std::vector<std::string> splitFields(const std::string& line);

// Convierte la cadena al enum Command
Command parseCommand(const std::string& line);

// Formatea una petición: CMD|param1|param2…\n
std::string formatRequest(Command cmd, const std::vector<std::string>& params = {});
