#include "HighlightCurrentLine.h"
#include <QtDebug>


HighlightCurrentLine::HighlightCurrentLine(QPlainTextEdit* parent) : QPlainTextEdit(parent) {
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlight()));

    // highlight right after creating widget
    highlight();
}


HighlightCurrentLine::~HighlightCurrentLine() = default;


void HighlightCurrentLine::highlight() {
    QList<QTextEdit::ExtraSelection> selections;

    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(QColor(Qt::gray).lighter(120));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    selections.push_back(selection);

    setExtraSelections(selections);
}
