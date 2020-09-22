#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QListWidgetItem>

#include "vsignal.h"

#include <string>

namespace Ui {
    class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

signals:

    void trackbar( const uint8_t& val );
    void type( const std::string& data );

private slots:
    void on_horizontalSlider_valueChanged( int value );

    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_listWidget_itemChanged(QListWidgetItem *item);

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::Form *ui;
};

#endif // FORM_H
