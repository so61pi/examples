package com.so61pi.test.model.relationship.one2many.set;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * create table acities (id bigint not null, name varchar(255) not null, primary key (id))
 * create table acities_bapartments (ACity_id bigint not null, apartments_id bigint not null, primary key (ACity_id, apartments_id))
 * create table bapartments (id bigint not null, name varchar(255) not null, primary key (id))
 * alter table acities_bapartments add constraint UK_lwrhqjksa9p2a4e2dcuy1mlle unique (apartments_id)
 * alter table acities_bapartments add constraint FKkoe77funff59055qar6lyx3xx foreign key (apartments_id) references bapartments
 * alter table acities_bapartments add constraint FKdtfs6xwg9hbm1bd102y6xx804 foreign key (ACity_id) references acities
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
     * insert into acities (name, id) values (?, ?)
     * insert into bapartments (name, id) values (?, ?)
     * insert into bapartments (name, id) values (?, ?)
     * insert into bapartments (name, id) values (?, ?)
     * insert into acities_bapartments (ACity_id, apartments_id) values (?, ?)
     * insert into acities_bapartments (ACity_id, apartments_id) values (?, ?)
     * insert into acities_bapartments (ACity_id, apartments_id) values (?, ?)
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
     * insert into bapartments (name, id) values (?, ?)
     * insert into acities (name, id) values (?, ?)
     * insert into acities_bapartments (ACity_id, apartments_id) values (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * delete from bapartments where id=?
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * delete from acities_bapartments where ACity_id=? and apartments_id=?
     * delete from bapartments where id=?
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * delete from acities_bapartments where ACity_id=?
     * delete from acities where id=?
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into bapartments (name, id) values (?, ?)
     * insert into acities (name, id) values (?, ?)
     * insert into acities_bapartments (ACity_id, apartments_id) values (?, ?)
     * </pre>
     */
    @Test
    void testAddAWithMultipleSameBs() {
        t.testAddAWithMultipleSameBs();
    }
}
