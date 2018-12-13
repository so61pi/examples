#ifndef MANAGE_DICTIONARY_DIALOG_H
#define MANAGE_DICTIONARY_DIALOG_H


#include <memory>
#include <QDialog>

#include "DB/Database.h"


namespace Ui {
    class ManageDictionaryDialog;
}


class ManageDictionaryDialog : public QDialog
{
    Q_OBJECT


public:
    ManageDictionaryDialog(QWidget *parent, mydict::Database& db);
    ~ManageDictionaryDialog();


private slots:
    void OnAddDictionary();
    void OnEditDictionary();
    void OnRemoveDictionary();
    void OnDictionaryChanged(int dictionaryId);


private:
    void ReloadDictionaryList();


private: // data
    std::unique_ptr<Ui::ManageDictionaryDialog> ui;

    mydict::Database& m_db;
    int m_currentDictionaryId = -1;
};


#endif // MANAGE_DICTIONARY_DIALOG_H
