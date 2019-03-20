#ifndef ADD_DICTIONARY_DIALOG_H
#define ADD_DICTIONARY_DIALOG_H


#include <memory>
#include <QDialog>

#include "DB/Database.h"


namespace Ui {
    class AddDictionaryDialog;
};


class AddDictionaryDialog : public QDialog
{
    Q_OBJECT


public:
    explicit AddDictionaryDialog(QWidget *parent);
    ~AddDictionaryDialog();

    mydict::Dictionary GetDictionary() const;


private slots:
    void OnOK();


private: // data
    std::unique_ptr<Ui::AddDictionaryDialog> ui;
};


#endif // ADD_DICTIONARY_DIALOG_H
