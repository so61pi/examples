package com.so61pi.test.model.relationship.one2many.set;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.ArrayList;
import java.util.List;

@Getter
class TestData {
    private List<BApartment> b = new ArrayList<>();
    private ACity a;

    TestData() {
        // Create data.
        b.add(new BApartment("apartment-0"));
        b.add(new BApartment("apartment-1"));
        b.add(new BApartment("apartment-2"));
        a = new ACity("city-0");

        // Link data together.
        b.forEach(apartment -> a.getApartments().add(apartment));
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
        final BApartment apartment = new BApartment("apartment-10");
        em.persist(apartment);

        final ACity city = new ACity("city-10");
        em.persist(city);

        city.getApartments().add(apartment);
        log.info("testMakeRelationshipFromASide ends");
    }

    void testRemoveB() {
        log.info("testRemoveB begins");
        final BApartment apartment = testData.getB().get(0);
        em.remove(apartment);
        log.info("testRemoveB ends");
    }

    void testRemoveBFromASide() {
        log.info("testRemoveBFromASide begins");
        final BApartment apartment = testData.getB().get(0);

        // A removes B (break A-B relationship).
        testData.getA().getApartments().remove(apartment);

        // Now remove B.
        em.remove(apartment);
        log.info("testRemoveBFromASide ends");
    }

    void testRemoveA() {
        log.info("testRemoveA begins");
        final ACity city = testData.getA();
        em.remove(city);
        log.info("testRemoveA ends");
    }

    void testAddAWithMultipleSameBs() {
        log.info("testAddAWithMultipleSameBs begins");
        final BApartment apartment = new BApartment("apartment-30");
        em.persist(apartment);

        final ACity city = new ACity("city-30");
        city.getApartments().add(apartment);
        city.getApartments().add(apartment);
        em.persist(city);
        log.info("testAddAWithMultipleSameBs ends");
    }
}
