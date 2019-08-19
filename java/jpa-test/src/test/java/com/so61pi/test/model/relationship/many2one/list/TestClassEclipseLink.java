package com.so61pi.test.model.relationship.many2one.list;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * CREATE TABLE acomments (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, POST_ID BIGINT, PRIMARY KEY (ID))
 * CREATE TABLE bposts (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * ALTER TABLE acomments ADD CONSTRAINT FK_acomments_POST_ID FOREIGN KEY (POST_ID) REFERENCES bposts (ID)
 * </pre>
 */
class TestClassEclipseLink {
    private TestClass t = new TestClass("test-persistence-unit-eclipselink");

    /**
     * <pre>
     * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
     * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
     * INSERT INTO bposts (ID, NAME) VALUES (?, ?)
     * INSERT INTO acomments (ID, NAME, POST_ID) VALUES (?, ?, ?)
     * INSERT INTO acomments (ID, NAME, POST_ID) VALUES (?, ?, ?)
     * INSERT INTO acomments (ID, NAME, POST_ID) VALUES (?, ?, ?)
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
     * INSERT INTO bposts (ID, NAME) VALUES (?, ?)
     * INSERT INTO acomments (ID, NAME, POST_ID) VALUES (?, ?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * INSERT INTO bposts (ID, NAME) VALUES (?, ?)
     * INSERT INTO acomments (ID, NAME, POST_ID) VALUES (?, ?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromBSide() {
        t.testMakeRelationshipFromBSide();
    }

    /**
     * <pre>
     * DELETE FROM bposts WHERE (ID = ?)
     * Internal Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Referential integrity constraint violation: "FK_ACOMMENTS_POST_ID: PUBLIC.ACOMMENTS FOREIGN KEY(POST_ID) REFERENCES PUBLIC.BPOSTS(ID) (1)"; SQL statement:
     * DELETE FROM bposts WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * UPDATE acomments SET POST_ID = ? WHERE (ID = ?)
     * UPDATE acomments SET POST_ID = ? WHERE (ID = ?)
     * UPDATE acomments SET POST_ID = ? WHERE (ID = ?)
     * DELETE FROM bposts WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * DELETE FROM acomments WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * DELETE FROM acomments WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveAFromBSide() {
        t.testRemoveAFromBSide();
    }

    /**
     * <pre>
     * INSERT INTO bposts (ID, NAME) VALUES (?, ?)
     * INSERT INTO acomments (ID, NAME, POST_ID) VALUES (?, ?, ?)
     * </pre>
     */
    @Test
    void testAddBWithMultipleSameAs() {
        t.testAddBWithMultipleSameAs();
    }
}
