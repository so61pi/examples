#ifndef DATABASE_H
#define DATABASE_H


#include <cstdint>
#include <exception>
#include <memory>
#include <stdexcept>
#include <vector>


namespace mydict {

    class DatabaseError : public std::domain_error
    {
        // using std::domain_error::domain_error;

    public:
        explicit DatabaseError(const std::string& what) :
            std::domain_error(what)
        {}
    };

    
    const int64_t InvalidId = 0;


    struct Dictionary {
        int64_t Id = InvalidId;
        std::string Name;
        std::string Description;
    };


    struct Word {
        int64_t Id = InvalidId;
        std::string Name;
        std::string Definition;
        std::string Related;
        std::string Examples;
        int64_t DictionaryId = InvalidId;
    };


    bool IsIdInValidRange(int64_t id);


    bool IsDictionaryContentValid(const Dictionary& dictionary);
    bool IsWordContentValid(const Word& word);


    class Database {
    public:
        // Construct new database
        //
        // dbFile must be in UTF-8 format
        explicit Database(const std::string& dbFile);
        ~Database();

        // Add new dictionary to database.
        int64_t AddDictionary(const Dictionary& dictionary);

        // Load a dictionary using dictionary id
        Dictionary LoadDictionary(const int64_t dictionaryId) const;

        // Update an existed dictionary
        void UpdateDictionary(const Dictionary& dictionary);

        // Remove an existed dictionary
        void RemoveDictionary(const int64_t dictionaryId);


        //
        int64_t AddWord(const int64_t dictionaryId, const Word& word);

        //
        Word LoadWord(int64_t wordId) const;

        //
        void UpdateWord(const Word& word);

        //
        void RemoveWord(const int64_t wordId);


        //
        std::vector<uint64_t> GetDictionaryIdList() const;

        //
        std::vector<uint64_t> GetWordIdList(const int64_t dictionaryId) const;


        //
        std::vector<Dictionary> GetDictionaryList() const;

        //
        std::vector<Word> GetWordList(const int64_t dictionaryId) const;


        //
        std::vector<Word> Search(const int64_t dictionaryId, const std::string& text) const;


        //
        void RemoveAll();


        //
        int64_t GetNumberOfDictionaries() const;

        //
        int64_t GetNumberOfWords(const int64_t dictionaryId) const;


    private: // data
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };


    bool IsDatabaseValid(Database& db);

    void Import(Database& db, const Dictionary& dictionary, const std::vector<Word>& wordList);
    void Export(const std::string& dbFile, Database& db, const std::vector<int64_t>& dictionaryIdList);
}


#endif // DATABASE_H
