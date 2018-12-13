#ifndef WORD_WIDGET_H
#define WORD_WIDGET_H


#include <memory>
#include <QWidget>

#include "DB/Database.h"


namespace Ui {
    class WordWidget;
};


class WordWidget : public QWidget
{
    Q_OBJECT


public:
    explicit WordWidget(QWidget *parent);
    ~WordWidget();

    void SetWord(const mydict::Word& word);
    mydict::Word GetWord() const;

    void Clear();


private: // data
    std::unique_ptr<Ui::WordWidget> ui;
};


#endif // WORD_WIDGET_H
