package com.so61pi.test.model.relationship.many2many.list;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * create table aauthors (id bigint not null, name varchar(255) not null, primary key (id))
 * create table aauthors_bbooks (AAuthor_id bigint not null, books_id bigint not null)
 * create table bbooks (id bigint not null, name varchar(255) not null, primary key (id))
 * alter table aauthors_bbooks add constraint FK7cgi1wrr83dh5ueh1lev6bii7 foreign key (books_id) references bbooks
 * alter table aauthors_bbooks add constraint FK8x6r80kton4eewc2qrsidu2dn foreign key (AAuthor_id) references aauthors
 * </pre>
 */
class TestClassHibernate {
    private TestClass t = new TestClass("test-persistence-unit-hibernate");

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into aauthors (name, id) values (?, ?)
     * insert into aauthors (name, id) values (?, ?)
     * insert into aauthors (name, id) values (?, ?)
     * insert into bbooks (name, id) values (?, ?)
     * insert into bbooks (name, id) values (?, ?)
     * insert into bbooks (name, id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
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
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into bbooks (name, id) values (?, ?)
     * insert into aauthors (name, id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * delete from bbooks where id=?
     * ERROR: Referential integrity constraint violation: "FK7CGI1WRR83DH5UEH1LEV6BII7: PUBLIC.AAUTHORS_BBOOKS FOREIGN KEY(BOOKS_ID) REFERENCES PUBLIC.BBOOKS(ID) (4)"; SQL statement:
     * delete from bbooks where id=? [23503-199]
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * delete from aauthors_bbooks where AAuthor_id=?
     * delete from aauthors where id=?
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * delete from aauthors_bbooks where AAuthor_id=?
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * delete from aauthors_bbooks where AAuthor_id=?
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * delete from aauthors_bbooks where AAuthor_id=?
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * delete from bbooks where id=?
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into bbooks (name, id) values (?, ?)
     * insert into aauthors (name, id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * insert into aauthors_bbooks (AAuthor_id, books_id) values (?, ?)
     * </pre>
     */
    @Test
    void testAddAWithMultipleSameBs() {
        t.testAddAWithMultipleSameBs();
    }
}
