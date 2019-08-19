package com.so61pi.test.model.relationship.many2one;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.ArrayList;
import java.util.List;

@Getter
class TestData {
    private BMother b;
    private List<AChild> a = new ArrayList<>();

    TestData() {
        // Create data.
        b = new BMother("mother-0");
        a.add(new AChild("child-0"));
        a.add(new AChild("child-1"));
        a.add(new AChild("child-2"));

        // Link data together.
        a.forEach(child -> child.setMother(b));
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
        em.persist(testData.getB());
        testData.getA().forEach(em::persist);
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
        final BMother mother = new BMother("mother-10");
        em.persist(mother);

        final AChild child = new AChild("child-10");
        em.persist(child);

        child.setMother(mother);
        log.info("testMakeRelationshipFromASide ends");
    }

    void testRemoveB() {
        log.info("testRemoveB begins");
        final BMother mother = testData.getB();
        em.remove(mother);
        log.info("testRemoveB ends");
    }

    void testRemoveBFromASide() {
        log.info("testRemoveBFromASide begins");
        final BMother mother = testData.getB();

        // A removes B (break A-B relationship).
        testData.getA().forEach(child -> child.setMother(null));

        // Now remove B.
        em.remove(mother);
        log.info("testRemoveBFromASide ends");
    }

    void testRemoveA() {
        log.info("testRemoveA begins");
        final AChild child = testData.getA().get(0);
        em.remove(child);
        log.info("testRemoveA ends");
    }

}
