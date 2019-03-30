#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textBrowser->setText("abcdef\nhhhh\nhttp://google.com/");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnStartStopClicked()
{
    QMessageBox::about(this, "Title", "Text");
}

