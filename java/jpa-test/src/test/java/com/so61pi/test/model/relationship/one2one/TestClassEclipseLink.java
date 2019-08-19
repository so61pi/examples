package com.so61pi.test.model.relationship.one2one;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * CREATE TABLE baccounts (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE aemails (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, ACCOUNT_ID BIGINT, PRIMARY KEY (ID))
 * ALTER TABLE aemails ADD CONSTRAINT FK_aemails_ACCOUNT_ID FOREIGN KEY (ACCOUNT_ID) REFERENCES baccounts (ID)
 * </pre>
 */
class TestClassEclipseLink {
    private TestClass t = new TestClass("test-persistence-unit-eclipselink");

    /**
     * <pre>
     * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
     * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
     * INSERT INTO baccounts (ID, NAME) VALUES (?, ?)
     * INSERT INTO aemails (ID, NAME, ACCOUNT_ID) VALUES (?, ?, ?)
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
     * INSERT INTO baccounts (ID, NAME) VALUES (?, ?)
     * INSERT INTO aemails (ID, NAME, ACCOUNT_ID) VALUES (?, ?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * DELETE FROM baccounts WHERE (ID = ?)
     * Internal Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Referential integrity constraint violation: "FK_AEMAILS_ACCOUNT_ID: PUBLIC.AEMAILS FOREIGN KEY(ACCOUNT_ID) REFERENCES PUBLIC.BACCOUNTS(ID) (2)"; SQL statement:
     * DELETE FROM baccounts WHERE (ID = ?) [23503-199]
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * UPDATE aemails SET ACCOUNT_ID = ? WHERE (ID = ?)
     * DELETE FROM baccounts WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * DELETE FROM aemails WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }
}
