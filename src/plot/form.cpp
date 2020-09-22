#include "form.h"
#include "ui_form.h"

#include "vapplication.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    ui->listWidget->addItems(
    {
                    "THRESH_BINARY",
                    "THRESH_BINARY_INV",
                    "THRESH_TRUNC",
                    "THRESH_TOZERO",
                    "THRESH_TOZERO_INV",
                    "THRESH_MASK",
                    "THRESH_OTSU",
                    "THRESH_TRIANGLE",
                } );
}

Form::~Form()
{
    delete ui;
}

void Form::on_horizontalSlider_valueChanged(int value)
{
    emit trackbar( value );
}

void Form::on_listWidget_itemActivated(QListWidgetItem *item)
{}

void Form::on_listWidget_itemChanged(QListWidgetItem *item)
{}


void Form::on_listWidget_itemClicked(QListWidgetItem *item)
{
    emit type( item->text().toStdString() );
}
