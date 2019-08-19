package com.so61pi.test.model.relationship.many2one;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * create table achildren (id bigint not null, name varchar(255) not null, mother_id bigint, primary key (id))
 * create table bmothers (id bigint not null, name varchar(255) not null, primary key (id))
 * alter table achildren add constraint FKkctavj0klal2dch0ysyso0fxl foreign key (mother_id) references bmothers
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
     * insert into bmothers (name, id) values (?, ?)
     * insert into achildren (mother_id, name, id) values (?, ?, ?)
     * insert into achildren (mother_id, name, id) values (?, ?, ?)
     * insert into achildren (mother_id, name, id) values (?, ?, ?)
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
     * insert into bmothers (name, id) values (?, ?)
     * insert into achildren (mother_id, name, id) values (?, ?, ?)
     * update achildren set mother_id=?, name=? where id=?
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * delete from bmothers where id=?
     * ERROR: Referential integrity constraint violation: "FKKCTAVJ0KLAL2DCH0YSYSO0FXL: PUBLIC.ACHILDREN FOREIGN KEY(MOTHER_ID) REFERENCES PUBLIC.BMOTHERS(ID) (1)"; SQL statement:
     * delete from bmothers where id=? [23503-199]
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * update achildren set mother_id=?, name=? where id=?
     * update achildren set mother_id=?, name=? where id=?
     * update achildren set mother_id=?, name=? where id=?
     * delete from bmothers where id=?
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * delete from achildren where id=?
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }
}
