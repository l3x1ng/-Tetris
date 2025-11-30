#include "gui/TetrisGameWindow.h"
#include <QApplication>


//
// game 프로그램 그래픽 생성
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TetrisGameWindow w;
    w.show();

    return a.exec();
}
