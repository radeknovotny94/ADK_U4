#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"

#include <QFileDialog>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    ui->Canvas->setAB();
}

void Widget::on_pushButton_4_clicked()
{
    ui->Canvas->clearAll();

    repaint();
}

void Widget::on_pushButton_2_clicked()
{
    //Perform boolean operation

    TBooleanOperation operation;
    switch(ui->comboBox->currentIndex())
    {
        case 0: operation = INTERSECTION; break;
        case 1: operation = UNION; break;
        case 2: operation = DIFFAB; break;
        case 3: operation = DIFFBA;
    }

    // Get those polygons
    std::vector<QPointFB> A = ui->Canvas->getA();
    std::vector<QPointFB> B = ui->Canvas->getB();

    std::vector<std::vector<QPointFB>> result = Algorithms::BooleanOper(A, B, operation);

    ui->Canvas->setRes(result);

    repaint();
}

void Widget::on_pushButton_3_clicked()
{

}

void Widget::on_pushButton_5_clicked()
{

}

void Widget::on_load_pol_clicked()
{
    //get path to directory upper of build
    QDir current_path = QDir::currentPath();
    current_path.cdUp();
    QString path = current_path.path();

    //open file dialog
    QString point_path_a = QFileDialog::getOpenFileName(
                this,
                tr("Select file with POLYGON A"),
                path,
                "Text file (*.txt);;All files (*.*)");

    QString point_path_b = QFileDialog::getOpenFileName(
                this,
                tr("Select file with POLYGON B"),
                path,
                "Text file (*.txt);;All files (*.*)");

    //convert path from QString to string (change coding to utf8 for ifstream)
    std::string point_path_a_utf8 = point_path_a.toUtf8().constData();
    std::string point_path_b_utf8 = point_path_b.toUtf8().constData();

    QSizeF canvas_size = ui->Canvas->size();

    ui->Canvas->loadPoints(point_path_a_utf8, point_path_b_utf8, canvas_size); //load
    repaint();
}
