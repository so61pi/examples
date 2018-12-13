#ifndef DICTIONARYDIALOG_H
#define DICTIONARYDIALOG_H

#include <QDialog>

#include "DB/Database.h"

namespace Ui {
class DictionaryDialog;
}

class DictionaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DictionaryDialog(QWidget *parent, mydict::Dictionary& dictionary);
    ~DictionaryDialog();

    static bool editDictionary(QWidget *parent, mydict::Dictionary& dictionary);

private:
    bool validate();

private slots:
    void accept();

private:
    Ui::DictionaryDialog *ui;

    mydict::Dictionary& m_dictionary;
};

#endif // DICTIONARYDIALOG_H
