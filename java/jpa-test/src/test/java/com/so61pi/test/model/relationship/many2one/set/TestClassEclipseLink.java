package com.so61pi.test.model.relationship.many2one.set;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * CREATE TABLE amembers (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, FAMILY_ID BIGINT, PRIMARY KEY (ID))
 * CREATE TABLE bfamilies (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * ALTER TABLE amembers ADD CONSTRAINT FK_amembers_FAMILY_ID FOREIGN KEY (FAMILY_ID) REFERENCES bfamilies (ID)
 * </pre>
 */
class TestClassEclipseLink {
    private TestClass t = new TestClass("test-persistence-unit-eclipselink");

    /**
     * <pre>
     * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
     * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
     * INSERT INTO bfamilies (ID, NAME) VALUES (?, ?)
     * INSERT INTO amembers (ID, NAME, FAMILY_ID) VALUES (?, ?, ?)
     * INSERT INTO amembers (ID, NAME, FAMILY_ID) VALUES (?, ?, ?)
     * INSERT INTO amembers (ID, NAME, FAMILY_ID) VALUES (?, ?, ?)
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
     * INSERT INTO bfamilies (ID, NAME) VALUES (?, ?)
     * INSERT INTO amembers (ID, NAME, FAMILY_ID) VALUES (?, ?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * INSERT INTO bfamilies (ID, NAME) VALUES (?, ?)
     * INSERT INTO amembers (ID, NAME, FAMILY_ID) VALUES (?, ?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromBSide() {
        t.testMakeRelationshipFromBSide();
    }

    /**
     * <pre>
     * DELETE FROM bfamilies WHERE (ID = ?)
     * Internal Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Referential integrity constraint violation: "FK_AMEMBERS_FAMILY_ID: PUBLIC.AMEMBERS FOREIGN KEY(FAMILY_ID) REFERENCES PUBLIC.BFAMILIES(ID) (1)"; SQL statement:
     * DELETE FROM bfamilies WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * UPDATE amembers SET FAMILY_ID = ? WHERE (ID = ?)
     * UPDATE amembers SET FAMILY_ID = ? WHERE (ID = ?)
     * UPDATE amembers SET FAMILY_ID = ? WHERE (ID = ?)
     * DELETE FROM bfamilies WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * DELETE FROM amembers WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * DELETE FROM amembers WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveAFromBSide() {
        t.testRemoveAFromBSide();
    }

    /**
     * <pre>
     * INSERT INTO bfamilies (ID, NAME) VALUES (?, ?)
     * INSERT INTO amembers (ID, NAME, FAMILY_ID) VALUES (?, ?, ?)
     * </pre>
     */
    @Test
    void testAddBWithMultipleSameAs() {
        t.testAddBWithMultipleSameAs();
    }
}
