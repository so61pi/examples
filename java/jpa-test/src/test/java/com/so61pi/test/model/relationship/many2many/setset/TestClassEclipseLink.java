package com.so61pi.test.model.relationship.many2many.setset;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * CREATE TABLE aprojects (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE blanguages (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE aprojects_blanguages (projects_ID BIGINT NOT NULL, languages_ID BIGINT NOT NULL, PRIMARY KEY (projects_ID, languages_ID))
 * ALTER TABLE aprojects_blanguages ADD CONSTRAINT FK_aprojects_blanguages_projects_ID FOREIGN KEY (projects_ID) REFERENCES aprojects (ID)
 * ALTER TABLE aprojects_blanguages ADD CONSTRAINT FK_aprojects_blanguages_languages_ID FOREIGN KEY (languages_ID) REFERENCES blanguages (ID)
 * </pre>
 */
class TestClassEclipseLink {
    private TestClass t = new TestClass("test-persistence-unit-eclipselink");

    /**
     * <pre>
     * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
     * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
     * INSERT INTO aprojects (ID, NAME) VALUES (?, ?)
     * INSERT INTO aprojects (ID, NAME) VALUES (?, ?)
     * INSERT INTO aprojects (ID, NAME) VALUES (?, ?)
     * INSERT INTO blanguages (ID, NAME) VALUES (?, ?)
     * INSERT INTO blanguages (ID, NAME) VALUES (?, ?)
     * INSERT INTO blanguages (ID, NAME) VALUES (?, ?)
     * INSERT INTO aprojects_blanguages (languages_ID, projects_ID) VALUES (?, ?)
     * INSERT INTO aprojects_blanguages (languages_ID, projects_ID) VALUES (?, ?)
     * INSERT INTO aprojects_blanguages (languages_ID, projects_ID) VALUES (?, ?)
     * INSERT INTO aprojects_blanguages (languages_ID, projects_ID) VALUES (?, ?)
     * INSERT INTO aprojects_blanguages (languages_ID, projects_ID) VALUES (?, ?)
     * INSERT INTO aprojects_blanguages (languages_ID, projects_ID) VALUES (?, ?)
     * INSERT INTO aprojects_blanguages (languages_ID, projects_ID) VALUES (?, ?)
     * INSERT INTO aprojects_blanguages (languages_ID, projects_ID) VALUES (?, ?)
     * INSERT INTO aprojects_blanguages (languages_ID, projects_ID) VALUES (?, ?)
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
     * INSERT INTO aprojects (ID, NAME) VALUES (?, ?)
     * INSERT INTO blanguages (ID, NAME) VALUES (?, ?)
     * INSERT INTO aprojects_blanguages (languages_ID, projects_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * INSERT INTO aprojects (ID, NAME) VALUES (?, ?)
     * INSERT INTO blanguages (ID, NAME) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromBSide() {
        t.testMakeRelationshipFromBSide();
    }

    /**
     * <pre>
     * DELETE FROM aprojects_blanguages WHERE (languages_ID = ?)
     * DELETE FROM blanguages WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * DELETE FROM aprojects_blanguages WHERE (languages_ID = ?)
     * DELETE FROM aprojects_blanguages WHERE ((languages_ID = ?) AND (projects_ID = ?))
     * DELETE FROM aprojects_blanguages WHERE ((languages_ID = ?) AND (projects_ID = ?))
     * DELETE FROM aprojects_blanguages WHERE ((languages_ID = ?) AND (projects_ID = ?))
     * DELETE FROM blanguages WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * DELETE FROM aprojects_blanguages WHERE (projects_ID = ?)
     * DELETE FROM aprojects WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * DELETE FROM aprojects_blanguages WHERE (projects_ID = ?)
     * DELETE FROM aprojects WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveAFromBSide() {
        t.testRemoveAFromBSide();
    }

    /**
     * <pre>
     * INSERT INTO aprojects (ID, NAME) VALUES (?, ?)
     * INSERT INTO blanguages (ID, NAME) VALUES (?, ?)
     * INSERT INTO aprojects_blanguages (languages_ID, projects_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testAddAWithMultipleSameBs() {
        t.testAddAWithMultipleSameBs();
    }

    /**
     * <pre>
     * INSERT INTO aprojects (ID, NAME) VALUES (?, ?)
     * INSERT INTO blanguages (ID, NAME) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testAddBWithMultipleSameAs() {
        t.testAddBWithMultipleSameAs();
    }
}
