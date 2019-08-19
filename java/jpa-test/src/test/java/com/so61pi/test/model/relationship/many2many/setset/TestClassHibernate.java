package com.so61pi.test.model.relationship.many2many.setset;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * create table aprojects (id bigint not null, name varchar(255) not null, primary key (id))
 * create table aprojects_blanguages (projects_id bigint not null, languages_id bigint not null, primary key (projects_id, languages_id))
 * create table blanguages (id bigint not null, name varchar(255) not null, primary key (id))
 * alter table aprojects_blanguages add constraint FKk4yew6losr3arry7ts0g6kcla foreign key (languages_id) references blanguages
 * alter table aprojects_blanguages add constraint FKgpsrx12rsi7ed9nfsfjnywe16 foreign key (projects_id) references aprojects
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
     * insert into aprojects (name, id) values (?, ?)
     * insert into aprojects (name, id) values (?, ?)
     * insert into aprojects (name, id) values (?, ?)
     * insert into blanguages (name, id) values (?, ?)
     * insert into blanguages (name, id) values (?, ?)
     * insert into blanguages (name, id) values (?, ?)
     * insert into aprojects_blanguages (projects_id, languages_id) values (?, ?)
     * insert into aprojects_blanguages (projects_id, languages_id) values (?, ?)
     * insert into aprojects_blanguages (projects_id, languages_id) values (?, ?)
     * insert into aprojects_blanguages (projects_id, languages_id) values (?, ?)
     * insert into aprojects_blanguages (projects_id, languages_id) values (?, ?)
     * insert into aprojects_blanguages (projects_id, languages_id) values (?, ?)
     * insert into aprojects_blanguages (projects_id, languages_id) values (?, ?)
     * insert into aprojects_blanguages (projects_id, languages_id) values (?, ?)
     * insert into aprojects_blanguages (projects_id, languages_id) values (?, ?)
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
     * insert into aprojects (name, id) values (?, ?)
     * insert into blanguages (name, id) values (?, ?)
     * insert into aprojects_blanguages (projects_id, languages_id) values (?, ?)
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
     * insert into aprojects (name, id) values (?, ?)
     * insert into blanguages (name, id) values (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromBSide() {
        t.testMakeRelationshipFromBSide();
    }

    /**
     * <pre>
     * delete from blanguages where id=?
     * ERROR: Referential integrity constraint violation: "FKK4YEW6LOSR3ARRY7TS0G6KCLA: PUBLIC.APROJECTS_BLANGUAGES FOREIGN KEY(LANGUAGES_ID) REFERENCES PUBLIC.BLANGUAGES(ID) (4)"; SQL statement:
     * delete from blanguages where id=? [23503-199]
     * </pre>
     */
    @Test
    void testRemoveB() {
        t.testRemoveB();
    }

    /**
     * <pre>
     * delete from aprojects_blanguages where projects_id=? and languages_id=?
     * delete from aprojects_blanguages where projects_id=? and languages_id=?
     * delete from aprojects_blanguages where projects_id=? and languages_id=?
     * delete from blanguages where id=?
     * </pre>
     */
    @Test
    void testRemoveBFromASide() {
        t.testRemoveBFromASide();
    }

    /**
     * <pre>
     * delete from aprojects_blanguages where projects_id=?
     * delete from aprojects where id=?
     * </pre>
     */
    @Test
    void testRemoveA() {
        t.testRemoveA();
    }

    /**
     * <pre>
     * delete from aprojects_blanguages where projects_id=?
     * delete from aprojects where id=?
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
     * insert into blanguages (name, id) values (?, ?)
     * insert into aprojects (name, id) values (?, ?)
     * insert into aprojects_blanguages (projects_id, languages_id) values (?, ?)
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
     * insert into aprojects (name, id) values (?, ?)
     * insert into blanguages (name, id) values (?, ?)
     * </pre>
     */
    @Test
    void testAddBWithMultipleSameAs() {
        t.testAddBWithMultipleSameAs();
    }
}
