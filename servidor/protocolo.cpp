#include "protocolo.h"

std::vector<std::string> splitFields(const std::string& line) {
    std::vector<std::string> fields;
    size_t start = 0, pos;
    while ((pos = line.find('|', start)) != std::string::npos) {
        fields.push_back(line.substr(start, pos - start));
        start = pos + 1;
    }
    // quitar el '\n' final si existe
    std::string last = line.substr(start);
    if (!last.empty() && last.back()=='\n') last.pop_back();
    fields.push_back(last);
    return fields;
}

Command parseCommand(const std::string& line) {
    auto f = splitFields(line);
    if (f.empty()) return CMD_UNKNOWN;
    if (f[0]=="LOGIN")                return CMD_LOGIN;
    if (f[0]=="CONSULTA_HISTORIAL")   return CMD_CONSULTA_HISTORIAL;
    if (f[0]=="AGREGAR_HISTORIAL")    return CMD_AGREGAR_HISTORIAL;
    if (f[0]=="SALIR")                return CMD_SALIR;
    return CMD_UNKNOWN;
}

std::string formatRequest(Command cmd, const std::vector<std::string>& params) {
    std::string s;
    switch (cmd) {
        case CMD_LOGIN:               s = "LOGIN"; break;
        case CMD_CONSULTA_HISTORIAL:  s = "CONSULTA_HISTORIAL"; break;
        case CMD_AGREGAR_HISTORIAL:   s = "AGREGAR_HISTORIAL"; break;
        case CMD_SALIR:               s = "SALIR"; break;
        default:                      s = "UNKNOWN"; break;
    }
    for (auto &p: params) {
        s += "|" + p;
    }
    s += "\n";
    return s;
}
