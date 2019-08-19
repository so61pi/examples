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
    private List<BBook> b = new ArrayList<>();
    private List<AAuthor> a = new ArrayList<>();

    TestData() {
        // Create data.
        b.add(new BBook("book-0"));
        b.add(new BBook("book-1"));
        b.add(new BBook("book-2"));
        a.add(new AAuthor("author-0"));
        a.add(new AAuthor("author-1"));
        a.add(new AAuthor("author-2"));

        // Link data together.
        a.forEach(author -> b.forEach(book -> author.getBooks().add(book)));
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
        testData.getA().forEach(em::persist);
        testData.getB().forEach(em::persist);
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

    void testMakeRelationshipFromASide() {
        log.info("testMakeRelationshipFromASide begins");
        final BBook book = new BBook("book-10");
        em.persist(book);

        final AAuthor author = new AAuthor("author-10");
        em.persist(author);

        author.getBooks().add(book);
        log.info("testMakeRelationshipFromASide ends");
    }

    void testRemoveB() {
        log.info("testRemoveB begins");
        final BBook book = testData.getB().get(0);
        em.remove(book);
        log.info("testRemoveB ends");
    }

    void testRemoveBFromASide() {
        log.info("testRemoveBFromASide begins");
        final BBook book = testData.getB().get(0);

        // A removes B (break A-B relationship).
        testData.getA().forEach(author -> author.getBooks().remove(book));

        // Now remove B.
        em.remove(book);
        log.info("testRemoveBFromASide ends");
    }

    void testRemoveA() {
        log.info("testRemoveA begins");
        final AAuthor author = testData.getA().get(0);
        em.remove(author);
        log.info("testRemoveA ends");
    }

    void testAddAWithMultipleSameBs() {
        log.info("testAddAWithMultipleSameBs begins");
        final BBook book = new BBook("book-20");
        em.persist(book);

        final AAuthor author = new AAuthor("author-20");
        author.getBooks().add(book);
        author.getBooks().add(book);
        em.persist(author);
        log.info("testAddAWithMultipleSameBs ends");
    }
}
