package com.so61pi.test.model.relationship.many2one;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * CREATE TABLE achildren (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, MOTHER_ID BIGINT, PRIMARY KEY (ID))
 * CREATE TABLE bmothers (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * ALTER TABLE achildren ADD CONSTRAINT FK_achildren_MOTHER_ID FOREIGN KEY (MOTHER_ID) REFERENCES bmothers (ID)
 * </pre>
 */
class TestClassEclipseLink {
    private TestClass t = new TestClass("test-persistence-unit-eclipselink");

    /**
     * <pre>
     * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
     * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
     * INTO bmothers (ID, NAME) VALUES (?, ?)
     * INTO achildren (ID, NAME, MOTHER_ID) VALUES (?, ?, ?)
     * INTO achildren (ID, NAME, MOTHER_ID) VALUES (?, ?, ?)
     * INTO achildren (ID, NAME, MOTHER_ID) VALUES (?, ?, ?)
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
     * INTO bmothers (ID, NAME) VALUES (?, ?)
     * INTO achildren (ID, NAME, MOTHER_ID) VALUES (?, ?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * DELETE FROM bmothers WHERE (ID = ?)
     * Internal Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Referential integrity constraint violation: "FK_ACHILDREN_MOTHER_ID: PUBLIC.ACHILDREN FOREIGN KEY(MOTHER_ID) REFERENCES PUBLIC.BMOTHERS(ID) (1)"; SQL statement:
     * DELETE FROM bmothers WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * UPDATE achildren SET MOTHER_ID = ? WHERE (ID = ?)
     * UPDATE achildren SET MOTHER_ID = ? WHERE (ID = ?)
     * UPDATE achildren SET MOTHER_ID = ? WHERE (ID = ?)
     * DELETE FROM bmothers WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * DELETE FROM achildren WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }
}
