package com.so61pi.test.model.relationship.many2many.list;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.ArrayList;
import java.util.List;

@Getter
class TestData {
    private List<BBook> books = new ArrayList<>();
    private List<AAuthor> authors = new ArrayList<>();

    TestData() {
        // Create data.
        books.add(new BBook("book-0"));
        books.add(new BBook("book-1"));
        books.add(new BBook("book-2"));
        authors.add(new AAuthor("author-0"));
        authors.add(new AAuthor("author-1"));
        authors.add(new AAuthor("author-2"));

        // Link data together.
        authors.forEach(author -> books.forEach(book -> author.getBooks().add(book)));
    }
}

@Slf4j
class TestClass {
    private String persistenceUnit;
    private EntityManagerFactory emf;
    private EntityManager em;
    private TestData testData;

    TestClass(String persistenceUnit) {
        this.persistenceUnit = persistenceUnit;
    }

    void setUp() {
        log.info("setUp begins");
        emf = Persistence.createEntityManagerFactory(persistenceUnit);
        em = emf.createEntityManager();

        testData = new TestData();

        // Put data into database.
        em.getTransaction().begin();
        testData.getAuthors().forEach(em::persist);
        testData.getBooks().forEach(em::persist);
        em.getTransaction().commit();

        em.getTransaction().begin();
        log.info("setUp ends");
    }

    void tearDown() {
        log.info("tearDown begins");
        em.getTransaction().commit();
        em.close();
        emf.close();
        log.info("tearDown ends");
    }

    void testMakeRelationshipFromAuthorSide() {
        log.info("testMakeRelationshipFromAuthorSide begins");
        final BBook book = new BBook("book-10");
        em.persist(book);

        final AAuthor author = new AAuthor("author-10");
        em.persist(author);

        author.getBooks().add(book);
        log.info("testMakeRelationshipFromAuthorSide ends");
    }

    void testRemoveBookFromBookSide() {
        log.info("testRemoveBookFromBookSide begins");
        final BBook book = testData.getBooks().get(0);
        em.remove(book);
        log.info("testRemoveBookFromBookSide ends");
    }

    void testRemoveBookFromAuthorSide() {
        log.info("testRemoveBookFromAuthorSide begins");
        final BBook book = testData.getBooks().get(0);

        // Break author-book relationships for all authors.
        testData.getAuthors().forEach(author -> author.getBooks().remove(book));

        // Now remove book.
        em.remove(book);
        log.info("testRemoveBookFromAuthorSide ends");
    }

    void testRemoveAuthor() {
        log.info("testRemoveAuthor begins");
        final AAuthor author = testData.getAuthors().get(0);
        em.remove(author);
        log.info("testRemoveAuthor ends");
    }

    void testAddAuthorWithMultipleSameBooks() {
        log.info("testAddAuthorWithMultipleSameBooks begins");
        final BBook book = new BBook("book-20");
        em.persist(book);

        final AAuthor author = new AAuthor("author-20");
        author.getBooks().add(book);
        author.getBooks().add(book);
        em.persist(author);
        log.info("testAddAuthorWithMultipleSameBooks ends");
    }
}
