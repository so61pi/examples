package com.so61pi.test.model.relationship.one2one;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

@Getter
class TestData {
    private BAccount b;
    private AEmail a;

    TestData() {
        // Create data.
        b = new BAccount("account-0");
        a = new AEmail("email-0");

        // Link data together.
        a.setAccount(b);
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
        em.persist(testData.getA());
        em.persist(testData.getB());
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
        final BAccount account = new BAccount("account-10");
        em.persist(account);

        final AEmail email = new AEmail("email-10");
        em.persist(email);

        email.setAccount(account);
        log.info("testMakeRelationshipFromASide ends");
    }

    void testRemoveB() {
        log.info("testRemoveB begins");
        final BAccount account = testData.getB();
        em.remove(account);
        log.info("testRemoveB ends");
    }

    void testRemoveBFromASide() {
        log.info("testRemoveBFromASide begins");
        final AEmail email = testData.getA();
        final BAccount account = email.getAccount();

        // A removes B (break A-B relationship).
        email.setAccount(null);

        // Now remove B.
        em.remove(account);
        log.info("testRemoveBFromASide ends");
    }

    void testRemoveA() {
        log.info("testRemoveA begins");
        final AEmail email = testData.getA();
        em.remove(email);
        log.info("testRemoveA ends");
    }
}
