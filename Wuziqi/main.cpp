#include "index.h"
#include"chessboard.h"
#include <QApplication>
#include <QtMultimedia>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    index indexpage;
    indexpage.show();

    return app.exec();
}
