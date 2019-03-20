#ifndef HIGHLIGHTCURRENTLINE_H
#define HIGHLIGHTCURRENTLINE_H


#include <QPlainTextEdit>


class HighlightCurrentLine : public QPlainTextEdit {
    Q_OBJECT

public:
    HighlightCurrentLine(QPlainTextEdit* parent = 0);
    ~HighlightCurrentLine();

private slots:
    void highlight();
};


#endif // HIGHLIGHTCURRENTLINE_H
