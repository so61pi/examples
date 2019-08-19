package com.so61pi.test.model.relationship.many2one.list;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * create table acomments (id bigint not null, name varchar(255) not null, post_id bigint, primary key (id))
 * create table bposts (id bigint not null, name varchar(255) not null, primary key (id))
 * alter table acomments add constraint FKgdrcpaqnssthpkdwrnmkvv8lb foreign key (post_id) references bposts
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
     * insert into bposts (name, id) values (?, ?)
     * insert into acomments (name, post_id, id) values (?, ?, ?)
     * insert into acomments (name, post_id, id) values (?, ?, ?)
     * insert into acomments (name, post_id, id) values (?, ?, ?)
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
     * insert into bposts (name, id) values (?, ?)
     * insert into acomments (name, post_id, id) values (?, ?, ?)
     * update acomments set name=?, post_id=? where id=?
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
     * insert into bposts (name, id) values (?, ?)
     * insert into acomments (name, post_id, id) values (?, ?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromBSide() {
        t.testMakeRelationshipFromBSide();
    }

    /**
     * <pre>
     * delete from bposts where id=?
     * ERROR: Referential integrity constraint violation: "FKGDRCPAQNSSTHPKDWRNMKVV8LB: PUBLIC.ACOMMENTS FOREIGN KEY(POST_ID) REFERENCES PUBLIC.BPOSTS(ID) (1)"; SQL statement:
     * delete from bposts where id=?
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * update acomments set name=?, post_id=? where id=?
     * update acomments set name=?, post_id=? where id=?
     * update acomments set name=?, post_id=? where id=?
     * delete from bposts where id=?
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * delete from acomments where id=?
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * delete from acomments where id=?
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
     * insert into acomments (name, post_id, id) values (?, ?, ?)
     * insert into bposts (name, id) values (?, ?)
     * </pre>
     */
    @Test
    void testAddBWithMultipleSameAs() {
        t.testAddBWithMultipleSameAs();
    }
}
