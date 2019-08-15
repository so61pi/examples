package com.so61pi.test.model.relationship.many2many.list;

import org.junit.jupiter.api.*;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.FlushModeType;
import javax.persistence.Persistence;

class TestClass {
    private EntityManagerFactory emf;
    private EntityManager em;

    @BeforeEach
    void setUp() {
        emf = Persistence.createEntityManagerFactory("test-persistence-unit");
        em = emf.createEntityManager();
        em.getTransaction().begin();
    }

    @AfterEach
    void tearDown() {
        em.getTransaction().commit();
        em.close();
        emf.close();
    }

    /**
     * Entity author is the owner of the author-book relationship.
     * This means Author manages the join table (authors_books).
     */

    /**
     * Hibernate
     *
     * create table authors (id bigint not null, primary key (id))
     * create table authors_books (Author_id bigint not null, books_id bigint not null)
     * create table books (id bigint not null, primary key (id))
     * alter table authors_books add constraint FKllirsa7edekqe77avxsqm4o6 foreign key (books_id) references books
     * alter table authors_books add constraint FKhgbgyab6aj8oh4r0c1rcdkg4l foreign key (Author_id) references authors
     */

    /**
     * EclipseLink
     *
     * CREATE TABLE books (ID BIGINT NOT NULL, PRIMARY KEY (ID))
     * CREATE TABLE authors (ID BIGINT NOT NULL, PRIMARY KEY (ID))
     * CREATE TABLE authors_books (Author_ID BIGINT NOT NULL, books_ID BIGINT NOT NULL, PRIMARY KEY (Author_ID, books_ID))
     * ALTER TABLE authors_books ADD CONSTRAINT FK_authors_books_Author_ID FOREIGN KEY (Author_ID) REFERENCES authors (ID)
     * ALTER TABLE authors_books ADD CONSTRAINT FK_authors_books_books_ID FOREIGN KEY (books_ID) REFERENCES books (ID)
     */
    @Test
    void testAddBook() {
        Book book = new Book();

        /* Hibernate
         * call next value for hibernate_sequence
         * insert into books (id) values (?)
         *
         * EclipseLink
         * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
         * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
         * INSERT INTO books (ID) VALUES (?)
         */
        em.persist(book);
    }

    @Test
    void testRemoveBook() {
        Book book = new Book();

        /* Hibernate
         * call next value for hibernate_sequence
         * insert into books (id) values (?)
         *
         * EclipseLink
         * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
         * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
         * INSERT INTO books (ID) VALUES (?)
         */
        em.persist(book);
        em.flush();

        /* Hibernate
         * delete from books where id=?
         *
         * EclipseLink
         * DELETE FROM books WHERE (ID = ?)
         */
        em.remove(book);
    }

    @Test
    void testAddAuthor() {
        Author author = new Author();

        /* Hibernate
         * call next value for hibernate_sequence
         * insert into authors (id) values (?)
         *
         * EclipseLink
         * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
         * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
         * INSERT INTO authors (ID) VALUES (?)
         */
        em.persist(author);
    }

    @Test
    void testRemoveAuthor() {
        Author author = new Author();

        /* Hibernate
         * call next value for hibernate_sequence
         * insert into authors (id) values (?)
         *
         * EclipseLink
         * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
         * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
         * INSERT INTO authors (ID) VALUES (?)
         */
        em.persist(author);
        em.flush();

        /* Hibernate
         * delete from authors where id=?
         *
         * EclipseLink
         * DELETE FROM authors_books WHERE (Author_ID = ?)
         * DELETE FROM authors WHERE (ID = ?)
         */
        em.remove(author);
    }

    @Test
    void testAddAuthorWithBook() {
        Book book = new Book();
        Author author = new Author();
        author.getBooks().add(book);

        /*
         * We can persist author before book.
         */

        /* Hibernate
         * call next value for hibernate_sequence
         * call next value for hibernate_sequence
         * insert into authors (id) values (?)
         * insert into books (id) values (?)
         * insert into authors_books (Author_id, books_id) values (?, ?)
         *
         * EclipseLink
         * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
         * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
         * INSERT INTO authors (ID) VALUES (?)
         * INSERT INTO books (ID) VALUES (?)
         * INSERT INTO authors_books (books_ID, Author_ID) VALUES (?, ?)
         */
        em.persist(author);
        em.persist(book);
    }

    @Test
    void testAddAuthorWithDuplicatedBooks() {
        Book book = new Book();
        Author author = new Author();
        author.getBooks().add(book);
        author.getBooks().add(book);

        /* Hibernate
         * call next value for hibernate_sequence
         * call next value for hibernate_sequence
         * insert into books (id) values (?)
         * insert into authors (id) values (?)
         * insert into authors_books (Author_id, books_id) values (?, ?)
         * insert into authors_books (Author_id, books_id) values (?, ?)
         *
         * EclipseLink
         * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
         * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
         * INSERT INTO authors (ID) VALUES (?)
         * INSERT INTO books (ID) VALUES (?)
         * INSERT INTO authors_books (books_ID, Author_ID) VALUES (?, ?)
         * INSERT INTO authors_books (books_ID, Author_ID) VALUES (?, ?)
         * Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Unique index or primary key violation: "PUBLIC.PRIMARY_KEY_80 ON PUBLIC.AUTHORS_BOOKS(AUTHOR_ID, BOOKS_ID) VALUES 1";
         */
        em.persist(book);
        em.persist(author);
    }

    @Test
    void testAuthorRemoveBookIncorrectly() {
        Book book = new Book();
        Author author = new Author();
        author.getBooks().add(book);

        /* Hibernate
         * call next value for hibernate_sequence
         * call next value for hibernate_sequence
         * insert into books (id) values (?)
         * insert into authors (id) values (?)
         * insert into authors_books (Author_id, books_id) values (?, ?)
         *
         * EclipseLink
         * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
         * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
         * INSERT INTO authors (ID) VALUES (?)
         * INSERT INTO books (ID) VALUES (?)
         * INSERT INTO authors_books (books_ID, Author_ID) VALUES (?, ?)
         */
        em.persist(book);
        em.persist(author);
        em.flush();

        /*
         * Because author is the owner of the author-book relationship, removing book alone will lead to an SQL violation.
         */

        /* Hibernate
         * ERROR: Referential integrity constraint violation: "FKLLIRSA7EDEKQE77AVXSQM4O6: PUBLIC.AUTHORS_BOOKS FOREIGN KEY(BOOKS_ID) REFERENCES PUBLIC.BOOKS(ID) (1)"; SQL statement:
         * delete from books where id=?
         *
         * EclipseLink
         * DELETE FROM books WHERE (ID = ?)
         * Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Referential integrity constraint violation: "FK_AUTHORS_BOOKS_BOOKS_ID: PUBLIC.AUTHORS_BOOKS FOREIGN KEY(BOOKS_ID) REFERENCES PUBLIC.BOOKS(ID) (1)";
         */
        em.remove(book);
    }

    @Test
    void testAuthorRemoveBookCorrectly() {
        Book book = new Book();
        Author author = new Author();
        author.getBooks().add(book);

        /* Hibernate
         * call next value for hibernate_sequence
         * call next value for hibernate_sequence
         * insert into books (id) values (?)
         * insert into authors (id) values (?)
         * insert into authors_books (Author_id, books_id) values (?, ?)
         *
         * EclipseLink
         * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
         * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
         * INSERT INTO authors (ID) VALUES (?)
         * INSERT INTO books (ID) VALUES (?)
         * INSERT INTO authors_books (books_ID, Author_ID) VALUES (?, ?)
         */
        em.persist(book);
        em.persist(author);
        em.flush();

        /*
         * Let the owner (author) break the relationship before removing book.
         */

        /* Hibernate
         * delete from authors_books where Author_id=?
         * delete from books where id=?
         *
         * EclipseLink
         * DELETE FROM authors_books WHERE ((books_ID = ?) AND (Author_ID = ?))
         * DELETE FROM books WHERE (ID = ?)
         */
        author.getBooks().remove(book);
        em.remove(book);
    }

    @Test
    void testAuthorDetachBook() {
        Book book = new Book();
        Author author = new Author();
        author.getBooks().add(book);

        /* Hibernate
         * call next value for hibernate_sequence
         * call next value for hibernate_sequence
         * insert into books (id) values (?)
         * insert into authors (id) values (?)
         * insert into authors_books (Author_id, books_id) values (?, ?)
         *
         * EclipseLink
         * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
         * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
         * INSERT INTO authors (ID) VALUES (?)
         * INSERT INTO books (ID) VALUES (?)
         * INSERT INTO authors_books (books_ID, Author_ID) VALUES (?, ?)
         */
        em.persist(book);
        em.persist(author);
        em.flush();

        /* Hibernate
         * delete from authors_books where Author_id=?
         *
         * EclipseLink
         * DELETE FROM authors_books WHERE ((books_ID = ?) AND (Author_ID = ?))
         */
        author.getBooks().remove(book);
    }

    @Test
    void testRemoveAuthorWithBooks() {
        Book book1 = new Book();
        Book book2 = new Book();
        Author author = new Author();
        author.getBooks().add(book1);
        author.getBooks().add(book2);

        /* Hibernate
         * call next value for hibernate_sequence
         * call next value for hibernate_sequence
         * call next value for hibernate_sequence
         * insert into books (id) values (?)
         * insert into books (id) values (?)
         * insert into authors (id) values (?)
         * insert into authors_books (Author_id, books_id) values (?, ?)
         * insert into authors_books (Author_id, books_id) values (?, ?)
         *
         * EclipseLink
         * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
         * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
         * INSERT INTO authors (ID) VALUES (?)
         * INSERT INTO books (ID) VALUES (?)
         * INSERT INTO books (ID) VALUES (?)
         * INSERT INTO authors_books (books_ID, Author_ID) VALUES (?, ?)
         * INSERT INTO authors_books (books_ID, Author_ID) VALUES (?, ?)
         */
        em.persist(book1);
        em.persist(book2);
        em.persist(author);
        em.flush();

        /* Hibernate
         * delete from authors_books where Author_id=?
         * delete from authors where id=?
         *
         * EclipseLink
         * DELETE FROM authors_books WHERE (Author_ID = ?)
         * DELETE FROM authors WHERE (ID = ?)
         */
        em.remove(author);
    }

    @Test
    void testAuthorReduceBooks() {
        Book book1 = new Book();
        Book book2 = new Book();
        Book book3 = new Book();
        Author author = new Author();
        author.getBooks().add(book1);
        author.getBooks().add(book2);
        author.getBooks().add(book3);

        /* Hibernate
         * call next value for hibernate_sequence
         * call next value for hibernate_sequence
         * call next value for hibernate_sequence
         * call next value for hibernate_sequence
         * insert into books (id) values (?)
         * insert into books (id) values (?)
         * insert into books (id) values (?)
         * insert into authors (id) values (?)
         * insert into authors_books (Author_id, books_id) values (?, ?)
         * insert into authors_books (Author_id, books_id) values (?, ?)
         * insert into authors_books (Author_id, books_id) values (?, ?)
         *
         * EclipseLink
         * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
         * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
         * INSERT INTO authors (ID) VALUES (?)
         * INSERT INTO books (ID) VALUES (?)
         * INSERT INTO books (ID) VALUES (?)
         * INSERT INTO books (ID) VALUES (?)
         * INSERT INTO authors_books (books_ID, Author_ID) VALUES (?, ?)
         * INSERT INTO authors_books (books_ID, Author_ID) VALUES (?, ?)
         * INSERT INTO authors_books (books_ID, Author_ID) VALUES (?, ?)
         */
        em.persist(book1);
        em.persist(book2);
        em.persist(book3);
        em.persist(author);
        em.flush();

        /* Hibernate
         * delete from authors_books where Author_id=?
         * insert into authors_books (Author_id, books_id) values (?, ?)
         * insert into authors_books (Author_id, books_id) values (?, ?)
         * delete from books where id=?
         *
         * EclipseLink
         * DELETE FROM authors_books WHERE ((books_ID = ?) AND (Author_ID = ?))
         * DELETE FROM books WHERE (ID = ?)
         */
        author.getBooks().remove(book3);
        em.remove(book3);
        em.flush();
    }
}
