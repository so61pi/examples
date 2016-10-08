#ifndef EDITOR_H
#define EDITOR_H


#include <QPlainTextEdit>


class LineNumberArea;


class Editor : public QPlainTextEdit {
    Q_OBJECT

public:
    Editor(QWidget* parent = 0);
    ~Editor();

    void lineNumberAreaPaintEvent(QPaintEvent* e);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent* e) Q_DECL_OVERRIDE;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRect const& rect, int dy);

private:
    QWidget* mLineNumberArea;
};


class LineNumberArea : public QWidget {
public:
    LineNumberArea(Editor* editor)
        : QWidget(editor), mEditor(editor) {}

    QSize sizeHint() const Q_DECL_OVERRIDE {
        return QSize(mEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent* e) Q_DECL_OVERRIDE {
        mEditor->lineNumberAreaPaintEvent(e);
    }

private:
    Editor* mEditor;
};


#endif // EDITOR_H

