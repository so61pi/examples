package com.so61pi.test.model.relationship.many2many.setlist;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.ArrayList;
import java.util.List;

@Getter
class TestData {
    private List<BSong> b = new ArrayList<>();
    private List<AArtist> a = new ArrayList<>();

    TestData() {
        // Create data.
        b.add(new BSong("song-0"));
        b.add(new BSong("song-1"));
        b.add(new BSong("song-2"));
        a.add(new AArtist("artist-0"));
        a.add(new AArtist("artist-1"));
        a.add(new AArtist("artist-2"));

        // Link data together.
        a.forEach(artist -> b.forEach(song -> artist.getSongs().add(song)));
        b.forEach(song -> a.forEach(artist -> song.getArtists().add(artist)));
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
        final AArtist artist = new AArtist("artist-10");
        em.persist(artist);

        final BSong song = new BSong("song-10");
        em.persist(song);

        artist.getSongs().add(song);
        log.info("testMakeRelationshipFromASide ends");
    }

    void testMakeRelationshipFromBSide() {
        log.info("testMakeRelationshipFromBSide begins");
        final AArtist artist = new AArtist("artist-20");
        em.persist(artist);

        final BSong song = new BSong("song-20");
        em.persist(song);

        song.getArtists().add(artist);
        log.info("testMakeRelationshipFromBSide ends");
    }

    void testRemoveB() {
        log.info("testRemoveB begins");
        final BSong song = testData.getB().get(0);
        em.remove(song);
        log.info("testRemoveB ends");
    }

    void testRemoveBFromASide() {
        log.info("testRemoveBFromASide begins");
        final BSong song = testData.getB().get(0);

        // A removes B (break A-B relationship).
        testData.getA().forEach(artist -> artist.getSongs().remove(song));

        // Now remove B.
        em.remove(song);
        log.info("testRemoveBFromASide ends");
    }

    void testRemoveA() {
        log.info("testRemoveA begins");
        final AArtist artist = testData.getA().get(0);
        em.remove(artist);
        log.info("testRemoveA ends");
    }

    void testRemoveAFromBSide() {
        log.info("testRemoveAFromBSide begins");
        final AArtist artist = testData.getA().get(0);

        // B removes A (break A-B relationship).
        testData.getB().forEach(song -> song.getArtists().remove(artist));

        // Now remove A.
        em.remove(artist);
        log.info("testRemoveAFromBSide ends");
    }

    void testAddAWithMultipleSameBs() {
        log.info("testAddAWithMultipleSameBs begins");
        final BSong song = new BSong("song-30");
        em.persist(song);

        final AArtist artist = new AArtist("artist-30");
        artist.getSongs().add(song);
        artist.getSongs().add(song);
        em.persist(artist);
        log.info("testAddAWithMultipleSameBs ends");
    }

    void testAddBWithMultipleSameAs() {
        log.info("testAddBWithMultipleSameAs begins");
        final AArtist artist = new AArtist("artist-40");
        em.persist(artist);

        final BSong song = new BSong("song-40");
        song.getArtists().add(artist);
        song.getArtists().add(artist);
        em.persist(song);
        log.info("testAddBWithMultipleSameAs ends");
    }
}
