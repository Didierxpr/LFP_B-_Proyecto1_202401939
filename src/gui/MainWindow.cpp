#include "MainWindow.h"

#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      sourceEditor_(nullptr),
      tokensTable_(nullptr),
      errorsTable_(nullptr),
      loadButton_(nullptr),
      analyzeButton_(nullptr),
      statusLabel_(nullptr) {
    setupUi();
}

void MainWindow::setupUi() {
    setWindowTitle("MedLexer - Proyecto 1");
    resize(1200, 760);

    auto *central = new QWidget(this);
    auto *rootLayout = new QVBoxLayout(central);
    auto *topBar = new QHBoxLayout();

    loadButton_ = new QPushButton("Cargar .med", this);
    analyzeButton_ = new QPushButton("Analizar", this);
    statusLabel_ = new QLabel("Listo para iniciar", this);

    topBar->addWidget(loadButton_);
    topBar->addWidget(analyzeButton_);
    topBar->addStretch();
    topBar->addWidget(statusLabel_);

    sourceEditor_ = new QTextEdit(this);
    sourceEditor_->setPlaceholderText("Aqui se cargara el contenido del archivo .med");

    tokensTable_ = new QTableWidget(this);
    tokensTable_->setColumnCount(5);
    tokensTable_->setHorizontalHeaderLabels({"No.", "Lexema", "Tipo", "Linea", "Columna"});
    tokensTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    errorsTable_ = new QTableWidget(this);
    errorsTable_->setColumnCount(7);
    errorsTable_->setHorizontalHeaderLabels(
        {"No.", "Lexema", "Tipo Error", "Descripcion", "Linea", "Columna", "Gravedad"});
    errorsTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    rootLayout->addLayout(topBar);
    rootLayout->addWidget(new QLabel("Codigo fuente MedLang", this));
    rootLayout->addWidget(sourceEditor_, 3);
    rootLayout->addWidget(new QLabel("Tabla de tokens", this));
    rootLayout->addWidget(tokensTable_, 2);
    rootLayout->addWidget(new QLabel("Tabla de errores", this));
    rootLayout->addWidget(errorsTable_, 2);

    setCentralWidget(central);
}
