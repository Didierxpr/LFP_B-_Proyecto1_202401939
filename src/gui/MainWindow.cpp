#include "MainWindow.h"

#include "../core/LexicalAnalyzer.h"
#include "../core/Token.h"
#include "../core/ErrorManager.h"
#include "../core/MedLangParser.h"
#include "../core/models/Patient.h"
#include "../core/models/Doctor.h"
#include "../core/models/Appointment.h"
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

void exportReport(const std::vector<Token>& tokens, const std::vector<LexicalError>& errors,
                  const std::vector<Patient>& patients, const std::vector<Doctor>& doctors,
                  const std::vector<Appointment>& appointments);

void exportDotFile(const std::vector<Patient>& patients, const std::vector<Doctor>& doctors,
                   const std::vector<Appointment>& appointments);

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

    rootLayout->addWidget(new QLabel("Pacientes", this));
    patientsTable_ = new QTableWidget(this);
    patientsTable_->setColumnCount(3);
    patientsTable_->setHorizontalHeaderLabels({"Nombre", "Edad", "Tipo Sangre"});
    patientsTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    rootLayout->addWidget(patientsTable_, 1);

    rootLayout->addWidget(new QLabel("Doctores", this));
    doctorsTable_ = new QTableWidget(this);
    doctorsTable_->setColumnCount(2);
    doctorsTable_->setHorizontalHeaderLabels({"Nombre", "Especialidad"});
    doctorsTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    rootLayout->addWidget(doctorsTable_, 1);

    rootLayout->addWidget(new QLabel("Citas", this));
    appointmentsTable_ = new QTableWidget(this);
    appointmentsTable_->setColumnCount(4);
    appointmentsTable_->setHorizontalHeaderLabels({"Paciente", "Doctor", "Fecha", "Hora"});
    appointmentsTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    rootLayout->addWidget(appointmentsTable_, 1);

    central->setLayout(rootLayout);

    setCentralWidget(central);

    connect(analyzeButton_, &QPushButton::clicked,
        this, &MainWindow::onAnalyzeClicked);

    connect(loadButton_, &QPushButton::clicked,
        this, &MainWindow::onLoadClicked);

}

void MainWindow::onAnalyzeClicked() {
    std::string source = sourceEditor_->toPlainText().toStdString();

    if (source.empty()) {
        statusLabel_->setText("Error: no hay contenido");
        return;
    }

    LexicalAnalyzer lexer(source);
    auto tokens = lexer.scanAllTokens();
    auto errors = lexer.getErrors();

    tokensTable_->setRowCount(0);
    errorsTable_->setRowCount(0);
    patientsTable_->setRowCount(0);
    doctorsTable_->setRowCount(0);
    appointmentsTable_->setRowCount(0);

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

    MedLangParser parser(tokens);
    parser.parse();

    row = 0;
    for (const auto &patient : parser.getPatients()) {
        patientsTable_->insertRow(row);
        patientsTable_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(patient.name)));
        patientsTable_->setItem(row, 1, new QTableWidgetItem(QString::number(patient.age)));
        patientsTable_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(patient.bloodType)));
        row++;
    }

    row = 0;
    for (const auto &doctor : parser.getDoctors()) {
        doctorsTable_->insertRow(row);
        doctorsTable_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(doctor.name)));
        doctorsTable_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(doctor.specialty)));
        row++;
    }

    row = 0;
    for (const auto &appointment : parser.getAppointments()) {
        appointmentsTable_->insertRow(row);
        appointmentsTable_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(appointment.patient)));
        appointmentsTable_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(appointment.doctor)));
        appointmentsTable_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(appointment.date)));
        appointmentsTable_->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(appointment.time)));
        row++;
    }

    statusLabel_->setText("Analisis completado");
    exportReport(tokens, errors, parser.getPatients(), parser.getDoctors(), parser.getAppointments());
    exportDotFile(parser.getPatients(), parser.getDoctors(), parser.getAppointments());
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

void exportReport(const std::vector<Token>& tokens, const std::vector<LexicalError>& errors,
                  const std::vector<Patient>& patients, const std::vector<Doctor>& doctors,
                  const std::vector<Appointment>& appointments) {

    std::ofstream file("reporte.html");

    file << "<html><head><title>Reporte MedLexer</title><style>";
    file << "body { font-family: Arial, sans-serif; margin: 20px; }";
    file << "h1 { color: #333; border-bottom: 2px solid #007bff; padding-bottom: 10px; }";
    file << "table { border-collapse: collapse; width: 100%; margin: 20px 0; }";
    file << "th, td { border: 1px solid #ddd; padding: 12px; text-align: left; }";
    file << "th { background-color: #007bff; color: white; }";
    file << "tr:nth-child(even) { background-color: #f9f9f9; }";
    file << "tr:hover { background-color: #f0f0f0; }";
    file << ".error { color: red; font-weight: bold; }";
    file << "</style></head><body>";

    file << "<h1>Reporte de Análisis MedLang</h1>";

    // Tabla de Tokens
    file << "<h2>Tabla de Tokens</h2><table border='1'>";
    file << "<tr><th>No</th><th>Lexema</th><th>Tipo</th><th>Línea</th><th>Columna</th></tr>";

    int i = 1;
    for (const auto &t : tokens) {
        if (t.type == TokenType::EndOfFile) continue;

        file << "<tr>";
        file << "<td>" << i++ << "</td>";
        file << "<td>" << t.lexeme << "</td>";
        file << "<td>" << tokenTypeToString(t.type) << "</td>";
        file << "<td>" << t.line << "</td>";
        file << "<td>" << t.column << "</td>";
        file << "</tr>";
    }

    file << "</table>";

    // Tabla de Errores
    file << "<h2>Errores</h2><table border='1'>";
    file << "<tr><th>No</th><th>Lexema</th><th>Descripción</th><th>Línea</th><th>Columna</th><th>Gravedad</th></tr>";

    i = 1;
    for (const auto &e : errors) {
        file << "<tr class='error'>";
        file << "<td>" << i++ << "</td>";
        file << "<td>" << e.lexeme << "</td>";
        file << "<td>" << e.description << "</td>";
        file << "<td>" << e.line << "</td>";
        file << "<td>" << e.column << "</td>";
        file << "<td>" << e.severity << "</td>";
        file << "</tr>";
    }

    file << "</table>";

    // Tabla de Pacientes
    file << "<h2>Pacientes</h2><table border='1'>";
    file << "<tr><th>Nombre</th><th>Edad</th><th>Tipo de Sangre</th></tr>";

    for (const auto &p : patients) {
        file << "<tr>";
        file << "<td>" << p.name << "</td>";
        file << "<td>" << p.age << "</td>";
        file << "<td>" << p.bloodType << "</td>";
        file << "</tr>";
    }

    file << "</table>";

    // Tabla de Doctores
    file << "<h2>Doctores</h2><table border='1'>";
    file << "<tr><th>Nombre</th><th>Especialidad</th></tr>";

    for (const auto &d : doctors) {
        file << "<tr>";
        file << "<td>" << d.name << "</td>";
        file << "<td>" << d.specialty << "</td>";
        file << "</tr>";
    }

    file << "</table>";

    // Tabla de Citas
    file << "<h2>Citas</h2><table border='1'>";
    file << "<tr><th>Paciente</th><th>Doctor</th><th>Fecha</th><th>Hora</th></tr>";

    for (const auto &a : appointments) {
        file << "<tr>";
        file << "<td>" << a.patient << "</td>";
        file << "<td>" << a.doctor << "</td>";
        file << "<td>" << a.date << "</td>";
        file << "<td>" << a.time << "</td>";
        file << "</tr>";
    }

    file << "</table>";

    file << "</body></html>";
    file.close();
}

void exportDotFile(const std::vector<Patient>& patients, const std::vector<Doctor>& doctors,
                   const std::vector<Appointment>& appointments) {

    std::ofstream file("relaciones.dot");

    file << "digraph Hospital {" << std::endl;
    file << "    rankdir=LR;" << std::endl;
    file << "    node [shape=box, style=rounded, color=lightblue, fontname=Arial];" << std::endl;
    file << "    edge [fontname=Arial];" << std::endl << std::endl;

    // Nodo Hospital
    file << "    Hospital [label=\"Hospital\", color=lightgreen, style=\"rounded,filled\"];" << std::endl << std::endl;

    // Nodos Pacientes
    file << "    // Pacientes" << std::endl;
    int patientCount = 1;
    for (const auto &p : patients) {
        std::string patientId = "Patient_" + std::to_string(patientCount);
        file << "    \"" << patientId << "\" [label=\"" << p.name << "\\nEdad: " << p.age << "\\n" << p.bloodType << "\", color=lightyellow];" << std::endl;
        file << "    Hospital -> \"" << patientId << "\" [label=\"Paciente\"];" << std::endl;
        patientCount++;
    }

    file << std::endl << "    // Doctores" << std::endl;
    // Nodos Doctores
    int doctorCount = 1;
    for (const auto &d : doctors) {
        std::string doctorId = "Doctor_" + std::to_string(doctorCount);
        file << "    \"" << doctorId << "\" [label=\"" << d.name << "\\n" << d.specialty << "\", color=lightcoral];" << std::endl;
        doctorCount++;
    }

    file << std::endl << "    // Citas" << std::endl;
    // Relaciones de Citas
    int citaCount = 1;
    for (const auto &a : appointments) {
        // Buscar el índice del paciente correspondiente
        int patientIdx = 1;
        for (int i = 0; i < patients.size(); i++) {
            if (patients[i].name == a.patient) {
                patientIdx = i + 1;
                break;
            }
        }

        // Buscar el índice del doctor correspondiente
        int doctorIdx = 1;
        for (int i = 0; i < doctors.size(); i++) {
            if (doctors[i].name == a.doctor) {
                doctorIdx = i + 1;
                break;
            }
        }

        std::string citaId = "Appointment_" + std::to_string(citaCount);
        std::string patientId = "Patient_" + std::to_string(patientIdx);
        std::string doctorId = "Doctor_" + std::to_string(doctorIdx);

        file << "    \"" << citaId << "\" [label=\"Cita " << citaCount << "\\nFecha: " << a.date << "\\nHora: " << a.time << "\", color=lavender];" << std::endl;
        file << "    \"" << patientId << "\" -> \"" << citaId << "\" [label=\"Tiene\"];" << std::endl;
        file << "    \"" << citaId << "\" -> \"" << doctorId << "\" [label=\"Con\"];" << std::endl;

        citaCount++;
    }

    file << std::endl << "}" << std::endl;
    file.close();
}
