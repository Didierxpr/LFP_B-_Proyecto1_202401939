#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTextEdit;
class QTableWidget;
class QPushButton;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    private slots:
    void onAnalyzeClicked();
    void onLoadClicked();

private:
    void setupUi();

    QTextEdit *sourceEditor_;
    QTableWidget *tokensTable_;
    QTableWidget *errorsTable_;
    QPushButton *loadButton_;
    QPushButton *analyzeButton_;
    QLabel *statusLabel_;
};



#endif
