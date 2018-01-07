#ifndef IMPORT_DICTIONARY_DIALOG_H
#define IMPORT_DICTIONARY_DIALOG_H


#include <memory>
#include <vector>
#include <QDialog>

#include "Db/Database.h"


namespace Ui {
    class ImportDictionaryDialog;
};


class ImportDictionaryDialog : public QDialog
{
    Q_OBJECT


public:
    ImportDictionaryDialog(QWidget *parent, mydict::Database& db);
    ~ImportDictionaryDialog();


private slots:
    void OnSelectPath();
    void OnDictionaryClicked(int64_t dictionaryId);
    void OnOK();


private: // data
    std::unique_ptr<Ui::ImportDictionaryDialog> ui;
    mydict::Database& m_db;
    std::unique_ptr<mydict::Database> m_newDb;
};


#endif // IMPORT_DICTIONARY_DIALOG_H
