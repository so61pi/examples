package com.so61pi.test.model.relationship.many2many.listlist;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.ArrayList;
import java.util.List;

@Getter
class TestData {
    private List<BTag> b = new ArrayList<>();
    private List<AEmployee> a = new ArrayList<>();

    TestData() {
        // Create data.
        b.add(new BTag("tag-0"));
        b.add(new BTag("tag-1"));
        b.add(new BTag("tag-2"));
        a.add(new AEmployee("employee-0"));
        a.add(new AEmployee("employee-1"));
        a.add(new AEmployee("employee-2"));

        // Link data together.
        a.forEach(employee -> b.forEach(tag -> employee.getTags().add(tag)));
        b.forEach(tag -> a.forEach(employee -> tag.getEmployees().add(employee)));
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
        final AEmployee employee = new AEmployee("employee-10");
        em.persist(employee);

        final BTag tag = new BTag("tag-10");
        em.persist(tag);

        employee.getTags().add(tag);
        log.info("testMakeRelationshipFromASide ends");
    }

    void testMakeRelationshipFromBSide() {
        log.info("testMakeRelationshipFromBSide begins");
        final AEmployee employee = new AEmployee("employee-20");
        em.persist(employee);

        final BTag tag = new BTag("tag-20");
        em.persist(tag);

        tag.getEmployees().add(employee);
        log.info("testMakeRelationshipFromBSide ends");
    }

    void testRemoveB() {
        log.info("testRemoveB begins");
        final BTag tag = testData.getB().get(0);
        em.remove(tag);
        log.info("testRemoveB ends");
    }

    void testRemoveBFromASide() {
        log.info("testRemoveBFromASide begins");
        final BTag tag = testData.getB().get(0);

        // A removes B (break A-B relationship).
        testData.getA().forEach(employee -> employee.getTags().remove(tag));

        // Now remove B.
        em.remove(tag);
        log.info("testRemoveBFromASide ends");
    }

    void testRemoveA() {
        log.info("testRemoveA begins");
        final AEmployee employee = testData.getA().get(0);
        em.remove(employee);
        log.info("testRemoveA ends");
    }

    void testRemoveAFromBSide() {
        log.info("testRemoveAFromBSide begins");
        final AEmployee employee = testData.getA().get(0);

        // B removes A (break A-B relationship).
        testData.getB().forEach(tag -> tag.getEmployees().remove(employee));

        // Now remove A.
        em.remove(employee);
        log.info("testRemoveAFromBSide ends");
    }

    void testAddAWithMultipleSameBs() {
        log.info("testAddAWithMultipleSameBs begins");
        final BTag tag = new BTag("tag-30");
        em.persist(tag);

        final AEmployee employee = new AEmployee("employee-30");
        employee.getTags().add(tag);
        employee.getTags().add(tag);
        em.persist(employee);
        log.info("testAddAWithMultipleSameBs ends");
    }

    void testAddBWithMultipleSameAs() {
        log.info("testAddBWithMultipleSameAs begins");
        final AEmployee employee = new AEmployee("employee-40");
        em.persist(employee);

        final BTag tag = new BTag("tag-40");
        tag.getEmployees().add(employee);
        tag.getEmployees().add(employee);
        em.persist(tag);
        log.info("testAddBWithMultipleSameAs ends");
    }
}
