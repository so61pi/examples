package com.so61pi.test.model.relationship.one2one.both;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * CREATE TABLE baddresses (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE aworkers (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, ADDRESS_ID BIGINT, PRIMARY KEY (ID))
 * ALTER TABLE aworkers ADD CONSTRAINT FK_aworkers_ADDRESS_ID FOREIGN KEY (ADDRESS_ID) REFERENCES baddresses (ID)
 * </pre>
 */
class TestClassEclipseLink {
    private TestClass t = new TestClass("test-persistence-unit-eclipselink");

    /**
     * <pre>
     * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
     * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
     * INSERT INTO baddresses (ID, NAME) VALUES (?, ?)
     * INSERT INTO aworkers (ID, NAME, ADDRESS_ID) VALUES (?, ?, ?)
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
     * INSERT INTO baddresses (ID, NAME) VALUES (?, ?)
     * INSERT INTO aworkers (ID, NAME, ADDRESS_ID) VALUES (?, ?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * INSERT INTO baddresses (ID, NAME) VALUES (?, ?)
     * INSERT INTO aworkers (ID, NAME, ADDRESS_ID) VALUES (?, ?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromBSide() {
        t.testMakeRelationshipFromBSide();
    }

    /**
     * <pre>
     * DELETE FROM baddresses WHERE (ID = ?)
     * Internal Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Referential integrity constraint violation: "FK_AWORKERS_ADDRESS_ID: PUBLIC.AWORKERS FOREIGN KEY(ADDRESS_ID) REFERENCES PUBLIC.BADDRESSES(ID) (2)"; SQL statement:
     * DELETE FROM baddresses WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * UPDATE aworkers SET ADDRESS_ID = ? WHERE (ID = ?)
     * DELETE FROM baddresses WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * DELETE FROM aworkers WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * DELETE FROM aworkers WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveAFromBSide() {
        t.testRemoveAFromBSide();
    }
}
