#ifndef EDIT_WORD_DIALOG_H
#define EDIT_WORD_DIALOG_H


#include <memory>
#include <QDialog>

#include "Db/Database.h"


namespace Ui {
    class EditWordDialog;
};


class EditWordDialog : public QDialog
{
    Q_OBJECT


public:
    EditWordDialog(QWidget *parent, mydict::Word& word);
    ~EditWordDialog();


private slots:
    void OnOK();


private: // data
    std::unique_ptr<Ui::EditWordDialog> ui;
    mydict::Word& m_word;
};


#endif // EDIT_WORD_DIALOG_H
