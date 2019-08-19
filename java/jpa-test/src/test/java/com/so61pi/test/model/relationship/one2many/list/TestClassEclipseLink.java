package com.so61pi.test.model.relationship.one2many.list;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * CREATE TABLE acarts (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE bproducts (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE acarts_bproducts (ACart_ID BIGINT NOT NULL, products_ID BIGINT NOT NULL, PRIMARY KEY (ACart_ID, products_ID))
 * ALTER TABLE acarts_bproducts ADD CONSTRAINT FK_acarts_bproducts_products_ID FOREIGN KEY (products_ID) REFERENCES bproducts (ID)
 * ALTER TABLE acarts_bproducts ADD CONSTRAINT FK_acarts_bproducts_ACart_ID FOREIGN KEY (ACart_ID) REFERENCES acarts (ID)
 * </pre>
 */
class TestClassEclipseLink {
    private TestClass t = new TestClass("test-persistence-unit-eclipselink");

    /**
     * <pre>
     * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
     * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
     * INSERT INTO acarts (ID, NAME) VALUES (?, ?)
     * INSERT INTO bproducts (ID, NAME) VALUES (?, ?)
     * INSERT INTO bproducts (ID, NAME) VALUES (?, ?)
     * INSERT INTO bproducts (ID, NAME) VALUES (?, ?)
     * INSERT INTO acarts_bproducts (products_ID, ACart_ID) VALUES (?, ?)
     * INSERT INTO acarts_bproducts (products_ID, ACart_ID) VALUES (?, ?)
     * INSERT INTO acarts_bproducts (products_ID, ACart_ID) VALUES (?, ?)
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
     * INSERT INTO acarts (ID, NAME) VALUES (?, ?)
     * INSERT INTO bproducts (ID, NAME) VALUES (?, ?)
     * INSERT INTO acarts_bproducts (products_ID, ACart_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * DELETE FROM bproducts WHERE (ID = ?)
     * Internal Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Referential integrity constraint violation: "FK_ACARTS_BPRODUCTS_PRODUCTS_ID: PUBLIC.ACARTS_BPRODUCTS FOREIGN KEY(PRODUCTS_ID) REFERENCES PUBLIC.BPRODUCTS(ID) (2)"; SQL statement:
     * DELETE FROM bproducts WHERE (ID = ?) [23503-199]
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * DELETE FROM acarts_bproducts WHERE ((products_ID = ?) AND (ACart_ID = ?))
     * DELETE FROM bproducts WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * DELETE FROM acarts_bproducts WHERE (ACart_ID = ?)
     * DELETE FROM acarts WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * INSERT INTO acarts (ID, NAME) VALUES (?, ?)
     * INSERT INTO bproducts (ID, NAME) VALUES (?, ?)
     * INSERT INTO acarts_bproducts (products_ID, ACart_ID) VALUES (?, ?)
     * INSERT INTO acarts_bproducts (products_ID, ACart_ID) VALUES (?, ?)
     * Internal Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Unique index or primary key violation: "PUBLIC.PRIMARY_KEY_29 ON PUBLIC.ACARTS_BPRODUCTS(ACART_ID, PRODUCTS_ID) VALUES 7"; SQL statement:
     * INSERT INTO acarts_bproducts (products_ID, ACart_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testAddAWithMultipleSameBs() {
        t.testAddAWithMultipleSameBs();
    }
}
