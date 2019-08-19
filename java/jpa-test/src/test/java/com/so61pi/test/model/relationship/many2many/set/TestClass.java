package com.so61pi.test.model.relationship.many2many.set;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.ArrayList;
import java.util.List;

@Getter
class TestData {
    private List<BLabel> b = new ArrayList<>();
    private List<AAlbum> a = new ArrayList<>();

    TestData() {
        // Create data.
        b.add(new BLabel("label-0"));
        b.add(new BLabel("label-1"));
        b.add(new BLabel("label-2"));
        a.add(new AAlbum("album-0"));
        a.add(new AAlbum("album-1"));
        a.add(new AAlbum("album-2"));

        // Link data together.
        a.forEach(album -> b.forEach(label -> album.getLabels().add(label)));
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
        final BLabel label = new BLabel("label-10");
        em.persist(label);

        final AAlbum album = new AAlbum("album-10");
        em.persist(album);

        album.getLabels().add(label);
        log.info("testMakeRelationshipFromASide ends");
    }

    void testRemoveB() {
        log.info("testRemoveB begins");
        final BLabel label = testData.getB().get(0);
        em.remove(label);
        log.info("testRemoveB ends");
    }

    void testRemoveBFromASide() {
        log.info("testRemoveBFromASide begins");
        final BLabel label = testData.getB().get(0);

        // A removes B (break A-B relationship).
        testData.getA().forEach(album -> album.getLabels().remove(label));

        // Now remove B.
        em.remove(label);
        log.info("testRemoveBFromASide ends");
    }

    void testRemoveA() {
        log.info("testRemoveA begins");
        final AAlbum album = testData.getA().get(0);
        em.remove(album);
        log.info("testRemoveA ends");
    }

    void testAddAWithMultipleSameBs() {
        log.info("testAddAWithMultipleSameBs begins");
        final BLabel label = new BLabel("label-20");
        em.persist(label);

        final AAlbum album = new AAlbum("album-20");
        album.getLabels().add(label);
        album.getLabels().add(label);
        em.persist(album);
        log.info("testAddAWithMultipleSameBs ends");
    }
}
