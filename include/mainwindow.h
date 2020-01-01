#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <array>
#include <memory>
#include <QMainWindow>
#include <QFont>

// Forward declarations
namespace Ui {class MainWindow;}
class MyLineEdit;
class QGridLayout;
namespace Sudoku {enum class Difficulty;}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow* ui;
    Sudoku::Difficulty dif;
    std::array<std::array<MyLineEdit*,9>,9> btn_storage;
    std::array<std::array<int, 9>, 9> grid;

    QGridLayout* layout;
    QFont serifFont;


public:
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    ~MainWindow();

private slots:
    void about();
    void cell_changed(std::size_t row, std::size_t col);
    void new_game();
    void beginner();
    void intermediate();
    void hard();
    void solve();

private:
    void init_board();
    void create_puzzle();
    void highlight_cell(std::size_t row, std::size_t col);
    void restore_cell(std::size_t row, std::size_t col);
    void clear_highlights();
    void cell_font_blue(std::size_t row, std::size_t col);
    void cell_font_black(std::size_t row, std::size_t col);
    void clear_all();

};

#endif // MAINWINDOW_H
