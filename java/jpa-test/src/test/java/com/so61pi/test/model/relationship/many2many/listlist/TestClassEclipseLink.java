package com.so61pi.test.model.relationship.many2many.listlist;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * CREATE TABLE btags (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE aemployees (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE aemployees_btags (tags_ID BIGINT NOT NULL, employees_ID BIGINT NOT NULL, PRIMARY KEY (tags_ID, employees_ID))
 * ALTER TABLE aemployees_btags ADD CONSTRAINT FK_aemployees_btags_employees_ID FOREIGN KEY (employees_ID) REFERENCES aemployees (ID)
 * ALTER TABLE aemployees_btags ADD CONSTRAINT FK_aemployees_btags_tags_ID FOREIGN KEY (tags_ID) REFERENCES btags (ID)
 * </pre>
 */
class TestClassEclipseLink {
    private TestClass t = new TestClass("test-persistence-unit-eclipselink");

    /**
     * <pre>
     * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
     * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
     * INSERT INTO aemployees (ID, NAME) VALUES (?, ?)
     * INSERT INTO aemployees (ID, NAME) VALUES (?, ?)
     * INSERT INTO aemployees (ID, NAME) VALUES (?, ?)
     * INSERT INTO btags (ID, NAME) VALUES (?, ?)
     * INSERT INTO btags (ID, NAME) VALUES (?, ?)
     * INSERT INTO btags (ID, NAME) VALUES (?, ?)
     * INSERT INTO aemployees_btags (tags_ID, employees_ID) VALUES (?, ?)
     * INSERT INTO aemployees_btags (tags_ID, employees_ID) VALUES (?, ?)
     * INSERT INTO aemployees_btags (tags_ID, employees_ID) VALUES (?, ?)
     * INSERT INTO aemployees_btags (tags_ID, employees_ID) VALUES (?, ?)
     * INSERT INTO aemployees_btags (tags_ID, employees_ID) VALUES (?, ?)
     * INSERT INTO aemployees_btags (tags_ID, employees_ID) VALUES (?, ?)
     * INSERT INTO aemployees_btags (tags_ID, employees_ID) VALUES (?, ?)
     * INSERT INTO aemployees_btags (tags_ID, employees_ID) VALUES (?, ?)
     * INSERT INTO aemployees_btags (tags_ID, employees_ID) VALUES (?, ?)
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
     * INSERT INTO aemployees (ID, NAME) VALUES (?, ?)
     * INSERT INTO btags (ID, NAME) VALUES (?, ?)
     * INSERT INTO aemployees_btags (tags_ID, employees_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * INSERT INTO aemployees (ID, NAME) VALUES (?, ?)
     * INSERT INTO btags (ID, NAME) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromBSide() {
        t.testMakeRelationshipFromBSide();
    }

    /**
     * <pre>
     * DELETE FROM aemployees_btags WHERE (tags_ID = ?)
     * DELETE FROM btags WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * DELETE FROM aemployees_btags WHERE (tags_ID = ?)
     * DELETE FROM aemployees_btags WHERE ((tags_ID = ?) AND (employees_ID = ?))
     * DELETE FROM aemployees_btags WHERE ((tags_ID = ?) AND (employees_ID = ?))
     * DELETE FROM aemployees_btags WHERE ((tags_ID = ?) AND (employees_ID = ?))
     * DELETE FROM btags WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * DELETE FROM aemployees_btags WHERE (employees_ID = ?)
     * DELETE FROM aemployees WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * DELETE FROM aemployees_btags WHERE (employees_ID = ?)
     * DELETE FROM aemployees WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveAFromBSide() {
        t.testRemoveAFromBSide();
    }

    /**
     * <pre>
     * INSERT INTO aemployees (ID, NAME) VALUES (?, ?)
     * INSERT INTO btags (ID, NAME) VALUES (?, ?)
     * INSERT INTO aemployees_btags (tags_ID, employees_ID) VALUES (?, ?)
     * INSERT INTO aemployees_btags (tags_ID, employees_ID) VALUES (?, ?)
     * Internal Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Unique index or primary key violation: "PUBLIC.PRIMARY_KEY_E2 ON PUBLIC.AEMPLOYEES_BTAGS(TAGS_ID, EMPLOYEES_ID) VALUES 10"; SQL statement:
     * INSERT INTO aemployees_btags (tags_ID, employees_ID) VALUES (?, ?) [23505-199]
     * </pre>
     */
    @Test
    void testAddAWithMultipleSameBs() {
        t.testAddAWithMultipleSameBs();
    }

    /**
     * <pre>
     * INSERT INTO aemployees (ID, NAME) VALUES (?, ?)
     * INSERT INTO btags (ID, NAME) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testAddBWithMultipleSameAs() {
        t.testAddBWithMultipleSameAs();
    }
}
