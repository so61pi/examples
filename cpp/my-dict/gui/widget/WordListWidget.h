#ifndef WORD_LIST_WIDGET_H
#define WORD_LIST_WIDGET_H


#include <memory>
#include <vector>
#include <QTreeWidgetItem>
#include <QWidget>

#include "DB/Database.h"


namespace Ui {
    class WordListWidget;
}


class WordListWidget : public QWidget
{
    Q_OBJECT


public:
    explicit WordListWidget(QWidget *parent);
    ~WordListWidget();

    void SetWordList(const std::vector<mydict::Word>& wordList);
    
    void Clear();

    int64_t GetSelectedWordId() const;


private slots:
    void OnWordClicked(QTreeWidgetItem *item, int column);


private: // data
    std::unique_ptr<Ui::WordListWidget> ui;
};


#endif // WORD_LIST_WIDGET_H
