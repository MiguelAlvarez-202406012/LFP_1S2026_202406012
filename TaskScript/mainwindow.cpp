#include "mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QGroupBox>
#include <QHeaderView>
#include <QFont>
#include <QDateTime>
#include "LexicalAnalyzer.h"
#include <string>
#include <iostream>
#include <set>
#include <QMessageBox> //POPUP
#include <QFile>//GENERAR
#include <QTextStream>//OUTPUT O INPUT
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QRegularExpression>



#include "mainwindow.h"
#include "./ui_mainwindow.h"
int conflicts;
MainWindow::MainWindow(QWidget *parent) //CONSTRUCTOR DE UI (SE PUEDEN EJECUTRA METODOS)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) //Inicia UI
{
    ui->setupUi(this);
    setWindowTitle("TaskScript Reader 0.2.0");
    conectarUI();
    setupTablaErrores();
    setupTablaTokens();
    setupTablaErroresS();
    this->setFixedSize(1095 , 536);
}



void MainWindow::conectarUI(){
    connect(ui->btn_load, &QPushButton::clicked, this, &MainWindow::cargarArchivo);
    connect(ui->btn_analizar, &QPushButton::clicked, this, &MainWindow::analizarArchivo);
    connect(ui->btnReporte1, &QPushButton::clicked, this, &MainWindow::abrirReporte1);
    connect(ui->btnReporte2, &QPushButton::clicked, this, &MainWindow::abrirReporte2);
    connect(ui->btnReporte5, &QPushButton::clicked, this, &MainWindow::abrirReporte5);
    //connect funciona para vincular botones del la Interfaz grafica y que hagan una accioin
    //que se definio en el header de private slots

    for (auto *btn : {ui->btnReporte1, ui->btnReporte2, ui->btnReporte5}) {
        btn->setEnabled(false);
    }
    ui->btn_analizar->setEnabled(false);
}

void MainWindow::setupTablaTokens(){
    ui->tbl_tokens->setColumnCount(5);
    ui->tbl_tokens->setHorizontalHeaderLabels({"#", "Lexema", "Tipo de Token", "Linea", "Columna"});
    ui->tbl_tokens->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tbl_tokens->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tbl_tokens->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tbl_tokens->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbl_tokens->setAlternatingRowColors(true);
    ui->tbl_tokens->verticalHeader()->setVisible(false);
}

void MainWindow::setupTablaErrores(){
    ui->tbl_errors->setColumnCount(5);
    ui->tbl_errors->setHorizontalHeaderLabels({"#", "Lexema", "Tipo de Error", "Descripcion", "Linea", "Columna"});
    ui->tbl_errors->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tbl_errors->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tbl_errors->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbl_errors->setAlternatingRowColors(true);
    ui->tbl_errors->verticalHeader()->setVisible(false);
}
void MainWindow::setupTablaErroresS(){
    ui->tbl_sError->setColumnCount(3);
    ui->tbl_sError->setHorizontalHeaderLabels({"linea","columna","Tipo de Error"});
    ui->tbl_sError->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tbl_sError->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tbl_sError->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tbl_sError->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tbl_sError->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbl_sError->setAlternatingRowColors(true);
    ui->tbl_sError->verticalHeader()->setVisible(false);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::cargarArchivo() {
    QString ruta = QFileDialog::getOpenFileName(
        this, "Abrir archivo .task", "", "Archivos Task PTM (*.task);;Todos los archivos (*)");

    if (ruta.isEmpty()) return;
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
        return;
    }


    QTextStream in(&archivo);
    ui->txt_codigoFuente->setText(in.readAll());
    archivo.close();

    archivoActual = ruta;
    ui->label_fileName->setText(QFileInfo(ruta).fileName());
    ui->label_fileName->setStyleSheet("color: #a6e3a1; font-weight: bold;");
    ui->btn_analizar->setEnabled(true);

    // Limpiar resultados anteriores
    ui->tbl_tokens->setRowCount(0);
    ui->tbl_errors->setRowCount(0);
    for (auto *btn : {ui->btnReporte1, ui->btnReporte2, ui->btnReporte5})
        btn->setEnabled(false);

    statusBar()->showMessage("Archivo cargado: " + ruta + " — Presiona 'Analizar' para comenzar.");
}

void MainWindow::analizarArchivo() { //ALTERACION
    if (archivoActual.isEmpty()) return;


    /// lectura de contenido
    /// PARA LA LECTURA DEL CONTENIDO
    // Leer contenido del archivo
    std::string contenido = ui->txt_codigoFuente->toPlainText().toStdString(); //ruta

    LexicalAnalyzer lexer(contenido);
    std::vector<Token> tokens = lexer.tokenize(); //traer la tokenizacion

    //GUARDAR DE LEXER A MAIN, LECTURA DE VECTORES DE LEXICAL ANALIZER A MAINWINDOW,GENERACION DE TOKENS
    column = lexer.columnStorage;
    errLexico = lexer.errores_L;
    errSintax = lexer.errores_S;
    //DEBEN IR EN ORDEN

    // Llenar tabla de tokens
    ui->tbl_tokens->setRowCount(0);
    for (int i = 0; i < (int)tokens.size(); i++) {
        ui->tbl_tokens->insertRow(i);
        ui->tbl_tokens->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui->tbl_tokens->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(tokens[i].lexema)));
        ui->tbl_tokens->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(tokenTypeToString(tokens[i].tipo))));
        ui->tbl_tokens->setItem(i, 3, new QTableWidgetItem(QString::number(tokens[i].linea)));
        ui->tbl_tokens->setItem(i, 4, new QTableWidgetItem(QString::number(tokens[i].columna)));
    }

    // Poblar tabla de errores
    ui->tbl_errors->setRowCount(0);
    for (int i = 0; i < (int)lexer.errores_L.size(); i++) {
        auto& e = lexer.errores_L[i];
        ui->tbl_errors->insertRow(i);
        ui->tbl_errors->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui->tbl_errors->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(e.lexema)));
        ui->tbl_errors->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(e.tipoError)));
        ui->tbl_errors->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(e.descripcion)));
        ui->tbl_errors->setItem(i, 4, new QTableWidgetItem(QString::number(e.linea)));
        ui->tbl_errors->setItem(i, 5, new QTableWidgetItem(QString::number(e.columna)));
    }

    ui->tbl_sError->setRowCount(0);
    //poblar tabla de errores S
    for(int i = 0 ; i < (int)lexer.errores_S.size();i++){
        auto& e = lexer.errores_S[i];
        ui->tbl_sError->insertRow(i);
        ui->tbl_sError->setItem(i,0,new QTableWidgetItem(QString::number(e.linea)));
        ui->tbl_sError->setItem(i,1,new QTableWidgetItem(QString::number(e.columna)));
        ui->tbl_sError->setItem(i,2,new QTableWidgetItem(QString::fromStdString(e.descripcion)));
    }

    for (auto *btn : {ui->btnReporte1, ui->btnReporte2, ui->btnReporte5})
        btn->setEnabled(true);


    int nTokens = tokens.size();
    int nErrores = lexer.errores_L.size();
    int sErrores = lexer.errores_S.size();
    statusBar()->showMessage(QString("Analisis completado — %1 tokens, %2 errores léxicos, %3 errores Sintacticos")
                                 .arg(nTokens).arg(nErrores).arg(sErrores));

    QMessageBox::information(this, "Lectura compeltada!","Lectura de archivo .task completado!");
}

QString MainWindow::clearCorrupted(const string& texto){
    QString result = QString::fromStdString(texto);


    result.replace("\"", "\\\"");
    result.replace("\\", "\\\\");
    result.replace("\n", "\\n");
    result.replace("\r", "");
    result.remove(QRegularExpression("[\\x00-\\x1F\\x7F]"));

    return result;

}


//


////////////////! REPORTE GENERAR POR ENCARGAD////////////////////////////////////////////////////

int manyFilesLoad = 0; //Numero de archivos Creados
bool itExistsLoad = false; //Si el archivo existe para errores lexicos
void MainWindow::abrirReporte2() { //GENERAR REPORTE DE ENCARGADOS

    qInfo() << "Generando reporte de Cargas por Encargado ";
    if (archivoActual.isEmpty()) {
        qWarning() << "ERROR: No hay archivo cargado";
        QMessageBox::warning(this, "Error", "Primero carga un archivo .med");
        return;
    }

    if(itExistsLoad == false){
        if(!archivoActual.isEmpty()){
            QString ruta = QFileInfo(archivoActual).absolutePath();
            QString path = ruta + "/r_incharge.html"; //Ruta donde se almacenara

            QDir dir(ruta);
            if(!dir.exists()){
                QMessageBox::critical(this, "Error", "La ruta: " + ruta + " No existe");
                return; //defunde la fucnbion
            }
            QString htmlContents = carga_per_user(); //genera el reporte con datos
            QFile archivo(path); //guarda el archivo
            if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
                QTextStream out(&archivo);
                out << htmlContents;
                archivo.close();
                //POPUP
                QMessageBox::information(this, "Citas!","Reporte de Errores Lexicos y Sintacticos generado correctamente.");
                manyFilesLoad++;
                itExistsLoad = true;
            }else{
                QMessageBox::information(this, "ERROR, Reporte E Lexicos!","Ocurrio un error al GENERAR el archivo");
            }
        }


    }else{
        QMessageBox::information(this, "Advertencia, Archivo numero: " + QString::number(manyFilesLoad),"El archivo que intenta crear ya existe, Se generarar otro archivo nuevo.");
        if(!archivoActual.isEmpty()){
            QString ruta = QFileInfo(archivoActual).absolutePath();
            QString path = ruta + "/r_incharge"  + QString::number(manyFilesLoad) +  ".html"; //Ruta donde se almacenara

            QDir dir(ruta);
            if(!dir.exists()){
                QMessageBox::critical(this, "Error", "La ruta: " + ruta + " No existe");
                return; //defunde la fucnbion
            }

            QString htmlContents = carga_per_user(); //genera el reporte con datos
            QFile archivo(path); //guarda el archivo
            if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
                QTextStream out(&archivo);
                out << htmlContents;
                archivo.close();
                //POPUP
                QMessageBox::information(this, "Archivo Generado!","Reporte de Errores Lexicos y Sintacticos generado correctamente.");
                manyFilesLoad++;
            }else{
                QMessageBox::information(this, "ERROR, Reporte E Lexicos!","Ocurrio un error al GENERAR el archivo");
            }
        }
    }

}

/////////////////////////////// GENERADOR DE REPORTE: ERRORES ///////////////////////////////////////////////////////////////////////////
int manyFilesErrors = 0; //Numero de archivos Creados
bool itExistsErrors = false; //Si el archivo existe para errores lexicos
void MainWindow::abrirReporte5() { //REPORTE DE ERRORES LEXICOS + ARBOL


    qInfo() << "Generando reporte de Errores lexicos y Sintacticos ";


    if (archivoActual.isEmpty()) {
        qWarning() << "ERROR: No hay archivo cargado";
        QMessageBox::warning(this, "Error", "Primero carga un archivo .med");
        return;
    }

    if(itExistsErrors == false){
        if(!archivoActual.isEmpty()){
            QString ruta = QFileInfo(archivoActual).absolutePath();
            QString path = ruta + "/r_errors.html"; //Ruta donde se almacenara

            QDir dir(ruta);
            if(!dir.exists()){
                QMessageBox::critical(this, "Error", "La ruta: " + ruta + " No existe");
                return; //defunde la fucnbion
            }
            QString htmlContents = lexicalErrors(); //genera el reporte con datos
            QFile archivo(path); //guarda el archivo
            if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
                QTextStream out(&archivo);
                out << htmlContents;
                archivo.close();
                //POPUP
                QMessageBox::information(this, "Citas!","Reporte de Errores Lexicos y Sintacticos generado correctamente.");
                manyFilesErrors++;
                itExistsErrors = true;
            }else{
                QMessageBox::information(this, "ERROR, Reporte E Lexicos!","Ocurrio un error al GENERAR el archivo");
            }
        }


    }else{
        QMessageBox::information(this, "Advertencia, Archivo numero: " + QString::number(manyFilesErrors),"El archivo que intenta crear ya existe, Se generarar otro archivo nuevo.");
        if(!archivoActual.isEmpty()){
            QString ruta = QFileInfo(archivoActual).absolutePath();
            QString path = ruta + "/r_errors_"  + QString::number(manyFilesErrors) +  ".html"; //Ruta donde se almacenara

            QDir dir(ruta);
            if(!dir.exists()){
                QMessageBox::critical(this, "Error", "La ruta: " + ruta + " No existe");
                return; //defunde la fucnbion
            }

            QString htmlContents = lexicalErrors(); //genera el reporte con datos
            QFile archivo(path); //guarda el archivo
            if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
                QTextStream out(&archivo);
                out << htmlContents;
                archivo.close();
                //POPUP
                QMessageBox::information(this, "Archivo Generado!","Reporte de Errores Lexicos y Sintacticos generado correctamente.");
                manyFilesErrors++;
            }else{
                QMessageBox::information(this, "ERROR, Reporte E Lexicos!","Ocurrio un error al GENERAR el archivo");
            }
        }
    }



    //Generar Reporte de Errores lexicos
}


/////////////////////////////// REPORTE: KANBAN ///////////////////////////////////////////////////////////////////////////
int manyFilesKAN = 0; //Numero de archivos Creados
bool itExistsKAN = false; //Si el archivo existe para errores lexicos
void MainWindow::abrirReporte1() { //GENERAR REPORTE KANBAN
    //REPORTE DE CITAS

    //GENERAR CITAS
    qInfo() << "Generando reporte KANBAN ";

    if (archivoActual.isEmpty()) {
        qWarning() << "ERROR: No hay archivo cargado";
        QMessageBox::warning(this, "Error", "Primero carga un archivo .med");
        return;
    }

    if(itExistsKAN == false){
        if(!archivoActual.isEmpty()){
            QString ruta = QFileInfo(archivoActual).absolutePath();
            QString path = ruta + "/r_KANBAN.html"; //Ruta donde se almacenara

            QDir dir(ruta);
            if(!dir.exists()){
                QMessageBox::critical(this, "Error", "La ruta: " + ruta + " No existe");
                return; //defunde la fucnbion
            }
            QString htmlContents = todo_Reporte(); //genera el reporte con datos
            QFile archivo(path); //guarda el archivo
            if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
                QTextStream out(&archivo);
                out << htmlContents;
                archivo.close();
                //POPUP
                QMessageBox::information(this, "KANBAN!","Reporte de KANBAN generado correctamente.");
                manyFilesKAN++;
                itExistsKAN = true;
            }else{
                QMessageBox::information(this, "ERROR, Reporte KANBAN!","Ocurrio un error al GENERAR el archivo");
            }
        }


    }else{
        QMessageBox::information(this, "Advertencia, Archivo numero: " + QString::number(manyFilesKAN),"El archivo que intenta crear ya existe, Se generarar otro archivo nuevo.");
        if(!archivoActual.isEmpty()){
            QString ruta = QFileInfo(archivoActual).absolutePath();
            QString path = ruta + "/r_KANBAN"  + QString::number(manyFilesKAN) +  ".html"; //Ruta donde se almacenara

            QDir dir(ruta);
            if(!dir.exists()){
                QMessageBox::critical(this, "Error", "La ruta: " + ruta + " No existe");
                return; //defunde la fucnbion
            }

            QString htmlContents = todo_Reporte(); //genera el reporte con datos
            QFile archivo(path); //guarda el archivo
            if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
                QTextStream out(&archivo);
                out << htmlContents;
                archivo.close();
                //POPUP
                QMessageBox::information(this, "Citas!","Reporte de Errores Lexicos y Sintacticos generado correctamente.");
                manyFilesKAN++;
            }else{
                QMessageBox::information(this, "ERROR, Reporte E Lexicos!","Ocurrio un error al GENERAR el archivo");
            }
        }
    }


}

/////////////////////////////// REPORTE: MEDICOS Y CARGA ///////////////////////////////////////////////////////////////////////////

QString MainWindow::todo_Reporte(){ //CREA HTML PARA TAREAS KANBAN
    QString html = R"(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Reporte KANBAN</title>
<style>
 body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 40px;
            background-color: #373737;
            color: #ffffff;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background-color: rgb(15, 15, 15);
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            padding: 30px;
        }
        h1 {
            color: #ba6824;
            border-bottom: 3px solid #ff7b00da;
            padding-bottom: 10px;
        }
        h2{  color: #e1af52; border-bottom: 3px solid #ff7b00da; padding-bottom: 10px; }

        .fecha {
            color: #7f8c8d;
            text-align: right;
            margin-bottom: 20px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        th {
            background-color: #e87714;
            color: white;
            padding: 12px;
            text-align: left;
        }

        td {
            padding: 10px;
            border-bottom: 1px solid #ddd;
        }
        tr:hover {
            background-color: #41362c;
        }

        .kanban-board {
                display: grid;
                grid-template-columns: repeat(3, 1fr);
                gap: 20px;
                padding: 20px;
                background: #ff7b00;
                font-family: 'Segoe UI', sans-serif;
        }

        .kanban-column {
            background: #2e2e2e;
            border-radius: 10px;
            padding: 12px;
            height: fit-content;
        }

        .column-header {
            padding: 10px;
            margin-bottom: 15px;
        }

        .column-header h2 {
            font-size: 1.1em;
            margin: 0;
            color: #e7873e;
        }

        .badge {
            background: #422c090f;
            padding: 2px 8px;
            border-radius: 20px;
            font-size: 0.8em;
            margin-left: 8px;
        }

        .task-list {
            display: flex;
            flex-direction: column;
            gap: 12px;
        }

        .task-card {
            background: rgb(255, 177, 128);
            border-radius: 8px;
            padding: 12px;
            box-shadow: 0 1px 3px rgba(0,0,0,0.1);
            border-left: 4px solid;
            transition: transform 0.2s;
        }

        .task-card:hover {
            transform: translateY(-2px);
            box-shadow: 0 4px 8px rgba(58, 5, 22, 0.15);
        }

        .task-title {
            font-weight: bold;
            font-size: 1em;
            margin-bottom: 8px;
            color: #ffffff;
        }

        .priority-high {
            border-left-color: #ff0015;
        }
        .priority-high .task-priority {
            color: #ff0015;
            background: #7f050f36;
        }

        .priority-medium {
            border-left-color: #74461c;
        }
        .priority-medium .task-priority {
            color: #ffa502;
            background: #73410e2d;
        }

        .priority-low {
            border-left-color: #3af636;
        }
        .priority-low {
            color:  #3af636;
            background: #2ed5731a;
        }

        .task-priority {
            display: inline-block;
            font-size: 0.7em;
            font-weight: bold;
            padding: 2px 8px;
            border-radius: 20px;
            margin: 6px 0;
        }

        .task-date, .task-assignee  {
            display: inline-block;
            font-size: 0.7em;
            font-weight: bold;
            padding: 2px 8px;
            border-radius: 20px;
            margin: 6px 0;

            color: #ffffff;
            margin-top: 6px;

        }
    </style>
</head>
<body>
    <div class="container">
        <h1> KANBAN </h1>
        <div class="fecha">
            Fecha de generación: )" + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + R"(
        </div>
        <h2>todas las tareas se registran en el tablero KANBAN</h2>
        <div class='kanban-board'>
            )";
            //COLECCIONAR POR PRIORIDAD
            vector<Tarea> low;
            vector<Tarea> medium;
            vector<Tarea> high;
            int manyTareas_low = 0;
            int manyTareas_med = 0;
            int manyTareas_high = 0;

            /*recorrer el vector de contenedor y registrar por prioridad, luego pegarlos en sus respectivas cartas
                ,luego realizar por segmentos la asignacion de cartas
            */
            for(int p = 0; p < column.size() ; p++){ //lectura por cada columna
                qDebug() << "columna actual" << column[p].Columna;
                for(int t = 0; t < column[p].tarea_column.size() ; t++){ //lectura para cada tarea de una columna
                    string priority = column[p].tarea_column[t].nivel_prioridad;
                    if(priority == "BAJA"){
                        low.push_back(column[p].tarea_column[t]);
                        qDebug() << "Se Agrego la tarea con Prioridad ALTA";
                        manyTareas_low++;
                    }else if(priority == "MEDIA"){
                        medium.push_back(column[p].tarea_column[t]);
                        qDebug() << "Se Agrego la tarea con Prioridad MEDIA";
                         manyTareas_med++;
                    }else if(priority == "ALTA"){
                        high.push_back(column[p].tarea_column[t]);
                        qDebug() << "Se Agrego la tarea con Prioridad BAJA";
                        manyTareas_high++;
                    }else{

                    }


                }
            }
            html += R"(

            <div class="kanban-column">
                <div class="column-header">
                    <h2>PRIORIDAD BAJA
                     )";
                    html += "<span class='badge'>" +  QString::fromStdString("No. Tareas: ") + QString::number(manyTareas_low) + "</span>";
                    //Salir
                    html+= R"(
                    </h2>
                     </div>


                    )";
                    //GENERAR UNA TARJETA PARA PRIORIDAD BAJA
                    for (int l =0; l < low.size();l++){
                        html += "<div class='task-card priority-low'>";
                            html += "<div class='task-title'>" + QString::fromStdString(low[l].nombre_tarea) +"</div>";
                            html += "<div class='task-priority priority-low'>" + QString::fromStdString(low[l].nivel_prioridad) +"</div>";
                            html += "<div class='task-date'>" + QString::fromStdString(low[l].fecha_limit) +"</div>";
                            html += "<div class='task-assignee'>" + QString::fromStdString(low[l].responsable) +"</div>";
                        html += "</div>";

                    }
            html +=R"(
             </div>
            <div class="kanban-column">
                <div class="column-header">
                    <h2>PRIORIDAD MEDIA
                     )";
                    html += "<span class='badge'>" +  QString::fromStdString("No. Tareas: ") + QString::number(manyTareas_med) + "</span>";
                    //Salir
                    html+= R"(
                    </h2>
                </div>
                    )";
                    //GENERAR UNA TARJETA PARA PRIORIDAD BAJA
                    for (int l =0; l < medium.size();l++){
                        html += "<div class='task-card priority-medium'>";
                        html += "<div class='task-title'>" + QString::fromStdString(medium[l].nombre_tarea) +"</div>";
                        html += "<div class='task-priority priority-medium'>" + QString::fromStdString(medium[l].nivel_prioridad) +"</div>";
                        html += "<div class='task-date'>" + QString::fromStdString(medium[l].fecha_limit) +"</div>";
                        html += "<div class='task-assignee'>" + QString::fromStdString(medium[l].responsable) +"</div>";
                        html += "</div>";

                    }
                    html +=R"(
                </div>
            <div class="kanban-column">
                <div class="column-header">
                    <h2>PRIORIDAD ALTA
                     )";
                    html += "<span class='badge'>" +  QString::fromStdString("No. Tareas: ") + QString::number(manyTareas_high) + "</span>";
                    //Salir
                    html+= R"(
                    </h2>
                </div>
                    )";
                    //GENERAR UNA TARJETA PARA PRIORIDAD BAJA
                    for (int l =0; l < high.size();l++){
                        html += "<div class='task-card priority-high'>";
                        html += "<div class='task-title'>" + QString::fromStdString(high[l].nombre_tarea) +"</div>";
                        html += "<div class='task-priority priority-high'>" + QString::fromStdString(high[l].nivel_prioridad) +"</div>";
                        html += "<div class='task-date'>" + QString::fromStdString(high[l].fecha_limit) +"</div>";
                        html += "<div class='task-assignee'>" + QString::fromStdString(high[l].responsable) +"</div>";
                        html += "</div>";

                    }
                    html +=R"(
                 </div>

                 </div>
            </div>
        </div>
    </div>
</body>
</html>
    )";

    return html;


}


QString MainWindow::carga_per_user(){
    QString html = R"(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Reporte por Carga</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 40px;
            background-color: #373737;
            color: #ffffff;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background-color: rgb(15, 15, 15);
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            padding: 30px;
        }
        h1 {
            color: #ba6824;
            border-bottom: 3px solid #ff7b00da;
            padding-bottom: 10px;
        }
        h2{  color: #e1af52; border-bottom: 3px solid #ff7b00da; padding-bottom: 10px; }

        .fecha {
            color: #7f8c8d;
            text-align: right;
            margin-bottom: 20px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        th {
            background-color: #e87714;
            color: white;
            padding: 12px;
            text-align: center;
        }
        thead{
            text-align: center;
        }
        td, th {
            padding: 10px;
            border-bottom: 1px solid #ddd;
            text-align: center;

        }
        tr:hover {
            background-color: #41362c;
        }
        .total {
            margin-top: 20px;
            font-weight: bold;
            color: #50402c;
        }

    </style>
</head>
<body>
    <div class="container">
        <h1> Carga Por persona</h1>
        <div class="fecha">
            Fecha de generación: )" + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + R"(
        </div>

        <table>
            <thead>
                <tr>
                    <th>encargado</th>
                    <th>Tareas asignadas</th>
                    <th>Prioridad Baja</th>
                    <th>Prioridad Media</th>
                    <th>Prioridad Alta</th>
                    <th>Carga</th>

                </tr>
            </thead>
        <tbody>
    )";
    //ADENTRO DEL TBODY
    //Contenido por codigo
    //RECORRE LA COLUMNAS Y TOMA A CADA UNO DE LOS ENCARGADOS Y POR MAP IRA INCREMENTANDO EL NUMERO DE TAREAS
    //SEGUN EL NUMERO DE TAREAS TENDRA SU NIVEL DE PRIORIDAD
    set<string> responsables;
    unordered_map<string, vector<int>> conteoPrioridad;  // [0]=ALTA, [1]=MEDIA, [2]=BAJA
    unordered_map<string, int> totalTareasPorResponsable;

    // Recorrer todas las columnas y tareas para recolectar datos
    for (int col = 0; col < (int)column.size(); col++) {
        for (int tarea = 0; tarea < (int)column[col].tarea_column.size(); tarea++) {
            string responsable = column[col].tarea_column[tarea].responsable;
            string prioridad = column[col].tarea_column[tarea].nivel_prioridad;

            // Insertar responsable en el set
            responsables.insert(responsable);

            // Inicializar mapa si es primera vez
            if (conteoPrioridad.find(responsable) == conteoPrioridad.end()) {
                conteoPrioridad[responsable] = {0, 0, 0};  // [ALTA, MEDIA, BAJA]
                totalTareasPorResponsable[responsable] = 0;
            }

            // Incrementar contador según prioridad
            if (prioridad == "ALTA") {
                conteoPrioridad[responsable][0]++;
            } else if (prioridad == "MEDIA") {
                conteoPrioridad[responsable][1]++;
            } else if (prioridad == "BAJA") {
                conteoPrioridad[responsable][2]++;
            }

            totalTareasPorResponsable[responsable]++;
        }
    }

    int maxTareas = 0;
    for (const auto& [responsable, total] : totalTareasPorResponsable) {
        if (total > maxTareas) maxTareas = total;
    }

    // Iterar sobre los responsables (ordenados por set)
    for (const string& responsable : responsables) {
        int total = totalTareasPorResponsable[responsable];
        int alta = conteoPrioridad[responsable][0];
        int media = conteoPrioridad[responsable][1];
        int baja = conteoPrioridad[responsable][2];
        QString nivelCarga;
        QString colorCarga;
        double porcentaje = (maxTareas > 0) ? (double)total / maxTareas * 100 : 0;

        if (total == 0) {
            nivelCarga = "Sin tareas";
            colorCarga = "gray";
        } else if (total <= 2) {
            nivelCarga = "BAJA";
            colorCarga = "#2ed573";
        } else if (total <= 5) {
            nivelCarga = "MEDIA";
            colorCarga = "#ffa502";
        } else if (total <= 8) {
            nivelCarga = "ALTA";
            colorCarga = "#ff7f00";
        } else {
            nivelCarga = "SATURADA";
            colorCarga = "#ff0015";
        }



        if(responsable == "" || responsable.empty()){
        html += "<td>" + QString::fromStdString("Sin RESPONSABLE!") + "<td>\n"; //Emcargado
        }else{
        html += "<td>" + QString::fromStdString(responsable) + "<td>\n"; //Emcargado
        }
        html += "<td>" + QString::number(total) + "</td>\n"; //total asignado
        html += "<td>" + QString::number(baja) + "</td>\n"; //bajo
        html += "<td>" + QString::number(media) + "</td>\n"; //media
        html += "<td>" + QString::number(alta) + "</td>\n";//alta
        html += "<td>" + nivelCarga + " (total: " + QString::number(total) + ")</td>\n";
        html += "</tr>";
    }

    html += R"(
            </tbody>
        </table>

    </div>
</body>
</html>


    )";

    return html;
}


QString MainWindow::lexicalErrors(){
    QString html = R"(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Reporte de Errores Lexicos y Sintacticos</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 40px;
            background-color: #373737;
            color: #ffffff;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background-color: rgb(15, 15, 15);
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            padding: 30px;
        }
        h1 {
            color: #ba6824;
            border-bottom: 3px solid #ff7b00da;
            padding-bottom: 10px;
        }
        h2{  color: #e1af52; border-bottom: 3px solid #ff7b00da; padding-bottom: 10px; }

        .fecha {
            color: #7f8c8d;
            text-align: right;
            margin-bottom: 20px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        th {
            background-color: #e87714;
            color: white;
            padding: 12px;
            text-align: left;
        }
        .CRIT{
            background-color: #FF0000;
            color: white;
            padding: 12px;
            text-align: left;
        }
        td {
            padding: 10px;
            border-bottom: 1px solid #ddd;
        }
        tr:hover {
            background-color: #41362c;
        }
        .total {
            margin-top: 20px;
            font-weight: bold;
            color: #50402c;
        }

    </style>
</head>
<body>
    <h2>Errores encontrados durante el analisis de los archivos .task</h2>
    <div class="container">
        <h1>ERRORES LEXICOS</h1>
        <div class="fecha">
            Fecha de generación: )" + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + R"(
        </div>
        <table>
            <thead>
                <tr>
                    <th>No.</th>
                    <th>Lexema Intro.</th>
                    <th>Error</th>
                    <th>Descripcion</th>
                    <th>Linea </th>
                    <th>Columna </th>
                    <th>Gravedad </th>

                </tr>
            </thead>
        <tbody>
    )";
    //Iteracion para Errores lexicos
    for(int e =0; e< errLexico.size() ; e++ ){
        html += "            <td>" + QString::number(e + 1) + "</td>\n"; //NUMERO
        html += "            <td>" + QString::fromStdString(errLexico[e].lexema) + "</td>\n";
        html += "            <td>" + QString::fromStdString(errLexico[e].tipoError) + "</td>\n";
        html += "            <td>" + QString::fromStdString(errLexico[e].descripcion) + "</td>\n";
        html += "            <td>" + QString::number(errLexico[e].linea) + "</td>\n";
        html += "            <td>" + QString::number(errLexico[e].columna) + "</td>\n";
        html += "            <td>" + QString::fromStdString("CRITICO") + "</td class='CRIT'>\n";
        html += "        </tr>\n";

    }

    html += R"(
            </tbody>
        </table>
    </div>
    <div class="container">
        <h1>ERRORES SINTACTICOS</h1>
        <table>
            <thead>
                <tr>
                    <th>No.</th>
                    <th>Descripcion</th>

                </tr>
            </thead>
        <tbody>
    )";
    //Iteracion para Errores lexicos
    for(int e =0; e< errSintax.size() ; e++ ){
        html += "            <td>" + QString::number(e + 1) + "</td>\n"; //NUMERO
        html += "            <td>" + QString::fromStdString(errSintax[e].descripcion) + "</td>\n";

        html += "        </tr>\n";

    }

    html += R"(
            </tbody>
        </table>
    </div>

</body>
</html>
    )";

    return html;


}





