#include "findwindow.hpp"
#include "ui_findwindow.h"

FindWindow::FindWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindWindow)
{
    ui->setupUi(this);
}

FindWindow::~FindWindow()
{
    delete ui;
}

void FindWindow::on_remove_clicked()
{
    this->remove = true;
    QMessageBox QMB;
    QMB.information(nullptr, "Sucesso", "Forma removida do banco de dados.");
    QMB.exec();
}

void FindWindow::setReg(DiskAPI::Registro* R){
    this->REG = R;
}

bool FindWindow::GetRemove(){
    return this->remove;
}