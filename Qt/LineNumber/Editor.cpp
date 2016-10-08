#include "Editor.h"

#include <QtWidgets>
#include <QtDebug>


Editor::Editor(QWidget *parent)
    : QPlainTextEdit(parent), mLineNumberArea(new LineNumberArea(this))
{
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

    updateLineNumberAreaWidth(0);
}


Editor::~Editor() = default;


void Editor::lineNumberAreaPaintEvent(QPaintEvent* e) {
    QPainter painter(mLineNumberArea);
    painter.fillRect(e->rect(), Qt::lightGray);

    auto block = firstVisibleBlock();
    auto numBlock = block.blockNumber();
    auto top = blockBoundingGeometry(block).translated(contentOffset()).top();
    auto bottom = top + blockBoundingRect(block).height();

    while (block.isValid() && top <= e->rect().bottom()) {
        if (block.isVisible() && bottom >= e->rect().top()) {
            auto number = QString::number(numBlock + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, mLineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        ++numBlock;
    }
}


int Editor::lineNumberAreaWidth() {
    auto digits = 1;
    for (auto max = qMax(1, blockCount()); max >= 10; ++digits) {
        max /= 10;
    }

    auto space = 3 + fontMetrics().width(QLatin1Char('0')) * digits;
    return space;
}


void Editor::resizeEvent(QResizeEvent* e) {
    QPlainTextEdit::resizeEvent(e);

    auto cr = contentsRect();
    mLineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}


void Editor::updateLineNumberAreaWidth(int /*newBlockCount*/) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void Editor::updateLineNumberArea(QRect const& rect, int dy) {
    if (dy) {
        mLineNumberArea->scroll(0, dy);
    } else {
        mLineNumberArea->update(0, rect.y(), mLineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

