#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <array>
#include <memory>
#include <QMainWindow>
#include <QFont>

#include "my_types.h"

// Forward declarations
namespace Ui {class MainWindow;}
class MyLineEdit;
class QGridLayout;


class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow* ui;
    Difficulty dif;
    std::array<std::array<MyLineEdit*,9>,9> btn_storage;
    Puzzle_t grid;

    QGridLayout* layout;
    QFont serifFont;


public:
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    ~MainWindow();

private slots:
    void about();
    void cell_changed(Row row, Col col);
    void new_game();
    void beginner();
    void intermediate();
    void hard();
    void solve();

private:
    void init_board();
    void create_puzzle();
    void highlight_cell(Row row, Col col);
    void restore_cell(Row row, Col col);
    void clear_highlights();
    void cell_font_blue(Row row, Col col);
    void cell_font_black(Row row, Col col);
    void clear_all();

};

#endif // MAINWINDOW_H
