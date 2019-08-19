package com.so61pi.test.model.relationship.one2many.list;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.ArrayList;
import java.util.List;

@Getter
class TestData {
    private List<BProduct> b = new ArrayList<>();
    private ACart a;

    TestData() {
        // Create data.
        b.add(new BProduct("product-0"));
        b.add(new BProduct("product-1"));
        b.add(new BProduct("product-2"));
        a = new ACart("cart-0");

        // Link data together.
        b.forEach(apartment -> a.getProducts().add(apartment));
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
        final BProduct product = new BProduct("product-10");
        em.persist(product);

        final ACart cart = new ACart("cart-10");
        em.persist(cart);

        cart.getProducts().add(product);
        log.info("testMakeRelationshipFromASide ends");
    }

    void testRemoveB() {
        log.info("testRemoveB begins");
        final BProduct product = testData.getB().get(0);
        em.remove(product);
        log.info("testRemoveB ends");
    }

    void testRemoveBFromASide() {
        log.info("testRemoveBFromASide begins");
        final BProduct product = testData.getB().get(0);

        // A removes B (break A-B relationship).
        testData.getA().getProducts().remove(product);

        // Now remove B.
        em.remove(product);
        log.info("testRemoveBFromASide ends");
    }

    void testRemoveA() {
        log.info("testRemoveA begins");
        final ACart cart = testData.getA();
        em.remove(cart);
        log.info("testRemoveA ends");
    }

    void testAddAWithMultipleSameBs() {
        log.info("testAddAWithMultipleSameBs begins");
        final BProduct product = new BProduct("product-30");
        em.persist(product);

        final ACart cart = new ACart("cart-30");
        cart.getProducts().add(product);
        cart.getProducts().add(product);
        em.persist(cart);
        log.info("testAddAWithMultipleSameBs ends");
    }
}
