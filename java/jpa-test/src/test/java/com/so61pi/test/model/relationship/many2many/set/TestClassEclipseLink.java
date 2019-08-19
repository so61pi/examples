package com.so61pi.test.model.relationship.many2many.set;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * CREATE TABLE aalbums (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE blabels (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE aalbums_blabels (AAlbum_ID BIGINT NOT NULL, labels_ID BIGINT NOT NULL, PRIMARY KEY (AAlbum_ID, labels_ID))
 * ALTER TABLE aalbums_blabels ADD CONSTRAINT FK_aalbums_blabels_labels_ID FOREIGN KEY (labels_ID) REFERENCES blabels (ID)
 * ALTER TABLE aalbums_blabels ADD CONSTRAINT FK_aalbums_blabels_AAlbum_ID FOREIGN KEY (AAlbum_ID) REFERENCES aalbums (ID)
 * </pre>
 */
class TestClassEclipseLink {
    private TestClass t = new TestClass("test-persistence-unit-eclipselink");

    /**
     * <pre>
     * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
     * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
     * INSERT INTO aalbums (ID, NAME) VALUES (?, ?)
     * INSERT INTO aalbums (ID, NAME) VALUES (?, ?)
     * INSERT INTO aalbums (ID, NAME) VALUES (?, ?)
     * INSERT INTO blabels (ID, NAME) VALUES (?, ?)
     * INSERT INTO blabels (ID, NAME) VALUES (?, ?)
     * INSERT INTO blabels (ID, NAME) VALUES (?, ?)
     * INSERT INTO aalbums_blabels (labels_ID, AAlbum_ID) VALUES (?, ?)
     * INSERT INTO aalbums_blabels (labels_ID, AAlbum_ID) VALUES (?, ?)
     * INSERT INTO aalbums_blabels (labels_ID, AAlbum_ID) VALUES (?, ?)
     * INSERT INTO aalbums_blabels (labels_ID, AAlbum_ID) VALUES (?, ?)
     * INSERT INTO aalbums_blabels (labels_ID, AAlbum_ID) VALUES (?, ?)
     * INSERT INTO aalbums_blabels (labels_ID, AAlbum_ID) VALUES (?, ?)
     * INSERT INTO aalbums_blabels (labels_ID, AAlbum_ID) VALUES (?, ?)
     * INSERT INTO aalbums_blabels (labels_ID, AAlbum_ID) VALUES (?, ?)
     * INSERT INTO aalbums_blabels (labels_ID, AAlbum_ID) VALUES (?, ?)
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
     * INSERT INTO aalbums (ID, NAME) VALUES (?, ?)
     * INSERT INTO blabels (ID, NAME) VALUES (?, ?)
     * INSERT INTO aalbums_blabels (labels_ID, AAlbum_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * DELETE FROM blabels WHERE (ID = ?)
     * Internal Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Referential integrity constraint violation: "FK_AALBUMS_BLABELS_LABELS_ID: PUBLIC.AALBUMS_BLABELS FOREIGN KEY(LABELS_ID) REFERENCES PUBLIC.BLABELS(ID) (4)"; SQL statement:
     * DELETE FROM blabels WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * DELETE FROM aalbums_blabels WHERE ((labels_ID = ?) AND (AAlbum_ID = ?))
     * DELETE FROM aalbums_blabels WHERE ((labels_ID = ?) AND (AAlbum_ID = ?))
     * DELETE FROM aalbums_blabels WHERE ((labels_ID = ?) AND (AAlbum_ID = ?))
     * DELETE FROM blabels WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * DELETE FROM aalbums_blabels WHERE (AAlbum_ID = ?)
     * DELETE FROM aalbums WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * INSERT INTO aalbums (ID, NAME) VALUES (?, ?)
     * INSERT INTO blabels (ID, NAME) VALUES (?, ?)
     * INSERT INTO aalbums_blabels (labels_ID, AAlbum_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testAddAWithMultipleSameBs() {
        t.testAddAWithMultipleSameBs();
    }
}
