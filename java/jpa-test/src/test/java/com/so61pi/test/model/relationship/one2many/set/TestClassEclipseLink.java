package com.so61pi.test.model.relationship.one2many.set;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * CREATE TABLE acities (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE bapartments (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE acities_bapartments (ACity_ID BIGINT NOT NULL, apartments_ID BIGINT NOT NULL, PRIMARY KEY (ACity_ID, apartments_ID))
 * ALTER TABLE acities_bapartments ADD CONSTRAINT FK_acities_bapartments_apartments_ID FOREIGN KEY (apartments_ID) REFERENCES bapartments (ID)
 * ALTER TABLE acities_bapartments ADD CONSTRAINT FK_acities_bapartments_ACity_ID FOREIGN KEY (ACity_ID) REFERENCES acities (ID)
 * </pre>
 */
class TestClassEclipseLink {
    private TestClass t = new TestClass("test-persistence-unit-eclipselink");

    /**
     * <pre>
     * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
     * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
     * INSERT INTO acities (ID, NAME) VALUES (?, ?)
     * INSERT INTO bapartments (ID, NAME) VALUES (?, ?)
     * INSERT INTO bapartments (ID, NAME) VALUES (?, ?)
     * INSERT INTO bapartments (ID, NAME) VALUES (?, ?)
     * INSERT INTO acities_bapartments (apartments_ID, ACity_ID) VALUES (?, ?)
     * INSERT INTO acities_bapartments (apartments_ID, ACity_ID) VALUES (?, ?)
     * INSERT INTO acities_bapartments (apartments_ID, ACity_ID) VALUES (?, ?)
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
     * INSERT INTO acities (ID, NAME) VALUES (?, ?)
     * INSERT INTO bapartments (ID, NAME) VALUES (?, ?)
     * INSERT INTO acities_bapartments (apartments_ID, ACity_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * DELETE FROM bapartments WHERE (ID = ?)
     * Internal Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Referential integrity constraint violation: "FK_ACITIES_BAPARTMENTS_APARTMENTS_ID: PUBLIC.ACITIES_BAPARTMENTS FOREIGN KEY(APARTMENTS_ID) REFERENCES PUBLIC.BAPARTMENTS(ID) (2)"; SQL statement:
     * DELETE FROM bapartments WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * DELETE FROM acities_bapartments WHERE ((apartments_ID = ?) AND (ACity_ID = ?))
     * DELETE FROM bapartments WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * DELETE FROM acities_bapartments WHERE (ACity_ID = ?)
     * DELETE FROM acities WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * INSERT INTO acities (ID, NAME) VALUES (?, ?)
     * INSERT INTO bapartments (ID, NAME) VALUES (?, ?)
     * INSERT INTO acities_bapartments (apartments_ID, ACity_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testAddAWithMultipleSameBs() {
        t.testAddAWithMultipleSameBs();
    }
}
