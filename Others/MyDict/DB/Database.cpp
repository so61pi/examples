#include "Database.h"

#include <cassert>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <boost/scope_exit.hpp>

#include "sqlite/sqlite3.h"


using namespace mydict;


class SQLiteError : public std::domain_error
{
    // using std::domain_error::domain_error;

public:
    explicit SQLiteError(const std::string& what) :
        std::domain_error(what)
    {}
};


namespace { namespace helper {
    namespace internal {
        void bind_value_guard(sqlite3_stmt *stmt, int col) {
            if (stmt == nullptr) {
                assert(false && "Invalid statement.");
                throw SQLiteError("Invalid statement.");
            }

            if (col <= 0) {
                assert(false && "Column must be greater than or equal to zero.");
                throw SQLiteError("Column must be greater than or equal to zero.");
            }
        }


        //
        // bind integral values
        //
        template<typename T, std::enable_if_t<std::is_same<T, int>::value> * = nullptr>
        void bind_value(sqlite3_stmt *stmt, int col, const T& val) {
            bind_value_guard(stmt, col);

            if (sqlite3_bind_int(stmt, col, val) != SQLITE_OK)
                throw SQLiteError("Cannot bind value.");
        }


        template<typename T, std::enable_if_t<std::is_same<T, int64_t>::value> * = nullptr>
        void bind_value(sqlite3_stmt *stmt, int col, const T& val) {
            bind_value_guard(stmt, col);

            if (sqlite3_bind_int64(stmt, col, val) != SQLITE_OK)
                throw SQLiteError("Cannot bind value.");
        }


        //
        // bind text values
        //
        template<typename T, std::enable_if_t<std::is_same<T, const char*>::value> * = nullptr>
        void bind_value(sqlite3_stmt *stmt, int col, const T& val) {
            bind_value_guard(stmt, col);

            if (sqlite3_bind_text(stmt, col, val, -1, nullptr) != SQLITE_OK)
                throw SQLiteError("Cannot bind value.");
        }


        template<typename T, std::enable_if_t<std::is_same<T, std::string>::value> * = nullptr>
        void bind_value(sqlite3_stmt *stmt, int col, const T& val) {
            bind_value_guard(stmt, col);

            if (sqlite3_bind_text(stmt, col, val.c_str(), val.size(), nullptr) != SQLITE_OK)
                throw SQLiteError("Cannot bind value.");
        }


        //
        void bind_values_impl(sqlite3_stmt *stmt, int col) {
            bind_value_guard(stmt, col);

            // template recursion ends here
        }


        template<typename First, typename... Rest>
        void bind_values_impl(sqlite3_stmt *stmt, int col, const First& val, const Rest&... rest) {
            bind_value_guard(stmt, col);

            // bind current value
            bind_value(stmt, col, val);

            // ... and next
            bind_values_impl(stmt, ++col, rest...);
        }


        //
        // by given values to sql statement
        //
        template<typename... Types>
        void bind_values(sqlite3_stmt *stmt, const Types&... args) {
            // don't need to check column here
            bind_value_guard(stmt, std::numeric_limits<int>::max());

            // bind values to statement
            bind_values_impl(stmt, 1, args...);
        }
    } // namespace internal


    namespace internal {
        void get_value_guard(sqlite3_stmt *stmt, int col) {
            if (stmt == nullptr) {
                assert(false && "Invalid statement.");
                throw SQLiteError("Invalid statement.");
            }

            if (col < 0) {
                assert(false && "Invalid column.");
                throw SQLiteError("Invalid column.");
            }
        }


        //
        // get value of integral types
        //
        template<typename T, std::enable_if_t<std::is_same<T, int>::value> * = nullptr>
        T get_value(sqlite3_stmt *stmt, int col) {
            get_value_guard(stmt, col);

            return sqlite3_column_int(stmt, col);
        }


        template<typename T, std::enable_if_t<std::is_same<T, int64_t>::value> * = nullptr>
        T get_value(sqlite3_stmt *stmt, int col) {
            get_value_guard(stmt, col);

            return sqlite3_column_int64(stmt, col);
        }


        //
        // get value of string types
        //
        template<typename T, std::enable_if_t<std::is_same<T, const char*>::value> * = nullptr>
        T get_value(sqlite3_stmt *stmt, int col) {
            get_value_guard(stmt, col);

            return reinterpret_cast<const char *>(sqlite3_column_text(stmt, col));
        }


        template<typename T, std::enable_if_t<std::is_same<T, std::string>::value> * = nullptr>
        T get_value(sqlite3_stmt *stmt, int col) {
            get_value_guard(stmt, col);

            return reinterpret_cast<const char *>(sqlite3_column_text(stmt, col));
        }


        // use struct to achieve partial specialization
        template<std::size_t Pos, typename... Types>
        struct get_values_impl {
            static void do_it(sqlite3_stmt *stmt, std::tuple<Types...>& result) {
                get_value_guard(stmt, Pos);

                using ty = std::tuple_element<Pos, std::tuple<Types...>>::type;
                std::get<Pos>(result) = get_value<ty>(stmt, Pos);

                get_values_impl<Pos - 1, Types...>::do_it(stmt, result);
            }
        };


        template<typename... Types>
        struct get_values_impl<-1, Types...> {
            static void do_it(sqlite3_stmt *stmt, std::tuple<Types...>& result) {
                // don't need to check column here
                get_value_guard(stmt, std::numeric_limits<int>::max());

                // template recursion ends here
            }
        };


        //
        // get all returned values from sql statement
        //
        template<typename... Types>
        std::tuple<Types...> get_values(sqlite3_stmt *stmt) {
            // don't need to check column here
            get_value_guard(stmt, std::numeric_limits<int>::max());

            std::tuple<Types...> result;
            get_values_impl<sizeof...(Types)-1, Types...>::do_it(stmt, result);
            return result;
        }
    } // namepsace internal


    template<typename... ReturnTypes>
    struct sql {
        //
        // run sql query and retrieve results
        //
        template<typename... Types>
        static std::vector<std::tuple<ReturnTypes...>> exec(
            sqlite3 *db,
            const std::string& query,
            const Types&... args)
        {
            static_assert(sizeof...(ReturnTypes) > 0, "Empty return types.");

            if (db == nullptr) {
                assert(false && "Invalid database.");
                throw SQLiteError("Invalid database.");
            }

            if (query.size() == 0) {
                assert(false && "Invalid query.");
                throw SQLiteError("Invalid query.");
            }

            sqlite3_stmt *stmt = nullptr;
            BOOST_SCOPE_EXIT_ALL(&) { sqlite3_finalize(stmt); };
            if (sqlite3_prepare_v2(db, query.c_str(), query.size(), &stmt, nullptr) != SQLITE_OK)
                throw SQLiteError("Cannot prepare query.");

            internal::bind_values(stmt, args...);

            std::vector<std::tuple<ReturnTypes...>> result;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                auto vals = internal::get_values<ReturnTypes...>(stmt);
                result.push_back(std::move(vals));
            }

            return result;
        }
    };


    template<>
    struct sql < > {
        //
        // run sql query
        //
        template<typename... Types>
        static void exec(sqlite3 *db, const std::string& query, const Types&... args) {
            if (db == nullptr) {
                assert(false && "Invalid database.");
                throw SQLiteError("Invalid database.");
            }

            if (query.size() == 0) {
                assert(false && "Invalid query.");
                throw SQLiteError("Invalid query.");
            }

            sqlite3_stmt *stmt = nullptr;
            BOOST_SCOPE_EXIT_ALL(&) { sqlite3_finalize(stmt); };
            if (sqlite3_prepare_v2(db, query.c_str(), query.size(), &stmt, nullptr) != SQLITE_OK)
                throw SQLiteError("Cannot prepare query.");

            internal::bind_values(stmt, args...);

            if (sqlite3_step(stmt) != SQLITE_DONE)
                throw SQLiteError("Cannot execute query.");
        }
    };
}} // namespace anonymous::helper


bool mydict::IsIdInValidRange(int64_t id) {
    return id > 0;
}


bool mydict::IsDictionaryContentValid(const Dictionary& dictionary) {
    if (dictionary.Name.size() > 0)
        return true;
    return false;
}


bool mydict::IsWordContentValid(const Word& word) {
    if (word.Name.size() > 0 || word.Examples.size() > 0)
        return true;
    return false;
}


class Database::Impl {
public:
    sqlite3 *m_db = nullptr;
};


Database::Database(const std::string& dbFile) {
    m_impl = std::make_unique<Impl>();

    try {
        if (sqlite3_open(dbFile.c_str(), &m_impl->m_db) != SQLITE_OK)
            throw SQLiteError("Cannot open database.");

        // create DICTIONARY table if it doesn't exist
        auto query =
            "CREATE TABLE IF NOT EXISTS"
            " DICTIONARY("
                "ID INTEGER PRIMARY KEY,"
                " NAME TEXT NOT NULL,"
                " DESCRIPTION TEXT"
            ");";
        
        helper::sql<>::exec(m_impl->m_db, query);

        // if there is no dictionary at all
        // add a default one
        if (GetDictionaryList().size() == 0) {
            query =
                "INSERT INTO DICTIONARY"
                " (NAME, DESCRIPTION)"
                " VALUES"
                " ('Default', 'Created automatically.');";
            helper::sql<>::exec(m_impl->m_db, query);
        }

        // create WORD table if it doesn't exist
        query =
            "CREATE TABLE IF NOT EXISTS"
            " WORD("
                "ID INTEGER PRIMARY KEY,"
                " NAME TEXT NOT NULL,"
                " DEFINITION TEXT,"
                " RELATED TEXT,"
                " EXAMPLES TEXT,"
                " DICTIONARY_ID INTEGER NOT NULL,"
                " FOREIGN KEY(DICTIONARY_ID) REFERENCES DICTIONARY(ID)"
            ");";
        helper::sql<>::exec(m_impl->m_db, query);
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot open database.");
    }
}


Database::~Database() {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    sqlite3_close(m_impl->m_db);
}


int64_t Database::AddDictionary(const Dictionary& dictionary) {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    if (!IsDictionaryContentValid(dictionary)) {
        assert(false && "Invalid dictionary.");
        throw std::invalid_argument("Invalid dictionary.");
    }

    try {
        helper::sql<>::exec(
            m_impl->m_db,
            "INSERT INTO DICTIONARY(NAME, DESCRIPTION) VALUES(?, ?);",
            dictionary.Name,
            dictionary.Description);

        auto lastId = sqlite3_last_insert_rowid(m_impl->m_db);
        assert(IsIdInValidRange(lastId) && "This id must be always valid.");
        return lastId;
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot add dictionary.");
    }
}


Dictionary Database::LoadDictionary(const int64_t dictionaryId) const {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    if (!IsIdInValidRange(dictionaryId)) {
        assert(false && "Invalid dictionary id.");
        throw std::invalid_argument("Invalid dictionary id.");
    }

    try {
        Dictionary dictionary;
        dictionary.Id = dictionaryId;

        auto list = helper::sql<std::string, std::string>::exec(
            m_impl->m_db,
            "SELECT NAME, DESCRIPTION FROM DICTIONARY WHERE ID = ?;",
            dictionaryId);

        // only 1 dictionary for 1 id
        if (list.size() == 0)
            throw DatabaseError("Dictionary doesn't exist.");
        else if (list.size() > 1)
            throw DatabaseError("Invalid database.");

        std::tie(dictionary.Name, dictionary.Description) = list.at(0);
        
        if (!IsIdInValidRange(dictionary.Id) || !IsDictionaryContentValid(dictionary))
            throw DatabaseError("Cannot load dictionary.");

        return dictionary;
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot load dictionary.");
    }
}


void Database::UpdateDictionary(const Dictionary& dictionary) {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    if (!IsIdInValidRange(dictionary.Id)) {
        assert(false && "Invalid dictionary id.");
        throw std::invalid_argument("Invalid dictionary id.");
    }
    if (!IsDictionaryContentValid(dictionary)) {
        assert(false && "Invalid dictionary.");
        throw std::invalid_argument("Invalid dictionary.");
    }

    try {
        helper::sql<>::exec(
            m_impl->m_db,
            "UPDATE DICTIONARY SET NAME = ?, DESCRIPTION = ? WHERE ID = ?;",
            dictionary.Name,
            dictionary.Description,
            dictionary.Id);
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot update dictionary.");
    }
}


void Database::RemoveDictionary(const int64_t dictionaryId) {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    if (!IsIdInValidRange(dictionaryId)) {
        assert(false && "Invalid dictionary id.");
        throw std::invalid_argument("Invalid dictionary id.");
    }

    try {
        // delete all words associated with given dictionary
        helper::sql<>::exec(
            m_impl->m_db,
            "DELETE FROM WORD WHERE DICTIONARY_ID = ?;",
            dictionaryId);

        // delete dictionary
        helper::sql<>::exec(
            m_impl->m_db,
            "DELETE FROM DICTIONARY WHERE ID = ?;",
            dictionaryId);
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot remove dictionary.");
    }
}


int64_t Database::AddWord(const int64_t dictionaryId, const Word& word) {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    if (!IsIdInValidRange(dictionaryId)) {
        assert(false && "Invalid dictionary id.");
        throw std::invalid_argument("Invalid dictionary id.");
    }
    if (!IsWordContentValid(word)) {
        assert(false && "Invalid word.");
        throw std::invalid_argument("Invalid word.");
    }

    try {
        helper::sql<>::exec(
            m_impl->m_db,
            "INSERT INTO WORD(NAME, DEFINITION, RELATED, EXAMPLES, DICTIONARY_ID) VALUES(?, ?, ?, ?, ?);",
            word.Name,
            word.Definition,
            word.Related,
            word.Examples,
            dictionaryId);

        auto lastId = sqlite3_last_insert_rowid(m_impl->m_db);
        assert(IsIdInValidRange(lastId) && "This id must be always valid.");
        return lastId;
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot add word.");
    }
}


Word Database::LoadWord(const int64_t wordId) const {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    if (!IsIdInValidRange(wordId)) {
        assert(false && "Invalid word id.");
        throw std::invalid_argument("Invalid word id.");
    }

    try {
        Word word;
        word.Id = wordId;

        auto list = helper::sql<std::string, std::string, std::string, std::string>::exec(
            m_impl->m_db,
            "SELECT NAME, DEFINITION, RELATED, EXAMPLES FROM WORD WHERE ID = ?;",
            wordId);

        // only 1 word for 1 id
        if (list.size() == 0)
            throw DatabaseError("Word doesn't exist.");
        else if (list.size() > 1)
            throw DatabaseError("Invalid database.");

        std::tie(word.Name, word.Definition, word.Related, word.Examples) = list.at(0);

        if (!IsIdInValidRange(word.Id) || !IsWordContentValid(word))
            throw DatabaseError("Cannot load word.");

        return word;
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot load word.");
    }
}


void Database::UpdateWord(const Word& word) {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    if (!IsIdInValidRange(word.Id)) {
        assert(false && "Invalid word id.");
        throw std::invalid_argument("Invalid word id.");
    }
    if (!IsWordContentValid(word)) {
        assert(false && "Invalid word.");
        throw std::invalid_argument("Invalid word.");
    }

    try {
        helper::sql<>::exec(
            m_impl->m_db,
            "UPDATE WORD SET NAME = ?, DEFINITION = ?, RELATED = ?, EXAMPLES = ? WHERE ID = ?;",
            word.Name,
            word.Definition,
            word.Related,
            word.Examples,
            word.Id);
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot update word.");
    }
}


void Database::RemoveWord(const int64_t wordId) {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    if (!IsIdInValidRange(wordId)) {
        assert(false && "Invalid word id.");
        throw std::invalid_argument("Invalid word id.");
    }

    try {
        helper::sql<>::exec(
            m_impl->m_db,
            "DELETE FROM WORD WHERE ID = ?;",
            wordId);
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot remove word.");
    }
}


std::vector<uint64_t> Database::GetDictionaryIdList() const {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    try {
        std::vector<uint64_t> dictionaryIdList;

        auto list = helper::sql<int64_t>::exec(
            m_impl->m_db,
            "SELECT ID FROM DICTIONARY;");

        for (auto& e : list) {
            auto id = std::get<0>(e);
            
            if (!IsIdInValidRange(id)) {
                assert(false && "Invalid database.");
                throw DatabaseError("Invalid database.");
            }

            dictionaryIdList.push_back(id);
        }

        return dictionaryIdList;
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot list dictionary id.");
    }
}


std::vector<uint64_t> Database::GetWordIdList(const int64_t dictionaryId) const {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    if (!IsIdInValidRange(dictionaryId)) {
        assert(false && "Invalid dictionary id.");
        throw std::invalid_argument("Invalid dictionary id.");
    }

    try {
        std::vector<uint64_t> wordIdList;

        auto list = helper::sql<int64_t>::exec(
            m_impl->m_db,
            "SELECT ID FROM WORD WHERE DICTIONARY_ID = ?;",
            dictionaryId);

        for (auto& e : list) {
            auto id = std::get<0>(e);

            if (!IsIdInValidRange(id)) {
                assert(false && "Invalid database.");
                throw DatabaseError("Invalid database.");
            }

            wordIdList.push_back(id);
        }

        return wordIdList;
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot list word id.");
    }
}


std::vector<Dictionary> Database::GetDictionaryList() const {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    try {
        std::vector<Dictionary> dictionaryList;

        auto list = helper::sql<int64_t, std::string, std::string>::exec(
            m_impl->m_db,
            "SELECT ID, NAME, DESCRIPTION FROM DICTIONARY;");

        for (auto& e : list) {
            Dictionary d;
            std::tie(d.Id, d.Name, d.Description) = e;

            if (!IsIdInValidRange(d.Id) || !IsDictionaryContentValid(d)) {
                assert(false && "Invalid database.");
                throw DatabaseError("Invalid database.");
            }

            dictionaryList.push_back(std::move(d));
        }

        return dictionaryList;
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot list dictionary.");
    }
}


std::vector<Word> Database::GetWordList(const int64_t dictionaryId) const {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    if (!IsIdInValidRange(dictionaryId)) {
        assert(false && "Invalid dictionary id.");
        throw std::invalid_argument("Invalid dictionary id.");
    }

    try {
        std::vector<Word> wordList;

        auto list = helper::sql<int64_t, std::string, std::string, std::string, std::string>::exec(
            m_impl->m_db,
            "SELECT ID, NAME, DEFINITION, RELATED, EXAMPLES FROM WORD WHERE DICTIONARY_ID = ? ORDER BY NAME ASC;",
            dictionaryId);

        for (auto& e : list) {
            Word w;
            std::tie(w.Id, w.Name, w.Definition, w.Related, w.Examples) = e;

            if (!IsIdInValidRange(w.Id) || !IsWordContentValid(w)) {
                assert(false && "Invalid database.");
                throw DatabaseError("Invalid database.");
            }

            wordList.push_back(std::move(w));
        }

        return wordList;
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot list word.");
    }
}


std::vector<Word> Database::Search(const int64_t dictionaryId, const std::string& text) const {
    assert((m_impl != nullptr) && (m_impl->m_db != nullptr));

    if (!IsIdInValidRange(dictionaryId)) {
        assert(false && "Invalid dictionary id.");
        throw std::invalid_argument("Invalid dictionary id.");
    }

    try {
        std::vector<Word> wordList;
        auto query =
            "SELECT ID, NAME, DEFINITION, RELATED, EXAMPLES"
            " FROM WORD"
            " WHERE"
            " DICTIONARY_ID = ?"
            " AND (NAME LIKE ? OR DEFINITION LIKE ? OR RELATED LIKE ? OR EXAMPLES LIKE ?)"
            " ORDER BY NAME ASC;";

        auto list = helper::sql<int64_t, std::string, std::string, std::string, std::string>::exec(
            m_impl->m_db,
            query,
            dictionaryId,
            "%" + text + "%",
            "%" + text + "%",
            "%" + text + "%",
            "%" + text + "%");

        for (auto& e : list) {
            Word w;
            std::tie(w.Id, w.Name, w.Definition, w.Related, w.Examples) = e;

            if (!IsIdInValidRange(w.Id) || !IsWordContentValid(w)) {
                assert(false && "Invalid database.");
                throw DatabaseError("Invalid database.");
            }

            wordList.push_back(std::move(w));
        }

        return wordList;
    }
    catch (const SQLiteError&) {
        throw DatabaseError("Cannot search.");
    }
}


void Database::RemoveAll() {
    auto dictionaryIdList = GetDictionaryIdList();
    for (auto& dictionaryId : dictionaryIdList) {
        assert(IsIdInValidRange(dictionaryId));

        RemoveDictionary(dictionaryId);
    }
}


int64_t Database::GetNumberOfDictionaries() const {
    return GetDictionaryIdList().size();
}


int64_t Database::GetNumberOfWords(const int64_t dictionaryId) const {
    if (!IsIdInValidRange(dictionaryId)) {
        assert(false && "Invalid dictionary id.");
        throw DatabaseError("Invalid dictionary id.");
    }

    return GetWordIdList(dictionaryId).size();
}


bool mydict::IsDatabaseValid(Database& db) {
    auto dictionaryList = db.GetDictionaryList();
    for (auto& dictionary : dictionaryList) {
        if (!IsIdInValidRange(dictionary.Id) || !IsDictionaryContentValid(dictionary))
            return false;

        auto wordList = db.GetWordList(dictionary.Id);
        for (auto& word : wordList) {
            if (!IsIdInValidRange(word.Id) || !IsWordContentValid(word))
                return false;
        }
    }
    return true;
}


void mydict::Import(Database& db, const Dictionary& dictionary, const std::vector<Word>& wordList) {
    // pre-check dictionary content
    if (!IsDictionaryContentValid(dictionary)) {
        assert(false && "Invalid dictionary.");
        throw DatabaseError("Invalid dictionary.");
    }

    // pre-check all words' content
    for (auto& word : wordList) {
        if (!IsWordContentValid(word)) {
            assert(false && "Invalid word.");
            throw DatabaseError("Invalid word.");
        }
    }

    // add new dictionary
    auto newDictionaryId = db.AddDictionary(dictionary);
    assert(IsIdInValidRange(newDictionaryId));

    // then add all the given words to it
    for (auto& word : wordList)
        db.AddWord(newDictionaryId, word);
}


void mydict::Export(const std::string& dbFile, Database& db, const std::vector<int64_t>& dictionaryIdList) {
    // pre-check all dictionaries' id
    for (auto& dictionaryId : dictionaryIdList) {
        if (!IsIdInValidRange(dictionaryId)) {
            assert(false && "Invalid dictionary id.");
            throw DatabaseError("Invalid dictionary id.");
        }
    }

    // create new database using the given file path
    auto newDb = std::make_unique<Database>(dbFile);
    newDb->RemoveAll();

    for (auto& dictionaryId : dictionaryIdList) {
        auto dictionary = db.LoadDictionary(dictionaryId);
        assert(IsDictionaryContentValid(dictionary));

        auto words = db.GetWordList(dictionaryId);

        auto newDictionaryId = newDb->AddDictionary(dictionary);
        assert(IsIdInValidRange(newDictionaryId));

        for (auto& word : words) {
            assert(IsWordContentValid(word));
            newDb->AddWord(newDictionaryId, word);
        }
    }
}
