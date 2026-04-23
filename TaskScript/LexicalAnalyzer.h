#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H
//PART 2
#include "Token.h"
#include <string>
#include <vector>


//ESTRUCTURA SINTACTICA

struct Tarea{
    string nombre_tarea;
    string nivel_prioridad;
    string fecha_limit;
    string responsable;
};

struct Columna{
    string Columna;
    //vector que contenga las tareas
    vector<Tarea> tarea_column;
};

struct ErrorLexico {
    string lexema;
    string tipoError;
    string descripcion;
    int linea;
    int columna;
};

struct ErrorSintactico {
    string descripcion;
};



//ANALIZADOR LEXICO
class LexicalAnalyzer {
public:

    explicit LexicalAnalyzer(const string& fuente); //Constructor OBLICATORIO
    //Constante para que no cambie
    //Contenedores ALMACENARAN LOS DATOS AL LEER EL DOCUMENTO
    //LUEGO SE DECLARAN OTROS VECTORES PARA MAIN WINDOW CON EL MISMO VALOR
    /*

    vector<Medic> medStorage; //
    vector<Citas> citStorage; //
    vector<Patient> patStorage; //
    vector<Diagnostic> diagStorage; //
    */

    vector<Columna> columnStorage; //almacenar tareas
    // Retorna todos los tokens del archivo
    vector<Token> tokenize(); //declara vector para tokenizacion y mostrar en tablas
    // Errores acumulados durante el análisis
    vector<ErrorLexico> errores_L;
    vector<ErrorSintactico> errores_S;

    bool parse();

private:
    //METODOS Y VARIABLES ABSTRACTAS, solo se declara su existencia pero no se definen
    string codigo;   // Contenido completo del archivo
    int pos;         // Posición actual en el string
    int linea;       //EN QUE LINEA
    int columna;        //EN QUE COLUMNA
    char actual();        // Carácter en pos actual
    char avanzar();       // Avanza y retorna el siguiente carácter
    void saltarBlancos(); // Ignora espacios, tabs, saltos de línea
    Token siguienteToken();        // AFD principal
    Token leerPalabraReservada(); // Reconoce HOSPITAL (y futuras), retorna
    Token leerString(); //Lectura de strings
    Token leerNum(); //lectura de numeros o guiones


    //!NEW!
    //!
    //!
    int tokenActual;
    vector<Token>tokens; //guardar tokens
    Token Consume(TokenType awaiting);
    Token currentToken() const;

    void  registrarError_L(const string& lexema,const string& tipo,const string& desc);
    void  registrarError_S(const string& desc);
    void parsePrograma();
    void parseColumns();
    void parseColumn();
    void parseTarea();
    void parseTareas();
    bool isFinished();
    bool isValid();




    //ENVIA NECESITA EL LEXEMA EL TIPO Y LO ENVIA AL VECTOR DE ERRORLEXICO

    //REGISTRAR DATOS EN LEXICALANALYZER
    void registrarColumnas(const vector<Token>& tokens, int& pos);
    void mostrarContenido();

    //UNUSED



};





#endif // LEXICALANALYZER_H