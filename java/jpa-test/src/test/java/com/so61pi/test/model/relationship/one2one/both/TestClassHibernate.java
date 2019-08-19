package com.so61pi.test.model.relationship.one2one.both;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * create table aworkers (id bigint not null, name varchar(255) not null, address_id bigint, primary key (id))
 * create table baddresses (id bigint not null, name varchar(255) not null, primary key (id))
 * alter table aworkers add constraint FK6m6f9olh77jqyg26jy842w5lh foreign key (address_id) references baddresses
 * </pre>
 */
class TestClassHibernate {
    private TestClass t = new TestClass("test-persistence-unit-hibernate");

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into aworkers (address_id, name, id) values (?, ?, ?)
     * insert into baddresses (name, id) values (?, ?)
     * update aworkers set address_id=?, name=? where id=?
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
     * insert into baddresses (name, id) values (?, ?)
     * insert into aworkers (address_id, name, id) values (?, ?, ?)
     * update aworkers set address_id=?, name=? where id=?
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
     * insert into baddresses (name, id) values (?, ?)
     * insert into aworkers (address_id, name, id) values (?, ?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromBSide() {
        t.testMakeRelationshipFromBSide();
    }

    /**
     * <pre>
     * delete from baddresses where id=?
     * ERROR: Referential integrity constraint violation: "FK6M6F9OLH77JQYG26JY842W5LH: PUBLIC.AWORKERS FOREIGN KEY(ADDRESS_ID) REFERENCES PUBLIC.BADDRESSES(ID) (2)"; SQL statement:
     * delete from baddresses where id=?
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * update aworkers set address_id=?, name=? where id=?
     * delete from baddresses where id=?
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * delete from aworkers where id=?
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * delete from aworkers where id=?
     * </pre>
     */
    @Test
    void testRemoveAFromBSide() {
        t.testRemoveAFromBSide();
    }
}
