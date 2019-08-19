package com.so61pi.test.model.relationship.one2many.list;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * create table acarts (id bigint not null, name varchar(255) not null, primary key (id))
 * create table acarts_bproducts (ACart_id bigint not null, products_id bigint not null)
 * create table bproducts (id bigint not null, name varchar(255) not null, primary key (id))
 * alter table acarts_bproducts add constraint UK_p2vrtp7x9f6nqelxe2y0mh0ug unique (products_id)
 * alter table acarts_bproducts add constraint FK6ky8o5mma801q7nkwt9xc8cc3 foreign key (products_id) references bproducts
 * alter table acarts_bproducts add constraint FK16jlqy5fb27uwy9pcqca3gits foreign key (ACart_id) references acarts
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
     * insert into acarts (name, id) values (?, ?)
     * insert into bproducts (name, id) values (?, ?)
     * insert into bproducts (name, id) values (?, ?)
     * insert into bproducts (name, id) values (?, ?)
     * insert into acarts_bproducts (ACart_id, products_id) values (?, ?)
     * insert into acarts_bproducts (ACart_id, products_id) values (?, ?)
     * insert into acarts_bproducts (ACart_id, products_id) values (?, ?)
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
     * insert into bproducts (name, id) values (?, ?)
     * insert into acarts (name, id) values (?, ?)
     * insert into acarts_bproducts (ACart_id, products_id) values (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromASide() {
        t.testMakeRelationshipFromASide();
    }

    /**
     * <pre>
     * delete from bproducts where id=?
     * ERROR: Referential integrity constraint violation: "FK6KY8O5MMA801Q7NKWT9XC8CC3: PUBLIC.ACARTS_BPRODUCTS FOREIGN KEY(PRODUCTS_ID) REFERENCES PUBLIC.BPRODUCTS(ID) (2)"; SQL statement:
     * delete from bproducts where id=? [23503-199]
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * delete from acarts_bproducts where ACart_id=?
     * insert into acarts_bproducts (ACart_id, products_id) values (?, ?)
     * insert into acarts_bproducts (ACart_id, products_id) values (?, ?)
     * delete from bproducts where id=?
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * delete from acarts_bproducts where ACart_id=?
     * delete from acarts where id=?
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
     * insert into bproducts (name, id) values (?, ?)
     * insert into acarts (name, id) values (?, ?)
     * insert into acarts_bproducts (ACart_id, products_id) values (?, ?)
     * insert into acarts_bproducts (ACart_id, products_id) values (?, ?)
     * ERROR: Unique index or primary key violation: "PUBLIC.UK_P2VRTP7X9F6NQELXE2Y0MH0UG_INDEX_2 ON PUBLIC.ACARTS_BPRODUCTS(PRODUCTS_ID) VALUES 4"; SQL statement:
     * insert into acarts_bproducts (ACart_id, products_id) values (?, ?)
     * </pre>
     */
    @Test
    void testAddAWithMultipleSameBs() {
        t.testAddAWithMultipleSameBs();
    }
}
