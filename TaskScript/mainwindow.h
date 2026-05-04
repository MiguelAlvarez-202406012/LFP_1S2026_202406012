#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QStatusBar>
#include <vector>
#include <string>
#include "LexicalAnalyzer.h"


//TODO LO QUE MANEJARA LA INTERFAZ
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    //FUNCIONES POR EVENTOS POR MEDIO DEL GUI
    void cargarArchivo();
    void analizarArchivo();
    void abrirReporte1(); //TAREAS
    void abrirReporte2(); //CARGA
    void abrirReporte5(); //ERRORES


    //UNUSED



private: //DONDE SE ALMACENARAN TODAS FUNCIONES QUE NO RESPONDEN A EVENTOS POR MEDIO DEL GUI
    //ESTAS FUNCIONES NO RESPONDEN A LOS BOTONES DEL UI, SOLAMENTE FUNCIONAN DE MANERA INTERNA
    Ui::MainWindow *ui; //Puntero a la interfaz generada (NO MOVER)

    //FUNCIONES DE RETORNO
    //vectores que utilizara
    int ignoredTasks;
    string mainTableName;
    vector<Columna> column;
    vector<ErrorLexico> errLexico;
    vector<ErrorSintactico> errSintax;

    //Funciones a utilizar
    string mayorCarga(); //mayor carga por usuario
    int returnEncargados(); // retorno de numero de encargados
    int returnTarea(); //retorno de tareas
    //
    QString archivoActual;
    //NEW REPORTES
    QString todo_Reporte(); //REPORTE DE TAREAS
    QString carga_per_user(); //REPORTE DE CARGA POR USUARIO
    QString lexicalErrors(); // REPORTE DE ERRORES
    QString dotFile(); //ARBOL SINTAXIS .DOT
    QString clearCorrupted(const string& texto); //limpiar comillas pedorras

    void welcomeMsg();
    void setDisable();
    void setupTablaTokens();
    void setupTablaErrores();
    void setupTablaProduccion();
    void setupTablaErroresS();
    void aplicarEstilos(); //ESTILOS
    void imprimirConsola();
    void conectarUI();

};
#endif // MAINWINDOW_H
