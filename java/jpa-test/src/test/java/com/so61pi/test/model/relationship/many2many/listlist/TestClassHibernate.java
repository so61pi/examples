package com.so61pi.test.model.relationship.many2many.listlist;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * create table aemployees (id bigint not null, name varchar(255) not null, primary key (id))
 * create table aemployees_btags (employees_id bigint not null, tags_id bigint not null)
 * create table btags (id bigint not null, name varchar(255) not null, primary key (id))
 * alter table aemployees_btags add constraint FKmkgmboycjwserlpkqxtnvns9d foreign key (tags_id) references btags
 * alter table aemployees_btags add constraint FK8tdrqgmv8m639u6x5235qeicy foreign key (employees_id) references aemployees
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
     * insert into aemployees (name, id) values (?, ?)
     * insert into aemployees (name, id) values (?, ?)
     * insert into aemployees (name, id) values (?, ?)
     * insert into btags (name, id) values (?, ?)
     * insert into btags (name, id) values (?, ?)
     * insert into btags (name, id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
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
     * insert into aemployees (name, id) values (?, ?)
     * insert into btags (name, id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into aemployees (name, id) values (?, ?)
     * insert into btags (name, id) values (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromBSide() {
        t.testMakeRelationshipFromBSide();
    }

    /**
     * delete from btags where id=?
     * ERROR: Referential integrity constraint violation: "FKMKGMBOYCJWSERLPKQXTNVNS9D: PUBLIC.AEMPLOYEES_BTAGS FOREIGN KEY(TAGS_ID) REFERENCES PUBLIC.BTAGS(ID) (4)"; SQL statement:
     * delete from btags where id=?
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * delete from aemployees_btags where employees_id=?
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * delete from aemployees_btags where employees_id=?
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * delete from aemployees_btags where employees_id=?
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * delete from btags where id=?
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * delete from aemployees_btags where employees_id=?
     * delete from aemployees where id=?
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * delete from aemployees_btags where employees_id=?
     * delete from aemployees where id=?
     * </pre>
     */
    @Test
    void testRemoveAFromBSide() {
        t.testRemoveAFromBSide();
    }

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into btags (name, id) values (?, ?)
     * insert into aemployees (name, id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * insert into aemployees_btags (employees_id, tags_id) values (?, ?)
     * </pre>
     */
    @Test
    void testAddAWithMultipleSameBs() {
        t.testAddAWithMultipleSameBs();
    }

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into aemployees (name, id) values (?, ?)
     * insert into btags (name, id) values (?, ?)
     * </pre>
     */
    @Test
    void testAddBWithMultipleSameAs() {
        t.testAddBWithMultipleSameAs();
    }
}
