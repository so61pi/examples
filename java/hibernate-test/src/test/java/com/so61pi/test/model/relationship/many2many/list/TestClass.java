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
     *
     * create table authors (id bigint not null, primary key (id))
     * create table authors_books (Author_id bigint not null, books_id bigint not null)
     * create table books (id bigint not null, primary key (id))
     * alter table authors_books add constraint FKllirsa7edekqe77avxsqm4o6 foreign key (books_id) references books
     * alter table authors_books add constraint FKhgbgyab6aj8oh4r0c1rcdkg4l foreign key (Author_id) references authors
     */
    @Test
    void testAddBook() {
        Book book = new Book();

        // insert into books (id) values (?)
        em.persist(book);
    }

    @Test
    void testRemoveBook() {
        Book book = new Book();

        // insert into books (id) values (?)
        em.persist(book);
        em.flush();

        // delete from books where id=?
        em.remove(book);
    }

    @Test
    void testAddAuthor() {
        Author author = new Author();

        // insert into authors (id) values (?)
        em.persist(author);
    }

    @Test
    void testRemoveAuthor() {
        Author author = new Author();

        // insert into authors (id) values (?)
        em.persist(author);
        em.flush();

        // delete from authors where id=?
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

        // insert into authors (id) values (?)
        // insert into books (id) values (?)
        // insert into authors_books (Author_id, books_id) values (?, ?)
        em.persist(author);
        em.persist(book);
    }

    @Test
    void testAuthorRemoveBookIncorrectly() {
        Book book = new Book();
        Author author = new Author();
        author.getBooks().add(book);

        // insert into books (id) values (?)
        // insert into authors (id) values (?)
        // insert into authors_books (Author_id, books_id) values (?, ?)
        em.persist(book);
        em.persist(author);
        em.flush();

        /*
         * Because author is the owner of the author-book relationship, removing book alone will lead to an SQL violation.
         */
        // ERROR: Referential integrity constraint violation: "FKLLIRSA7EDEKQE77AVXSQM4O6: PUBLIC.AUTHORS_BOOKS FOREIGN KEY(BOOKS_ID) REFERENCES PUBLIC.BOOKS(ID) (1)"; SQL statement:
        // delete from books where id=?
        em.remove(book);
    }

    @Test
    void testAuthorRemoveBookCorrectly() {
        Book book = new Book();
        Author author = new Author();
        author.getBooks().add(book);

        // insert into books (id) values (?)
        // insert into authors (id) values (?)
        // insert into authors_books (Author_id, books_id) values (?, ?)
        em.persist(book);
        em.persist(author);
        em.flush();

        /*
         * Let the owner (author) break the relationship before removing book.
         */
        // delete from authors_books where Author_id=?
        author.getBooks().remove(book);

        // delete from books where id=?
        em.remove(book);
    }

    @Test
    void testAuthorDetachBook() {
        Book book = new Book();
        Author author = new Author();
        author.getBooks().add(book);

        // insert into books (id) values (?)
        // insert into authors (id) values (?)
        // insert into authors_books (Author_id, books_id) values (?, ?)
        em.persist(book);
        em.persist(author);
        em.flush();

        // delete from authors_books where Author_id=?
        author.getBooks().remove(book);
    }

    @Test
    void testRemoveAuthorWithBooks() {
        Book book1 = new Book();
        Book book2 = new Book();
        Author author = new Author();
        author.getBooks().add(book1);
        author.getBooks().add(book2);

        // insert into books (id) values (?)
        // insert into books (id) values (?)
        // insert into authors (id) values (?)
        // insert into authors_books (Author_id, books_id) values (?, ?)
        // insert into authors_books (Author_id, books_id) values (?, ?)
        em.persist(book1);
        em.persist(book2);
        em.persist(author);
        em.flush();

        // delete from authors_books where Author_id=?
        // delete from authors where id=?
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

        // insert into books (id) values (?)
        // insert into books (id) values (?)
        // insert into books (id) values (?)
        // insert into authors (id) values (?)
        // insert into authors_books (Author_id, books_id) values (?, ?)
        // insert into authors_books (Author_id, books_id) values (?, ?)
        // insert into authors_books (Author_id, books_id) values (?, ?)
        em.persist(book1);
        em.persist(book2);
        em.persist(book3);
        em.persist(author);
        em.flush();

        // delete from authors_books where Author_id=?
        // insert into authors_books (Author_id, books_id) values (?, ?)
        // insert into authors_books (Author_id, books_id) values (?, ?)
        // delete from books where id=?
        author.getBooks().remove(book3);
        em.remove(book3);
        em.flush();
    }
}
