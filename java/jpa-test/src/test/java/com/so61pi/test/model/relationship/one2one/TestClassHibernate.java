package com.so61pi.test.model.relationship.one2one;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * create table aemails (id bigint not null, name varchar(255) not null, account_id bigint, primary key (id))
 * create table baccounts (id bigint not null, name varchar(255) not null, primary key (id))
 * alter table aemails add constraint FK75rfcmwuy5ringknfob5t86od foreign key (account_id) references baccounts
 * </pre>
 */
class TestClassHibernate {
    private TestClass t = new TestClass("test-persistence-unit-hibernate");

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into aemails (account_id, name, id) values (?, ?, ?)
     * insert into baccounts (name, id) values (?, ?)
     * update aemails set account_id=?, name=? where id=?
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
     * insert into baccounts (name, id) values (?, ?)
     * insert into aemails (account_id, name, id) values (?, ?, ?)
     * update aemails set account_id=?, name=? where id=?
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * delete from baccounts where id=?
     * ERROR: Referential integrity constraint violation: "FK75RFCMWUY5RINGKNFOB5T86OD: PUBLIC.AEMAILS FOREIGN KEY(ACCOUNT_ID) REFERENCES PUBLIC.BACCOUNTS(ID) (2)"; SQL statement:
     * delete from baccounts where id=? [23503-199]
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * update aemails set account_id=?, name=? where id=?
     * delete from baccounts where id=?
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * delete from aemails where id=?
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }
}
