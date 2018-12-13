#ifndef EDIT_DICTIONARY_DIALOG_H
#define EDIT_DICTIONARY_DIALOG_H


#include <memory>
#include <QDialog>

#include "DB/Database.h"


namespace Ui {
    class EditDictionaryDialog;
};


class EditDictionaryDialog : public QDialog
{
    Q_OBJECT


public:
    EditDictionaryDialog(QWidget *parent, mydict::Dictionary& dictionary);
    ~EditDictionaryDialog();
    

private slots:
    void OnOK();


private: // data
    std::unique_ptr<Ui::EditDictionaryDialog> ui;
    mydict::Dictionary& m_dictionary;
};


#endif // EDIT_DICTIONARY_DIALOG_H
