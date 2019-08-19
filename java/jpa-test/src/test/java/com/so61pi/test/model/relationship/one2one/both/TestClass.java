package com.so61pi.test.model.relationship.one2one.both;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

@Getter
class TestData {
    private BAddress b;
    private AWorker a;

    TestData() {
        // Create data.
        b = new BAddress("address-0");
        a = new AWorker("worker-0");

        // Link data together.
        a.setAddress(b);
        b.setWorker(a);
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
        final BAddress address = new BAddress("address-10");
        em.persist(address);

        final AWorker worker = new AWorker("worker-10");
        em.persist(worker);

        worker.setAddress(address);
        log.info("testMakeRelationshipFromASide ends");
    }

    void testMakeRelationshipFromBSide() {
        log.info("testMakeRelationshipFromBSide begins");
        final BAddress address = new BAddress("address-10");
        em.persist(address);

        final AWorker worker = new AWorker("worker-10");
        em.persist(worker);

        address.setWorker(worker);
        log.info("testMakeRelationshipFromBSide ends");
    }

    void testRemoveB() {
        log.info("testRemoveB begins");
        final BAddress address = testData.getB();
        em.remove(address);
        log.info("testRemoveB ends");
    }

    void testRemoveBFromASide() {
        log.info("testRemoveBFromASide begins");
        final AWorker worker = testData.getA();
        final BAddress address = worker.getAddress();

        // A removes B (break A-B relationship).
        worker.setAddress(null);

        // Now remove B.
        em.remove(address);
        log.info("testRemoveBFromASide ends");
    }

    void testRemoveA() {
        log.info("testRemoveA begins");
        final AWorker worker = testData.getA();
        em.remove(worker);
        log.info("testRemoveA ends");
    }

    void testRemoveAFromBSide() {
        log.info("testRemoveAFromBSide begins");
        final BAddress address = testData.getB();
        final AWorker worker = address.getWorker();

        // B removes A (break A-B relationship).
        address.setWorker(null);

        // Now remove A.
        em.remove(worker);
        log.info("testRemoveAFromBSide ends");
    }
}
