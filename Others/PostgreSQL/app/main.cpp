#include <cstdlib>
#include <exception>

#include <QtCore>
#include <QtDebug>
#include <QtSql>

// Document
// https://doc.qt.io/qt-5/sql-programming.html


int main() {
    try {
        auto const user = getenv("POSTGRES_USER");
        auto const password = getenv("POSTGRES_PASSWORD");
        auto const dbname = user; // By default, db name is same as user.

        if (!user || !password) {
            qCritical() << "POSTGRES_USER or POSTGRES_PASSWORD is not available";
            return 1;
        }

        // PostgreSQL
        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
        db.setHostName("db");
        db.setDatabaseName(dbname);
        db.setUserName(user);
        db.setPassword(password);

        if (!db.open()) {
            qCritical() << "Cannot open database";
            qDebug() << db.lastError();
            return 1;
        }

        QSqlQuery query("SELECT * FROM PRODUCTS");
        while (query.next()) {
            auto const id = query.value(0).toInt();
            auto const name = query.value(1).toString();
            auto const price = query.value(2).toString();
            qInfo() << id << " - " << name << " - " << price;
        }

        if (db.transaction()) {
            QSqlQuery query;
            query.prepare("INSERT INTO PRODUCTS(ID, NAME, PRICE) VALUES(:ID, :NAME, :PRICE)");
            query.bindValue(":ID", 4);
            query.bindValue(":NAME", "D");
            query.bindValue(":PRICE", "4.4");
            query.exec();
            db.commit();
            qDebug() << db.lastError();
        }

        return 0;
    } catch (std::exception const& e) {
        return 1;
    }
}
