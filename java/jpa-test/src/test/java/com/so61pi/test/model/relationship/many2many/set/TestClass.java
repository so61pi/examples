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
    private List<BLabel> labels = new ArrayList<>();
    private List<AAlbum> albums = new ArrayList<>();

    TestData() {
        // Create data.
        labels.add(new BLabel("label-0"));
        labels.add(new BLabel("label-1"));
        labels.add(new BLabel("label-2"));
        albums.add(new AAlbum("album-0"));
        albums.add(new AAlbum("album-1"));
        albums.add(new AAlbum("album-2"));

        // Link data together.
        albums.forEach(album -> labels.forEach(label -> album.getLabels().add(label)));
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
        testData.getAlbums().forEach(em::persist);
        testData.getLabels().forEach(em::persist);
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

    void testMakeRelationshipFromAlbumSide() {
        log.info("testMakeRelationshipFromAlbumSide begins");
        final BLabel label = new BLabel("label-10");
        em.persist(label);

        final AAlbum album = new AAlbum("album-10");
        em.persist(album);

        album.getLabels().add(label);
        log.info("testMakeRelationshipFromAlbumSide ends");
    }

    void testRemoveLabelFromLabelSide() {
        log.info("testRemoveLabelFromLabelSide begins");
        final BLabel label = testData.getLabels().get(0);
        em.remove(label);
        log.info("testRemoveLabelFromLabelSide ends");
    }

    void testRemoveLabelFromAlbumSide() {
        log.info("testRemoveLabelFromAlbumSide begins");
        final BLabel label = testData.getLabels().get(0);

        // Break album-label relationships for all albums.
        testData.getAlbums().forEach(album -> album.getLabels().remove(label));

        // Now remove label.
        em.remove(label);
        log.info("testRemoveLabelFromAlbumSide ends");
    }

    void testRemoveAlbum() {
        log.info("testRemoveAlbum begins");
        final AAlbum album = testData.getAlbums().get(0);
        em.remove(album);
        log.info("testRemoveAlbum ends");
    }

    void testAddAlbumWithMultipleSameLabels() {
        log.info("testAddAlbumWithMultipleSameLabels begins");
        final BLabel label = new BLabel("label-20");
        em.persist(label);

        final AAlbum album = new AAlbum("album-20");
        album.getLabels().add(label);
        album.getLabels().add(label);
        em.persist(album);
        log.info("testAddAlbumWithMultipleSameLabels ends");
    }
}
