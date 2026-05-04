#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

//PART 1
//Se declara para saber de su existencia

//Utilizacion de :: para llamar los atributos,metodos,datos y vectores de las clases
//TODOS LOS TIPOS

//se declaran para ser leidos por el lexicalAnalyzer y poder generar esturctras de tokens

enum class TokenType { //TODOS LOS TIPOS DE TOKENS
    // Palabra reservada raíz
    TABLERO,

    // Delimitadores
    LLAVE_ABRE,     // {
    LLAVE_CIERRA,   // }
    PUNTO_COMA,     // ;
    //PALABRAS RESERVADAS V2
    DESCONOCIDO, //
    FIN_ARCHIVO, //
    STRING, //
    CHAR, //
    NUMERO, //
    //SIMBOLOS ESPECIALES
    COMA, //
    DOS_PUNTOS, //
    CORCH_ABRE,//
    CORCH_CIERRA,//
    //NUEVOS TOKENS
    COLUMNA,
    tarea,
    prioridad,
    INCOMPLETE_STRING,

    ALTA,
    MEDIA,
    BAJA,
    responsable,
    fecha_limite,
    prioridades,
    Fecha,
    delimitador,
};

// Convierte el enum a string legible para la tabla y retorna EL TIPO DE STRING
inline string tokenTypeToString(TokenType t) { //RETORNA el tipo de string leido
    switch (t) {
        //SI NO INCLUYE COMILLAS ES UN TOKEN
        //de la clase::ValorDeLaClase
    case TokenType::TABLERO:      return "PALABRA_RESERVADA"; //Viene de... :: de donde viene que trae //SI NO TIENE LA PLABRA TABLERO VALIO QUESO
    case TokenType::LLAVE_ABRE:    return "LLAVE_ABRE";
    case TokenType::LLAVE_CIERRA:  return "LLAVE_CIERRA";
    case TokenType::PUNTO_COMA:    return "PUNTO_COMA";
    case TokenType::CORCH_CIERRA: return "CORCHETE_CIERRA";
    case TokenType::CORCH_ABRE: return "CORCHETE_CIERRA";
    case TokenType::DOS_PUNTOS: return "DOS_PUNTOS";
    case TokenType::FIN_ARCHIVO:   return "FIN_ARCHIVO";
    case TokenType::COMA: return "COMA";
    case TokenType::INCOMPLETE_STRING: return "STRING INCOMPLETO";

        //SINTAX
    case TokenType::tarea: return "tarea";
    case TokenType::prioridad: return "prioridad";
    case TokenType::responsable: return "responsable";
    case TokenType::fecha_limite: return "fecha_limite";
    case TokenType::prioridades: return "prioridad";
    case TokenType::Fecha: return "Fecha";
    case TokenType::delimitador: return "Delimitador";
    case TokenType::ALTA: return "ALTA";
    case TokenType::MEDIA: return "MEDIA";
    case TokenType::BAJA: return "BAJA";

        //NEST
    case TokenType::STRING: return "STRING";
    case TokenType::NUMERO: return "NUMERO";
    case TokenType::CHAR: return "CHAR"; //este por si las dudas


    default:                       return "DESCONOCIDO";
    }
}

struct Token { //Estructura del Token
    TokenType tipo; //Token
    string lexema; //Entrada
    int linea; //linea
    int columna; //columna
};

#endif // TOKEN_H