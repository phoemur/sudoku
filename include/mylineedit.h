#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>

#include "my_types.h"

class MyLineEdit : public QLineEdit
{

    Q_OBJECT

    bool editable = true;

public:
    MyLineEdit(QWidget* parent = nullptr);

    bool isEditable() const {return editable;}
    void setEditable(bool a) {editable = a;}

    Row row;
    Col col;

public slots:
    void back_cursor();

signals:
    void changed(Row row, Col col);

protected:
    virtual void keyPressEvent(QKeyEvent* e) override;

};

#endif // MYLINEEDIT_H
