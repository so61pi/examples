#ifndef EXPORT_DICTIONARY_DIALOG_H
#define EXPORT_DICTIONARY_DIALOG_H


#include <memory>
#include <string>
#include <QDialog>

#include "DB\Database.h"


namespace Ui {
    class ExportDictionaryDialog;
};


class ExportDictionaryDialog : public QDialog
{
    Q_OBJECT


public:
    ExportDictionaryDialog(QWidget *parent, mydict::Database& db);
    ~ExportDictionaryDialog();


private slots:
    void OnSelectPath();
    void OnDictionaryClicked(int64_t dictionaryId);
    void OnOK();


private: // data
    std::unique_ptr<Ui::ExportDictionaryDialog> ui;
    mydict::Database& m_db;
};


#endif // EXPORT_DICTIONARY_DIALOG_H
