#include <QApplication>
#include <QTranslator>
#include <mainwindow.h>

#include <iostream>

int main(int argc, char* argv[])
{
    QApplication app (argc, argv);

    QTranslator translator;

    // Find our locale and check if there is a translation available
    QString defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));

    if (translator.load(QString("translation_") + defaultLocale))
            app.installTranslator(&translator);

    //Run
    MainWindow win;
    win.setWindowTitle("Sudoku");
    win.move(200, 200);
    win.show();
    
    return app.exec();
}
