#include "mylineedit.h"

MyLineEdit::MyLineEdit(QWidget* parent)
    : QLineEdit(parent)
{
    setReadOnly(true);
}

void MyLineEdit::back_cursor()
{
    this->setCursorPosition(0);
}

void MyLineEdit::keyPressEvent(QKeyEvent *e)
{
    if (isEditable())
    {
        setReadOnly(false);
        QLineEdit::keyPressEvent(e);
        setReadOnly(true);

        if (this->text().size() > 0)
            emit(changed(row, col));
    }
}
