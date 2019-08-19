package com.so61pi.test.model.relationship.many2one.set;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.ArrayList;
import java.util.List;

@Getter
class TestData {
    private BFamily b;
    private List<AMember> a = new ArrayList<>();

    TestData() {
        // Create data.
        b = new BFamily("family-0");
        a.add(new AMember("member-0"));
        a.add(new AMember("member-1"));
        a.add(new AMember("member-2"));

        // Link data together.
        a.forEach(member -> member.setFamily(b));
        a.forEach(member -> b.getMembers().add(member));
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
        final BFamily family = new BFamily("family-10");
        em.persist(family);

        final AMember member = new AMember("member-10");
        em.persist(member);

        member.setFamily(family);
        log.info("testMakeRelationshipFromASide ends");
    }

    void testMakeRelationshipFromBSide() {
        log.info("testMakeRelationshipFromBSide begins");
        final BFamily family = new BFamily("family-20");
        em.persist(family);

        final AMember member = new AMember("member-20");
        em.persist(member);

        family.getMembers().add(member);
        log.info("testMakeRelationshipFromBSide ends");
    }

    void testRemoveB() {
        log.info("testRemoveB begins");
        final BFamily family = testData.getB();
        em.remove(family);
        log.info("testRemoveB ends");
    }

    void testRemoveBFromASide() {
        log.info("testRemoveBFromASide begins");
        final BFamily family = testData.getB();

        // A removes B (break A-B relationship).
        testData.getA().forEach(member -> member.setFamily(null));

        // Now remove B.
        em.remove(family);
        log.info("testRemoveBFromASide ends");
    }

    void testRemoveA() {
        log.info("testRemoveA begins");
        final AMember member = testData.getA().get(0);
        em.remove(member);
        log.info("testRemoveA ends");
    }

    void testRemoveAFromBSide() {
        log.info("testRemoveAFromBSide begins");
        final AMember member = testData.getA().get(0);

        // B removes A (break A-B relationship).
        testData.getB().getMembers().remove(member);

        // Now remove A.
        em.remove(member);
        log.info("testRemoveAFromBSide ends");
    }

    void testAddBWithMultipleSameAs() {
        log.info("testAddBWithMultipleSameAs begins");
        final AMember member = new AMember("member-40");
        em.persist(member);

        final BFamily family = new BFamily("family-40");
        family.getMembers().add(member);
        family.getMembers().add(member);
        em.persist(family);
        log.info("testAddBWithMultipleSameAs ends");
    }
}
