#include "LexicalAnalyzer.h"
#include <cctype>
#include <vector>
#include <cctype>
#include <QDebug>

//SOLO DECLARA LAS FUNCIONAS A UTILZIAR
//UTILIZACION DEL HEADER LexicalAnalyzer
//clase para el analizador lexico y utiliza el header de Lexicalanalyzer para decalrar las fucnoiones
// De que Clase ::
using namespace std;
// PUNTERO
//DE LEXICALANALYZER HEADER LA FUNCION
LexicalAnalyzer::LexicalAnalyzer(const string& fuente)
    //Resolucion de ambito
    : codigo(fuente), pos(0), linea(1), columna(1) {}
//Codigo fuente, en que posicion se encentra

//

// ── Helpers ─────────────────────────────────────────────────────

char LexicalAnalyzer::actual() {
    if (pos < (int)codigo.size()) return codigo[pos];
    //si la posicion es inferior al codigo retorna la posicion actual
    return '\0'; //de lo contrario si no lee nada retorna cero //devuelve un cahr
}

char LexicalAnalyzer::avanzar() {
    //FUNCION PARA MOVER A LA SIGUENTE FILA
    char c = actual(); //trae el valor de la posicion actual
    pos++; //incrementa la posicion actual
    if (c == '\n') { linea++; columna = 1; } //si es un salto de linea
    else           { columna++; }
    return c; //retorna actual
}

void LexicalAnalyzer::saltarBlancos() { //salta espacios en blanco
    while (pos < (int)codigo.size() && isspace((unsigned char)actual())) //mientras la posicion sea inferior al codigo Y ENCUENTRE UN ESPACIO EN BLANCO
        avanzar();
}


//al detectar un error lexico
void LexicalAnalyzer::registrarError_L(const string& lex,  const string& tipo,const string& desc) {
    //llama al vector de errores y mete el error lexico
    errores_L.push_back({ lex, tipo, desc, linea, columna });
}
//al detectar un error Sintax
void LexicalAnalyzer::registrarError_S(const string& desc) {
    //llama al vector de errores y mete el error lexico
    errores_S.push_back({ desc });
}





// ── AFD Principal ────────────────────────────────────────────────

Token LexicalAnalyzer::siguienteToken() { //PARA SIGUENTE TOKEN
    saltarBlancos();

    int linIni = linea; //en que linea
    int colIni = columna; //en que columna

    if (pos >= (int)codigo.size())
        return { TokenType::FIN_ARCHIVO, "EOF", linIni, colIni }; //Si la posicion es mayor o igual al tamaÑo del codigo actual regresa el token FIN?ARCHIVO

    char c = actual(); //RETORNO DEL ACTUAL

    // Delimitadores de un solo carácter
    if (c == '{') { avanzar(); return { TokenType::LLAVE_ABRE,   "{", linIni, colIni }; } //SI CH es un Bracket { avanza y retona que es una llave
    if (c == '}') { avanzar(); return { TokenType::LLAVE_CIERRA, "}", linIni, colIni }; } //SI CH es un Bracket { avanza y retona que es una llave
    if (c == ';') { avanzar(); return { TokenType::PUNTO_COMA,   ";", linIni, colIni }; } //SI CH es un SEMICOLON ; avanza y retona que es una SEMICOLON
    if (c == ':') { avanzar(); return { TokenType::DOS_PUNTOS,   ":", linIni, colIni }; }
    if (c == ',') { avanzar(); return { TokenType::COMA,         ",", linIni, colIni }; }
    if (c == '[') { avanzar(); return { TokenType::CORCH_ABRE, "[", linIni, colIni }; }
    if (c == ']') { avanzar(); return { TokenType::CORCH_CIERRA, "]", linIni, colIni }; }
    if (c == ',') { avanzar(); return { TokenType::COMA, "]", linIni, colIni }; }
    if (c == '"') { return leerString(); }  // Nueva función para strings

    //reconocimiento de numeros
    if(isdigit(c) || c=='-'){
        return leerNum();
    }


    // Palabra reservada o identificador
    if (std::isalpha((unsigned char)c) || c == '_') //SI ES PALABRA RESERVADA O ID
        return leerPalabraReservada();

    // Carácter no reconocido → error léxico
    std::string lex(1, c);
    avanzar();
    registrarError_L(lex, "Carácter ilegal",
                   "El carácter '" + lex + "' no pertenece al alfabeto de MedLang.");
    return { TokenType::DESCONOCIDO, lex, linIni, colIni };
}

// ── Reconocedor de palabras reservadas ──────────────────────────

Token LexicalAnalyzer::leerPalabraReservada() { //RETORNA UN TOKEN
    int linIni = linea;
    int colIni = columna;
    string lexema;

    // Leer todos los caracteres alfanuméricos o guión bajo
    while (pos < (int)codigo.size() &&
           //mientras la posciion sea inferior al tamaño del codigo y sea indefinido o _
           (std::isalnum((unsigned char)actual()) || actual() == '_')) {
        lexema += avanzar();
    }

    // Tabla de palabras reservadas, al leerla retorna un Token del tipo leido por el lexema
    if (lexema == "TABLERO") //SI NO ESTA VALIO TODO
        return { TokenType::TABLERO, lexema, linIni, colIni };
    //CASOS
    if(lexema == "{" )
        return {TokenType::LLAVE_ABRE,lexema,linIni,colIni};
    if(lexema == "}")
        return {TokenType::LLAVE_CIERRA,lexema,linIni,colIni};
    if(lexema == ";")
        return {TokenType::PUNTO_COMA,lexema,linIni,colIni};
    if(lexema == ":")
        return {TokenType::DOS_PUNTOS,lexema,linIni,colIni};
    if(lexema == "[")
        return {TokenType::CORCH_ABRE,lexema,linIni,colIni};
    if(lexema == "]")
        return {TokenType::CORCH_CIERRA,lexema,linIni,colIni};
    if(lexema ==",")
        return{TokenType::COMA,lexema,linIni,colIni};

    if(lexema == "COLUMNA")
        return {TokenType::COLUMNA,lexema,linIni,colIni};
    if(lexema == "tarea")
        return {TokenType::tarea,lexema,linIni,colIni};
    if(lexema == "prioridad")
        return {TokenType::prioridad,lexema,linIni,colIni};

    //NIVEL DE PRIORIDAD
    if(lexema == "ALTA")
        return {TokenType::ALTA,lexema,linIni,colIni};
    if(lexema == "MEDIA")
        return {TokenType::MEDIA,lexema,linIni,colIni};
    if(lexema == "BAJA")
        return {TokenType::BAJA,lexema,linIni,colIni};



    if(lexema == "responsable")
        return {TokenType::responsable,lexema,linIni,colIni};
    if(lexema == "fecha_limite")
        return {TokenType::fecha_limite,lexema,linIni,colIni};
    if(lexema == "prioridades")
        return {TokenType::prioridades,lexema,linIni,colIni};
    if(lexema == "Fecha")
        return {TokenType::Fecha,lexema,linIni,colIni};
    if(lexema == "delimitador")
        return {TokenType::delimitador,lexema,linIni,colIni};





    // En caso que no sea una palabra reservada entonces se llama a registrar el error
    registrarError_L(lexema, "Token Desconocido",
                   "'" + lexema + "' no es una palabra reservada válida en TaskScript.");
    return { TokenType::DESCONOCIDO, lexema, linIni, colIni };
}


//DECLARACION DE FUNCION PARA LECTURA DE STRINGS
Token LexicalAnalyzer::leerString(){
    int linIn = linea;
    int colIn = columna;
    string lexema; //llegara con cualquier contenido

    avanzar(); // Saltara la comilla inicial

    while (pos < (int)codigo.size() && actual() != '"') {
        lexema += avanzar();
    }

    // Verificar que se encontró la comilla de cierre
    if (actual() == '"') {
        avanzar(); // Saltar la comilla final
        return { TokenType::STRING, lexema, linIn, colIn };
    } else {
        registrarError_L(lexema, "String mal formado", "Falta comilla de cierre");
        return { TokenType::DESCONOCIDO, lexema, linIn, colIn };
    }
}

Token LexicalAnalyzer::leerNum(){ //LECTURA DE NUMEROS
    int LinIn = linea;
    int ColIn = columna;
    string lexema;
    while (pos < (int)codigo.size() && (isdigit((unsigned char)actual()) || actual() == '-' || actual() == ':')) { //si lee numeros y tienen los siguentes digitos
        lexema += avanzar();
        //Si encuentra que es un digito y contiene los simbolos especiales
    }

    if (lexema.find('-') != string::npos) {
        return { TokenType::Fecha, lexema, LinIn, ColIn };
    } else {
        return { TokenType::NUMERO, lexema, LinIn, ColIn };
    }

}

// ── Tokenización completa ──────────────────────────────────────── //LEE LOS TOKENS
//CREACION DE TOKENS
vector<Token> LexicalAnalyzer::tokenize() {
    vector<Token> tokens; //invoca al vector de tokens de LexicaAnalyzer
    pos = 0; linea = 1; columna = 1;
    //Limpieza de vectores
    columnStorage.clear();
    errores_L.clear();
    errores_S.clear();

    qDebug() << "Tamaño del código:" << codigo.size() << "bytes";
    //Parseo GENERAL
    while (true) {
        Token t = siguienteToken(); //lee os tokens
        if (t.tipo == TokenType::FIN_ARCHIVO) break;
        if (t.tipo != TokenType::DESCONOCIDO)
            tokens.push_back(t);
    }
    //Parseo de Columnas, GENERA LOS TOKENS PARA LOS VECTORES
    int posTok = 0;
    qDebug() << "Iniciando registro de columnas. Total tokens:" << tokens.size();

    while (posTok < (int)tokens.size()) {
        qDebug() << "  posTok:" << posTok
                 << "Token:" << QString::fromStdString(tokenTypeToString(tokens[posTok].tipo));

        if (tokens[posTok].tipo == TokenType::TABLERO) {
            qDebug() << "Encontrado TABLERO en posición" << posTok;
            posTok++; // Saltar TABLERO

            // Saltar el nombre del tablero (STRING)
            if (posTok < (int)tokens.size() && tokens[posTok].tipo == TokenType::STRING) {
                qDebug() << "  Saltando nombre:" << QString::fromStdString(tokens[posTok].lexema);
                posTok++;
            }

            // Ahora posTok apunta a LLAVE_ABRE
            registrarColumnas(tokens, posTok);
            qDebug() << "Volvió de registrarColumnas, posTok=" << posTok;
        } else {
            posTok++;
        }
    }

    //SINTAX PARSEO

    this->tokens = tokens;
    tokenActual =0;
    try {
        parsePrograma();//Inicio de parseo
        if(tokenActual < (int)tokens.size()){
            registrarError_S("ERROR,TOKENS SIN PROCESAR");
        }

    } catch (exception e) {
        qWarning() << "Error en la lectura Sintactica" << e.what();
    }

    //!
    qDebug() << "=== TOKENIZE END ===";
    qDebug() << "Columnas registradas:" << columnStorage.size();


    return tokens;//retorno
    ///! DEBUGG

}

//-- SINTAX

bool LexicalAnalyzer::isFinished(){
    return tokenActual >= (int)tokens.size();
}


bool LexicalAnalyzer::isValid(){
    return errores_L.empty() && errores_S.empty();
}

Token LexicalAnalyzer::Consume(TokenType await){
    //SI EL TOKEN ACTUAL ESTA UBICADO FUERA DE RANGO
    if(tokenActual >= (int)tokens.size()){
        registrarError_S("ERROR,TOKENS SIN PROCESAR");
        //retorna a la caja de tokens
        return Token{TokenType::DESCONOCIDO,"?",0,0};
    }

    Token t = tokens[tokenActual]; //Token actual lo tomara
    if(t.tipo == await){ //SI EN CASO DE QUE EL TIPO DE TOKEN SEA EL ESPERADO
        tokenActual++;
        return t; //RETORNA TOKEN ACTUAL SUMADO
    }else{
        registrarError_S(tokenTypeToString(await) + ", Token Diferente al esperado");
        return Token{TokenType::DESCONOCIDO,"",0,0};
    }


}


void LexicalAnalyzer::parsePrograma(){
    Consume(TokenType::TABLERO);

    Token name = Consume(TokenType::STRING); //CHAR
    Consume(TokenType::LLAVE_ABRE);
    parseColumns();
    Consume(TokenType::LLAVE_CIERRA);
    Consume(TokenType::PUNTO_COMA);

    qDebug() << "Parseo de Tablero Completado";
}


void LexicalAnalyzer::parseColumns(){
    parseColumn(); //DEBE HAbER UNA COLUMNA
    while (tokenActual < (int)tokens.size() &&
           tokens[tokenActual].tipo == TokenType::COLUMNA) {
        parseColumn();  // <- avance
        }
    qDebug() << "Parseo de Columnas Completado";

}

void LexicalAnalyzer::parseColumn(){
    Consume(TokenType::COLUMNA);
    Token name = Consume(TokenType::STRING); //CHAR
    Consume(TokenType::LLAVE_ABRE);
    parseTareas();
    Consume(TokenType::LLAVE_CIERRA);
    Consume(TokenType::PUNTO_COMA);
    qDebug() << "Parseo de Columna Completado";

}

void LexicalAnalyzer::parseTareas(){
    parseTarea();//MINIIMO UNA TAREA
    while (tokenActual < (int)tokens.size() &&
           tokens[tokenActual].tipo == TokenType::COMA) {
        Consume(TokenType::COMA);
        parseTarea();
    }
    qDebug() << "Parseo de Tareas Completado";
}

void LexicalAnalyzer::parseTarea(){
    Consume(TokenType::tarea);
    Consume(TokenType::DOS_PUNTOS);
    Consume(TokenType::STRING);
    Consume(TokenType::LLAVE_ABRE);

    // Procesar atributos
    while (tokenActual < (int)tokens.size() &&
           tokens[tokenActual].tipo != TokenType::LLAVE_CIERRA) {

        if (tokens[tokenActual].tipo == TokenType::prioridad) {
            Consume(TokenType::prioridad);
            Consume(TokenType::DOS_PUNTOS);
            if (tokens[tokenActual].tipo == TokenType::ALTA ||
                tokens[tokenActual].tipo == TokenType::MEDIA ||
                tokens[tokenActual].tipo == TokenType::BAJA) {
                Consume(tokens[tokenActual].tipo);
            }
        }
        else if (tokens[tokenActual].tipo == TokenType::responsable) {
            Consume(TokenType::responsable);
            Consume(TokenType::DOS_PUNTOS);
            Consume(TokenType::STRING);
        }
        else if (tokens[tokenActual].tipo == TokenType::fecha_limite) {
            Consume(TokenType::fecha_limite);
            Consume(TokenType::DOS_PUNTOS);
            Consume(TokenType::Fecha);
        }
        else if (tokens[tokenActual].tipo == TokenType::COMA) {
            Consume(TokenType::COMA);
        }
        else {
            break;
        }
    }

    Consume(TokenType::LLAVE_CIERRA);  // "}"


}


// -- LECTURA DE DATOS --

//! REGISTRAR COLUMNAS
void LexicalAnalyzer::registrarColumnas(const vector<Token>& tokens, int& pos) {
    qDebug() << "LECTURA COLUMNAS";
    //DESPUES DE TABLERO BUSCARA LLAVE ABRE
    if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::LLAVE_ABRE) {
        pos++; // Saltar { despues de tablero

        while (pos < (int)tokens.size() && tokens[pos].tipo != TokenType::LLAVE_CIERRA) { //MIENTRAS NO ENCUENTRE LLAVE CERRADA
            if (tokens[pos].tipo == TokenType::COLUMNA) {
                Columna nuevaCol;
                pos++; // Saltar COLUMNA
                if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) {
                    pos++; //SALTA DOS PUNTOS
                }
                if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::STRING) {
                    nuevaCol.Columna = tokens[pos].lexema;
                    qDebug() << "Columna:" << QString::fromStdString(nuevaCol.Columna);
                    pos++; // SALTA STRING
                }
                if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::LLAVE_ABRE) {
                    pos++; // Saltar {
                }
                while (pos < (int)tokens.size() && tokens[pos].tipo != TokenType::LLAVE_CIERRA) {  //MIENTRAS NO ENCUENTRE LLAVE CERRADA
                    if (tokens[pos].tipo == TokenType::tarea) { //Si encuentra tarea
                        Tarea newTarea; //Estructura temporal de tarea
                        pos++; // Saltar tarea y declara una nueva tarea, por cada nueva tarea se iran registrando los datos
                        if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) {
                            pos++; //SALTA 2 Puntos
                        }
                        if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::STRING) {
                            newTarea.nombre_tarea = tokens[pos].lexema;
                            qDebug() << "Tarea:" << QString::fromStdString(newTarea.nombre_tarea);
                            //agrega nombre de tarea
                            pos++;
                        }
                        if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::CORCH_ABRE) {
                            pos++; // Saltar '['
                        }
                        // Leer atributos dentro de CORCHETES
                        while (pos < (int)tokens.size() && tokens[pos].tipo != TokenType::CORCH_CIERRA) { //dentro de tarea buscara hasta que no hallan corchetes
                                    if (tokens[pos].tipo == TokenType::prioridad) {
                                        pos++;
                                        if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) pos++;

                                        if (pos < (int)tokens.size() &&
                                            (tokens[pos].tipo == TokenType::ALTA ||
                                             tokens[pos].tipo == TokenType::MEDIA ||
                                             tokens[pos].tipo == TokenType::BAJA ||
                                             tokens[pos].tipo == TokenType::STRING)) {
                                            newTarea.nivel_prioridad = tokens[pos].lexema;
                                            qDebug() << "SE AGREGO PRIORIDAD ,Prioridad:" << QString::fromStdString(newTarea.nivel_prioridad);
                                            pos++;
                                        }
                                    }
                                    else if (tokens[pos].tipo == TokenType::responsable) {
                                        pos++;
                                        if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) pos++;

                                        if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::STRING) {
                                            newTarea.responsable = tokens[pos].lexema;
                                            qDebug() << "SE AGREGO ReSPONSABLE,Responsable:" << QString::fromStdString(newTarea.responsable);
                                            pos++;
                                        }
                                    }
                                    else if (tokens[pos].tipo == TokenType::fecha_limite) {
                                        pos++;
                                        if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) pos++;

                                        if (pos < (int)tokens.size() &&
                                            (tokens[pos].tipo == TokenType::Fecha ||
                                             tokens[pos].tipo == TokenType::NUMERO ||
                                             tokens[pos].tipo == TokenType::STRING)) {
                                            newTarea.fecha_limit = tokens[pos].lexema;
                                            qDebug() << " SE AGREGO FECHA LIMIT: Fecha límite:" << QString::fromStdString(newTarea.fecha_limit);
                                            pos++;
                                        }
                                    }
                                    else if (tokens[pos].tipo == TokenType::COMA) {
                                        pos++; //si encuetnra coma se mueve
                                        //ACA REGRESA
                                    }
                                    else {
                                        pos++;
                                    }
                        } //fin while Corch_Cierra
                        //termina de leer lo que tiene dentro los corchetes
                        if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::CORCH_CIERRA) {
                            pos++;
                        } //SALTA CORCHETE
                        //Ahora como newTarea ya tiene contenido se envia a la columna principal
                        nuevaCol.tarea_column.push_back(newTarea);
                        qDebug() << " TAREA AGREGADA, BUSCANDO NUEVAS TAREAS";
                    } //fin if tarea
                    else {
                        pos++;
                    }
                    //se mueve hasta encontrar otra palabra clave con tarea hasta que salga de las llaves
                }//fin while LLAVE CERRADA
                if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::LLAVE_CIERRA) {
                    pos++;
                }
                if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::PUNTO_COMA) {
                    pos++;
                }

                //cuando llegue al punto de detectar una llave cerrada agregara la columna
                columnStorage.push_back(nuevaCol);
                qDebug() << "Columna agregada con" << nuevaCol.tarea_column.size() << ", Buscando mas columnas";
                //sube la columna actual y como el while no se completo sigue buscando.


            }//Fin if columna
            else {
                pos++;
            }
        }//Fin while columna llave abierta
        if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::LLAVE_CIERRA) {
            pos++;
        }

        //FINALZA ARCHIVO
    }
    qDebug() << "ARCHIVO FINALIZADO!";
    mostrarContenido();

}

void LexicalAnalyzer::mostrarContenido(){
    qDebug() << "/////////////////////////////////////////////////////////////////////////////////////////////////" ;
    qDebug() << "COLUMNAS REGISTRADAS";
    qDebug() << "Total columnas:" << columnStorage.size();
    for (int i = 0; i < (int)columnStorage.size(); i++) {
        qDebug() << "Columna" << i+1 << ":" << QString::fromStdString(columnStorage[i].Columna);
        for (int t = 0; t < (int)columnStorage[i].tarea_column.size(); t++) {
            qDebug() << "    Tarea:" << QString::fromStdString(columnStorage[i].tarea_column[t].nombre_tarea);
            qDebug() << "    Prioridad:" << QString::fromStdString(columnStorage[i].tarea_column[t].nivel_prioridad);
            qDebug() << "    Responsable:" << QString::fromStdString(columnStorage[i].tarea_column[t].responsable);
            qDebug() << "    Fecha límite:" << QString::fromStdString(columnStorage[i].tarea_column[t].fecha_limit);
        }
    }
    qDebug() << "/////////////////////////////////////////////////////////////////////////////////////////////////" ;
}
