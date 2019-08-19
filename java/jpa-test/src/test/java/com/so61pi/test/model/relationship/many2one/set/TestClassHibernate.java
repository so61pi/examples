package com.so61pi.test.model.relationship.many2one.set;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * create table amembers (id bigint not null, name varchar(255) not null, family_id bigint, primary key (id))
 * create table bfamilies (id bigint not null, name varchar(255) not null, primary key (id))
 * alter table amembers add constraint FKp0rn0l2svrkfkl7ntsd3da7lf foreign key (family_id) references bfamilies
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
     * insert into bfamilies (name, id) values (?, ?)
     * insert into amembers (family_id, name, id) values (?, ?, ?)
     * insert into amembers (family_id, name, id) values (?, ?, ?)
     * insert into amembers (family_id, name, id) values (?, ?, ?)
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
     * insert into bfamilies (name, id) values (?, ?)
     * insert into amembers (family_id, name, id) values (?, ?, ?)
     * update amembers set family_id=?, name=? where id=?
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
     * insert into bfamilies (name, id) values (?, ?)
     * insert into amembers (family_id, name, id) values (?, ?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromBSide() {
        t.testMakeRelationshipFromBSide();
    }

    /**
     * <pre>
     * delete from bfamilies where id=?
     * ERROR: Referential integrity constraint violation: "FKP0RN0L2SVRKFKL7NTSD3DA7LF: PUBLIC.AMEMBERS FOREIGN KEY(FAMILY_ID) REFERENCES PUBLIC.BFAMILIES(ID) (1)"; SQL statement:
     * delete from bfamilies where id=? [23503-199]
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * update amembers set family_id=?, name=? where id=?
     * update amembers set family_id=?, name=? where id=?
     * update amembers set family_id=?, name=? where id=?
     * delete from bfamilies where id=?
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * delete from amembers where id=?
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * delete from amembers where id=?
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
     * insert into amembers (family_id, name, id) values (?, ?, ?)
     * insert into bfamilies (name, id) values (?, ?)
     * </pre>
     */
    @Test
    void testAddBWithMultipleSameAs() {
        t.testAddBWithMultipleSameAs();
    }
}
