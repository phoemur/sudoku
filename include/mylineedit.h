#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>

class MyLineEdit : public QLineEdit
{

    Q_OBJECT

    bool editable = true;

public:
    MyLineEdit(QWidget* parent = nullptr);

    bool isEditable() const {return editable;}
    void setEditable(bool a) {editable = a;}

    std::size_t row;
    std::size_t col;

public slots:
    void back_cursor();

signals:
    void changed(std::size_t row, std::size_t col);

protected:
    virtual void keyPressEvent(QKeyEvent* e) override;

};

#endif // MYLINEEDIT_H
