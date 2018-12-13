#include "HighlightCurrentLine.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HighlightCurrentLine w;
    w.show();

    return a.exec();
}
