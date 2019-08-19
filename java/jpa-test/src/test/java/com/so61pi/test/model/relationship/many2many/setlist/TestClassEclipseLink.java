package com.so61pi.test.model.relationship.many2many.setlist;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * CREATE TABLE bsongs (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE aartists (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE aartists_bsongs (songs_ID BIGINT NOT NULL, artists_ID BIGINT NOT NULL, PRIMARY KEY (songs_ID, artists_ID))
 * ALTER TABLE aartists_bsongs ADD CONSTRAINT FK_aartists_bsongs_artists_ID FOREIGN KEY (artists_ID) REFERENCES aartists (ID)
 * ALTER TABLE aartists_bsongs ADD CONSTRAINT FK_aartists_bsongs_songs_ID FOREIGN KEY (songs_ID) REFERENCES bsongs (ID)
 * </pre>
 */
class TestClassEclipseLink {
    private TestClass t = new TestClass("test-persistence-unit-eclipselink");

    /**
     * <pre>
     * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
     * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
     * INSERT INTO aartists (ID, NAME) VALUES (?, ?)
     * INSERT INTO aartists (ID, NAME) VALUES (?, ?)
     * INSERT INTO aartists (ID, NAME) VALUES (?, ?)
     * INSERT INTO bsongs (ID, NAME) VALUES (?, ?)
     * INSERT INTO bsongs (ID, NAME) VALUES (?, ?)
     * INSERT INTO bsongs (ID, NAME) VALUES (?, ?)
     * INSERT INTO aartists_bsongs (songs_ID, artists_ID) VALUES (?, ?)
     * INSERT INTO aartists_bsongs (songs_ID, artists_ID) VALUES (?, ?)
     * INSERT INTO aartists_bsongs (songs_ID, artists_ID) VALUES (?, ?)
     * INSERT INTO aartists_bsongs (songs_ID, artists_ID) VALUES (?, ?)
     * INSERT INTO aartists_bsongs (songs_ID, artists_ID) VALUES (?, ?)
     * INSERT INTO aartists_bsongs (songs_ID, artists_ID) VALUES (?, ?)
     * INSERT INTO aartists_bsongs (songs_ID, artists_ID) VALUES (?, ?)
     * INSERT INTO aartists_bsongs (songs_ID, artists_ID) VALUES (?, ?)
     * INSERT INTO aartists_bsongs (songs_ID, artists_ID) VALUES (?, ?)
     * </pre>
     */
    @BeforeEach
    void setUp() {
        t.setUp();
    }

    @AfterEach
    void tearDown() {
        t.tearDown();
    }

    /**
     * <pre>
     * INSERT INTO aartists (ID, NAME) VALUES (?, ?)
     * INSERT INTO bsongs (ID, NAME) VALUES (?, ?)
     * INSERT INTO aartists_bsongs (songs_ID, artists_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * INSERT INTO aartists (ID, NAME) VALUES (?, ?)
     * INSERT INTO bsongs (ID, NAME) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromBSide() {
        t.testMakeRelationshipFromBSide();
    }

    /**
     * <pre>
     * DELETE FROM aartists_bsongs WHERE (songs_ID = ?)
     * DELETE FROM bsongs WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * DELETE FROM aartists_bsongs WHERE (songs_ID = ?)
     * DELETE FROM aartists_bsongs WHERE ((songs_ID = ?) AND (artists_ID = ?))
     * DELETE FROM aartists_bsongs WHERE ((songs_ID = ?) AND (artists_ID = ?))
     * DELETE FROM aartists_bsongs WHERE ((songs_ID = ?) AND (artists_ID = ?))
     * DELETE FROM bsongs WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * DELETE FROM aartists_bsongs WHERE (artists_ID = ?)
     * DELETE FROM aartists WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * DELETE FROM aartists_bsongs WHERE (artists_ID = ?)
     * DELETE FROM aartists WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveAFromBSide() {
        t.testRemoveAFromBSide();
    }

    /**
     * <pre>
     * INSERT INTO aartists (ID, NAME) VALUES (?, ?)
     * INSERT INTO bsongs (ID, NAME) VALUES (?, ?)
     * INSERT INTO aartists_bsongs (songs_ID, artists_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testAddAWithMultipleSameBs() {
        t.testAddAWithMultipleSameBs();
    }

    /**
     * <pre>
     * INSERT INTO aartists (ID, NAME) VALUES (?, ?)
     * INSERT INTO bsongs (ID, NAME) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testAddBWithMultipleSameAs() {
        t.testAddBWithMultipleSameAs();
    }
}
