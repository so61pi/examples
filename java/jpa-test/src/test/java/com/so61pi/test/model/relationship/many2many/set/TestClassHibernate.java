package com.so61pi.test.model.relationship.many2many.set;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * create table aalbums (id bigint not null, name varchar(255) not null, primary key (id))
 * create table aalbums_blabels (AAlbum_id bigint not null, labels_id bigint not null, primary key (AAlbum_id, labels_id))
 * create table blabels (id bigint not null, name varchar(255) not null, primary key (id))
 * alter table aalbums_blabels add constraint FKpo1eude1nuti22oi346rfvpb7 foreign key (labels_id) references blabels
 * alter table aalbums_blabels add constraint FK47exogw2a62nvd0a3hhkmfoht foreign key (AAlbum_id) references aalbums
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
     * insert into aalbums (name, id) values (?, ?)
     * insert into aalbums (name, id) values (?, ?)
     * insert into aalbums (name, id) values (?, ?)
     * insert into blabels (name, id) values (?, ?)
     * insert into blabels (name, id) values (?, ?)
     * insert into blabels (name, id) values (?, ?)
     * insert into aalbums_blabels (AAlbum_id, labels_id) values (?, ?)
     * insert into aalbums_blabels (AAlbum_id, labels_id) values (?, ?)
     * insert into aalbums_blabels (AAlbum_id, labels_id) values (?, ?)
     * insert into aalbums_blabels (AAlbum_id, labels_id) values (?, ?)
     * insert into aalbums_blabels (AAlbum_id, labels_id) values (?, ?)
     * insert into aalbums_blabels (AAlbum_id, labels_id) values (?, ?)
     * insert into aalbums_blabels (AAlbum_id, labels_id) values (?, ?)
     * insert into aalbums_blabels (AAlbum_id, labels_id) values (?, ?)
     * insert into aalbums_blabels (AAlbum_id, labels_id) values (?, ?)
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
     * insert into blabels (name, id) values (?, ?)
     * insert into aalbums (name, id) values (?, ?)
     * insert into aalbums_blabels (AAlbum_id, labels_id) values (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromAlbumSide() {
        t.testMakeRelationshipFromAlbumSide();
    }

    /**
     * <pre>
     * delete from blabels where id=?
     * ERROR: Referential integrity constraint violation: "FKPO1EUDE1NUTI22OI346RFVPB7: PUBLIC.AALBUMS_BLABELS FOREIGN KEY(LABELS_ID) REFERENCES PUBLIC.BLABELS(ID) (4)"; SQL statement:
     * delete from blabels where id=? [23503-199]
     * </pre>
     */
    @Test
    void testRemoveLabelFromLabelSide() {
        t.testRemoveLabelFromLabelSide();
    }

    /**
     * <pre>
     * delete from aalbums_blabels where AAlbum_id=? and labels_id=?
     * delete from aalbums_blabels where AAlbum_id=? and labels_id=?
     * delete from aalbums_blabels where AAlbum_id=? and labels_id=?
     * delete from blabels where id=?
     * </pre>
     */
    @Test
    void testRemoveLabelFromAlbumSide() {
        t.testRemoveLabelFromAlbumSide();
    }

    /**
     * <pre>
     * delete from aalbums_blabels where AAlbum_id=?
     * delete from aalbums where id=?
     * </pre>
     */
    @Test
    void testRemoveAlbum() {
        t.testRemoveAlbum();
    }

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into blabels (name, id) values (?, ?)
     * insert into aalbums (name, id) values (?, ?)
     * insert into aalbums_blabels (AAlbum_id, labels_id) values (?, ?)
     * </pre>
     */
    @Test
    void testAddAlbumWithMultipleSameLabels() {
        t.testAddAlbumWithMultipleSameLabels();
    }
}
