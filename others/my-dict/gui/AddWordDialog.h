#ifndef ADD_WORD_DIALOG_H
#define ADD_WORD_DIALOG_H


#include <memory>
#include <QDialog>

#include "Db/Database.h"


namespace Ui {
    class AddWordDialog;
};


class AddWordDialog : public QDialog
{
    Q_OBJECT


public:
    explicit AddWordDialog(QWidget *parent);
    ~AddWordDialog();

    mydict::Word GetWord() const;


private slots:
    void OnOK();


private: // data
    std::unique_ptr<Ui::AddWordDialog> ui;
};


#endif // ADD_WORD_DIALOG_H
