#include "MainWindow.h"

#include "../core/LexicalAnalyzer.h"
#include "../core/Token.h"
#include "../core/ErrorManager.h"
#include <QString>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QEvent>
#include <QDebug>
#include <fstream>

void exportReport(const std::vector<Token>& tokens, const std::vector<LexicalError>& errors);

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
    auto *topWidget = new QWidget(this);
    auto *topBar = new QHBoxLayout(topWidget);

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

    rootLayout->addWidget(topWidget);
    rootLayout->addWidget(new QLabel("Codigo fuente MedLang", this));
    rootLayout->addWidget(sourceEditor_, 3);
    rootLayout->addWidget(new QLabel("Tabla de tokens", this));
    rootLayout->addWidget(tokensTable_, 2);
    rootLayout->addWidget(new QLabel("Tabla de errores", this));
    rootLayout->addWidget(errorsTable_, 2);
    

    central->setLayout(rootLayout);

    setCentralWidget(central);

    connect(analyzeButton_, &QPushButton::clicked,
        this, &MainWindow::onAnalyzeClicked);

    connect(loadButton_, &QPushButton::clicked,
        this, &MainWindow::onLoadClicked);

}

void MainWindow::onAnalyzeClicked() {

    std::string source = sourceEditor_->toPlainText().toStdString();

    LexicalAnalyzer lexer(source);

    auto tokens = lexer.scanAllTokens();
    auto errors = lexer.getErrors();

    tokensTable_->setRowCount(0);
    errorsTable_->setRowCount(0);

    int row = 0;

    for (const auto &token : tokens) {
        if (token.type == TokenType::EndOfFile) continue;

        tokensTable_->insertRow(row);

        tokensTable_->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        tokensTable_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(token.lexeme)));
        tokensTable_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(tokenTypeToString(token.type))));
        tokensTable_->setItem(row, 3, new QTableWidgetItem(QString::number(token.line)));
        tokensTable_->setItem(row, 4, new QTableWidgetItem(QString::number(token.column)));

        row++;
    }

    row = 0;

    for (const auto &error : errors) {

        errorsTable_->insertRow(row);

        errorsTable_->setItem(row, 0, new QTableWidgetItem(QString::number(error.number)));
        errorsTable_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(error.lexeme)));
        errorsTable_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(error.type)));
        errorsTable_->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(error.description)));
        errorsTable_->setItem(row, 4, new QTableWidgetItem(QString::number(error.line)));
        errorsTable_->setItem(row, 5, new QTableWidgetItem(QString::number(error.column)));
        errorsTable_->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(error.severity)));

        row++;
    }

    statusLabel_->setText("Analisis completado");

    if (source.empty()) {
    statusLabel_->setText("Error: no hay contenido");
    return;
}

exportReport(tokens, errors);
system("start reporte.html");
}

void MainWindow::onLoadClicked() {

    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Abrir archivo .med",
        "",
        "Archivos MedLang (*.med)"
    );

    if (fileName.isEmpty()) return;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QString content = in.readAll();

    sourceEditor_->setText(content);

    file.close();

    if (!fileName.endsWith(".med")) {
    statusLabel_->setText("Error: archivo no es .med");
    return;
}

tokensTable_->setRowCount(0);
errorsTable_->setRowCount(0);
statusLabel_->setText("Archivo cargado");
}

void exportReport(const std::vector<Token>& tokens, const std::vector<LexicalError>& errors) {

    std::ofstream file("reporte.html");

    file << "<html><head><title>Reporte</title></head><body>";

    file << "<h1>Tabla de Tokens</h1><table border='1'>";
    file << "<tr><th>No</th><th>Lexema</th><th>Tipo</th></tr>";

    int i = 1;
    for (const auto &t : tokens) {
        if (t.type == TokenType::EndOfFile) continue;

        file << "<tr>";
        file << "<td>" << i++ << "</td>";
        file << "<td>" << t.lexeme << "</td>";
        file << "<td>" << tokenTypeToString(t.type) << "</td>";
        file << "</tr>";
    }

    file << "</table>";

    file << "<h1>Errores</h1><table border='1'>";
    file << "<tr><th>No</th><th>Descripcion</th></tr>";

    i = 1;
    for (const auto &e : errors) {
        file << "<tr style='color:red'>";
        file << "<td>" << i++ << "</td>";
        file << "<td>" << e.description << "</td>";
        file << "</tr>";
    }

    file << "</table>";

    file << "</body></html>";
    file.close();
}
