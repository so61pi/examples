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

    void testMakeRelationshipFromArtistSide() {
        log.info("testMakeRelationshipFromArtistSide begins");
        final AArtist artist = new AArtist("artist-10");
        em.persist(artist);

        final BSong song = new BSong("song-10");
        em.persist(song);

        artist.getSongs().add(song);
        log.info("testMakeRelationshipFromArtistSide ends");
    }

    void testMakeRelationshipFromSongSide() {
        log.info("testMakeRelationshipFromSongSide begins");
        final AArtist artist = new AArtist("artist-20");
        em.persist(artist);

        final BSong song = new BSong("song-20");
        em.persist(song);

        song.getArtists().add(artist);
        log.info("testMakeRelationshipFromSongSide ends");
    }

    void testRemoveSongFromSongSide() {
        log.info("testRemoveSongFromSongSide begins");
        final BSong song = testData.getB().get(0);
        em.remove(song);
        log.info("testRemoveSongFromSongSide ends");
    }

    void testRemoveSongFromArtistSide() {
        log.info("testRemoveSongFromArtistSide begins");
        final BSong song = testData.getB().get(0);

        // Break artist-song relationships for all artists.
        testData.getA().forEach(artist -> artist.getSongs().remove(song));

        // Now remove song.
        em.remove(song);
        log.info("testRemoveSongFromArtistSide ends");
    }

    void testRemoveArtistFromSongSide() {
        log.info("testRemoveArtistFromSongSide begins");
        final AArtist artist = testData.getA().get(0);

        // Break song-artist relationships for all songs.
        testData.getB().forEach(song -> song.getArtists().remove(artist));

        // Now remove artist.
        em.remove(artist);
        log.info("testRemoveArtistFromSongSide ends");
    }

    void testRemoveArtistFromArtistSide() {
        log.info("testRemoveArtistFromArtistSide begins");
        final AArtist artist = testData.getA().get(0);
        em.remove(artist);
        log.info("testRemoveArtistFromArtistSide ends");
    }

    void testAddArtistWithMultipleSameSongs() {
        log.info("testAddArtistWithMultipleSameSongs begins");
        final BSong song = new BSong("song-30");
        em.persist(song);

        final AArtist artist = new AArtist("artist-30");
        artist.getSongs().add(song);
        artist.getSongs().add(song);
        em.persist(artist);
        log.info("testAddArtistWithMultipleSameSongs ends");
    }

    void testAddSongWithMultipleSameArtists() {
        log.info("testAddSongWithMultipleSameArtists begins");
        final AArtist artist = new AArtist("artist-40");
        em.persist(artist);

        final BSong song = new BSong("song-40");
        song.getArtists().add(artist);
        song.getArtists().add(artist);
        em.persist(song);
        log.info("testAddSongWithMultipleSameArtists ends");
    }
}
