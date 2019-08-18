package com.so61pi.test.model.relationship.many2many.list;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * CREATE TABLE bbooks (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE aauthors (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE aauthors_bbooks (AAuthor_ID BIGINT NOT NULL, books_ID BIGINT NOT NULL, PRIMARY KEY (AAuthor_ID, books_ID))
 * ALTER TABLE aauthors_bbooks ADD CONSTRAINT FK_aauthors_bbooks_books_ID FOREIGN KEY (books_ID) REFERENCES bbooks (ID)
 * ALTER TABLE aauthors_bbooks ADD CONSTRAINT FK_aauthors_bbooks_AAuthor_ID FOREIGN KEY (AAuthor_ID) REFERENCES aauthors (ID)
 * </pre>
 */
class TestClassEclipseLink {
    private TestClass t = new TestClass("test-persistence-unit-eclipselink");

    /**
     * <pre>
     * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
     * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
     * INSERT INTO aauthors (ID, NAME) VALUES (?, ?)
     * INSERT INTO aauthors (ID, NAME) VALUES (?, ?)
     * INSERT INTO aauthors (ID, NAME) VALUES (?, ?)
     * INSERT INTO bbooks (ID, NAME) VALUES (?, ?)
     * INSERT INTO bbooks (ID, NAME) VALUES (?, ?)
     * INSERT INTO bbooks (ID, NAME) VALUES (?, ?)
     * INSERT INTO aauthors_bbooks (books_ID, AAuthor_ID) VALUES (?, ?)
     * INSERT INTO aauthors_bbooks (books_ID, AAuthor_ID) VALUES (?, ?)
     * INSERT INTO aauthors_bbooks (books_ID, AAuthor_ID) VALUES (?, ?)
     * INSERT INTO aauthors_bbooks (books_ID, AAuthor_ID) VALUES (?, ?)
     * INSERT INTO aauthors_bbooks (books_ID, AAuthor_ID) VALUES (?, ?)
     * INSERT INTO aauthors_bbooks (books_ID, AAuthor_ID) VALUES (?, ?)
     * INSERT INTO aauthors_bbooks (books_ID, AAuthor_ID) VALUES (?, ?)
     * INSERT INTO aauthors_bbooks (books_ID, AAuthor_ID) VALUES (?, ?)
     * INSERT INTO aauthors_bbooks (books_ID, AAuthor_ID) VALUES (?, ?)
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
     * INSERT INTO aauthors (ID, NAME) VALUES (?, ?)
     * INSERT INTO bbooks (ID, NAME) VALUES (?, ?)
     * INSERT INTO aauthors_bbooks (books_ID, AAuthor_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromAuthorSide() {
        t.testMakeRelationshipFromAuthorSide();
    }

    /**
     * <pre>
     * DELETE FROM bbooks WHERE (ID = ?)
     * Internal Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Referential integrity constraint violation: "FK_AAUTHORS_BBOOKS_BOOKS_ID: PUBLIC.AAUTHORS_BBOOKS FOREIGN KEY(BOOKS_ID) REFERENCES PUBLIC.BBOOKS(ID) (4)"; SQL statement:
     * DELETE FROM bbooks WHERE (ID = ?
     * </pre>
     */
    @Test
    void testRemoveBookFromBookSide() {
        t.testRemoveBookFromBookSide();
    }

    /**
     * <pre>
     * DELETE FROM aauthors_bbooks WHERE ((books_ID = ?) AND (AAuthor_ID = ?))
     * DELETE FROM aauthors_bbooks WHERE ((books_ID = ?) AND (AAuthor_ID = ?))
     * DELETE FROM aauthors_bbooks WHERE ((books_ID = ?) AND (AAuthor_ID = ?))
     * DELETE FROM bbooks WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveBookFromAuthorSide() {
        t.testRemoveBookFromAuthorSide();
    }

    /**
     * <pre>
     * DELETE FROM aauthors_bbooks WHERE (AAuthor_ID = ?)
     * DELETE FROM aauthors WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveAuthor() {
        t.testRemoveAuthor();
    }

    /**
     * <pre>
     * INSERT INTO aauthors (ID, NAME) VALUES (?, ?)
     * INSERT INTO bbooks (ID, NAME) VALUES (?, ?)
     * INSERT INTO aauthors_bbooks (books_ID, AAuthor_ID) VALUES (?, ?)
     * INSERT INTO aauthors_bbooks (books_ID, AAuthor_ID) VALUES (?, ?)
     * Internal Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Unique index or primary key violation: "PUBLIC.PRIMARY_KEY_66 ON PUBLIC.AAUTHORS_BBOOKS(AAUTHOR_ID, BOOKS_ID) VALUES 47"; SQL statement:
     * INSERT INTO aauthors_bbooks (books_ID, AAuthor_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testAddAuthorWithMultipleSameBooks() {
        t.testAddAuthorWithMultipleSameBooks();
    }
}
