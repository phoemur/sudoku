#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mylineedit.h"
#include "solver.hpp"

#include <QMessageBox>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow{}), btn_storage{}, grid{}, layout{nullptr}, serifFont{"Times", 13, QFont::Bold}
{
    for (auto& row : btn_storage)
        row.fill(nullptr);

    for (auto& row : grid)
        row.fill(0);

    ui->setupUi(this);

    init_board();

    grid = {{{3, 0, 6, 5, 0, 8, 4, 0, 0},
                                 {5, 2, 0, 0, 0, 0, 0, 0, 0},
                                 {0, 8, 7, 0, 0, 0, 0, 3, 1},
                                 {0, 0, 3, 0, 1, 0, 0, 8, 0},
                                 {9, 0, 0, 8, 6, 3, 0, 0, 5},
                                 {0, 5, 0, 0, 9, 0, 6, 0, 0},
                                 {1, 3, 0, 0, 0, 0, 2, 5, 0},
                                 {0, 0, 0, 0, 0, 0, 0, 7, 4},
                                 {7, 0, 5, 2, 0, 6, 3, 0, 0}}};

    create_puzzle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::about()
{
    QMessageBox::information(this, tr("About"),
                             tr("Sudoku version 0.1\nAuthor: Fernando B. Giannasi\njan/2020"));
}

void MainWindow::cell_changed(std::size_t row, std::size_t col)
{
    clear_highlights();
    int val = btn_storage[row][col]->text().toInt();

    auto opt = Sudoku::UsedInRow(grid, row, val);
    if (opt.has_value())
    {
        highlight_cell(opt.value().first,opt.value().second);
        btn_storage[row][col]->clear();
    }

    auto opt2 = Sudoku::UsedInCol(grid, col, val);
    if (opt2.has_value())
    {
        highlight_cell(opt2.value().first,opt2.value().second);
        btn_storage[row][col]->clear();
    }

    auto opt3 = Sudoku::UsedInBox(grid, row - row%3 , col - col%3, val);
    if (opt3.has_value())
    {
        highlight_cell(opt3.value().first,opt3.value().second);
        btn_storage[row][col]->clear();
    }

    if (opt.has_value() || opt2.has_value() || opt3.has_value())
        grid[row][col] = 0;
    else
        grid[row][col] = val;

    if (!Sudoku::FindUnassignedLocation(grid).has_value())
    {
        QMessageBox::information(this, tr("Congratulations!"),
                                 tr("You won!"));

        for (auto& row : btn_storage)
            for (auto ptr : row)
                ptr->setEditable(false);
    }
}

void MainWindow::init_board()
{
    layout = new QGridLayout(ui->centralwidget);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

    // This lambda creates a line break
    auto add_line = [this](std::size_t i){
        for (int n = 0; n < 11; ++n)
        {
            auto line = new QFrame(this->ui->centralwidget);
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Raised);
            line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            line->setFixedHeight(10);
            this->layout->addWidget(line, static_cast<int>(i), static_cast<int>(n));
        }
    };


    // Now create the board
    for (std::size_t i = 0, x = 0; i < 11; ++i)
    {
        if (i == 3 || i == 7)
        {
            ++x; // Increase offset
            add_line(i);
        }
        else
        {
            for (std::size_t j = 0, y = 0; j < 11; ++j)
            {
                if (j == 3 || j == 7) // Vertical line
                {
                    ++y; // Increase offset
                    auto line = new QFrame(ui->centralwidget);
                    line->setFrameShape(QFrame::VLine);
                    line->setFrameShadow(QFrame::Raised);
                    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                    line->setFixedWidth(10);
                    layout->addWidget(line, static_cast<int>(i), static_cast<int>(j));
                }
                else // Create a Cell
                {
                    auto btn = new MyLineEdit(ui->centralwidget);
                    btn->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
                    btn->setInputMask("D");
                    btn->setMaxLength(1);
                    btn->setFont(serifFont);
                    btn->setCursor(Qt::PointingHandCursor);
                    //btn->setEditable(false);
                    //btn->setText(QString::number(i-x) + " , " + QString::number(j-y));
                    btn_storage[i-x][j-y] = btn;
                    btn->row = i-x;
                    btn->col = j-y;
                    layout->addWidget(btn_storage[i-x][j-y], static_cast<int>(i), static_cast<int>(j));

                    connect(btn, &QLineEdit::cursorPositionChanged, btn, &MyLineEdit::back_cursor);
                    connect(btn, &MyLineEdit::changed, this, &MainWindow::cell_changed);
                }
            }
        }
    }
}

void MainWindow::create_puzzle()
{
    for (std::size_t i = 0; i < 9; ++i)
        for (std::size_t j = 0; j < 9; ++j)
            if (grid[i][j] != 0)
            {
                btn_storage[i][j]->setEditable(false);
                btn_storage[i][j]->setText(QString::number(grid[i][j]));
                cell_font_blue(i, j);
            }
}

void MainWindow::highlight_cell(std::size_t row, std::size_t col)
{
    auto btn = btn_storage[row][col];
    auto palette = btn->palette();
    palette.setColor(QPalette::Base, Qt::red);
    btn->setPalette(palette);
}

void MainWindow::restore_cell(std::size_t row, std::size_t col)
{
    auto btn = btn_storage[row][col];
    auto palette = btn->palette();
    palette.setColor(QPalette::Base, Qt::white);
    btn->setPalette(palette);
}

void MainWindow::clear_highlights()
{
    for (std::size_t i = 0; i < 9; ++i)
        for (std::size_t j = 0; j < 9; ++j)
            restore_cell(i, j);
}

void MainWindow::cell_font_blue(std::size_t row, std::size_t col)
{
    auto btn = btn_storage[row][col];
    auto palette = btn->palette();
    palette.setColor(QPalette::Text, Qt::blue);
    btn->setPalette(palette);
}

void MainWindow::cell_font_black(std::size_t row, std::size_t col)
{
    auto btn = btn_storage[row][col];
    auto palette = btn->palette();
    palette.setColor(QPalette::Text, Qt::black);
    btn->setPalette(palette);
}
