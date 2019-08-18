package com.so61pi.test.model.relationship.many2many.setlist;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * create table aartists (id bigint not null, name varchar(255) not null, primary key (id))
 * create table aartists_bsongs (artists_id bigint not null, songs_id bigint not null, primary key (artists_id, songs_id))
 * create table bsongs (id bigint not null, name varchar(255) not null, primary key (id))
 * alter table aartists_bsongs add constraint FKc9fqk415ty11u1gtf7p6gkp0t foreign key (songs_id) references bsongs
 * alter table aartists_bsongs add constraint FKip7gl1qyxtti0gauvr9mwbxgn foreign key (artists_id) references aartists
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
     * insert into aartists (name, id) values (?, ?)
     * insert into aartists (name, id) values (?, ?)
     * insert into aartists (name, id) values (?, ?)
     * insert into bsongs (name, id) values (?, ?)
     * insert into bsongs (name, id) values (?, ?)
     * insert into bsongs (name, id) values (?, ?)
     * insert into aartists_bsongs (artists_id, songs_id) values (?, ?)
     * insert into aartists_bsongs (artists_id, songs_id) values (?, ?)
     * insert into aartists_bsongs (artists_id, songs_id) values (?, ?)
     * insert into aartists_bsongs (artists_id, songs_id) values (?, ?)
     * insert into aartists_bsongs (artists_id, songs_id) values (?, ?)
     * insert into aartists_bsongs (artists_id, songs_id) values (?, ?)
     * insert into aartists_bsongs (artists_id, songs_id) values (?, ?)
     * insert into aartists_bsongs (artists_id, songs_id) values (?, ?)
     * insert into aartists_bsongs (artists_id, songs_id) values (?, ?)
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
     * insert into aartists (name, id) values (?, ?)
     * insert into bsongs (name, id) values (?, ?)
     * insert into aartists_bsongs (artists_id, songs_id) values (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromArtistSide() {
        t.testMakeRelationshipFromArtistSide();
    }

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into aartists (name, id) values (?, ?)
     * insert into bsongs (name, id) values (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromSongSide() {
        t.testMakeRelationshipFromSongSide();
    }

    /**
     * <pre>
     * delete from bsongs where id=?
     * ERROR: Referential integrity constraint violation: "FKC9FQK415TY11U1GTF7P6GKP0T: PUBLIC.AARTISTS_BSONGS FOREIGN KEY(SONGS_ID) REFERENCES PUBLIC.BSONGS(ID) (4)"; SQL statement:
     * delete from bsongs where id=? [23503-199]
     * </pre>
     */
    @Test
    void testRemoveSongFromSongSide() {
        t.testRemoveSongFromSongSide();
    }

    /**
     * <pre>
     * delete from aartists_bsongs where artists_id=? and songs_id=?
     * delete from aartists_bsongs where artists_id=? and songs_id=?
     * delete from aartists_bsongs where artists_id=? and songs_id=?
     * delete from bsongs where id=?
     * </pre>
     */
    @Test
    void testRemoveSongFromArtistSide() {
        t.testRemoveSongFromArtistSide();
    }

    /**
     * <pre>
     * delete from aartists_bsongs where artists_id=?
     * delete from aartists where id=?
     * </pre>
     */
    @Test
    void testRemoveArtistFromSongSide() {
        t.testRemoveArtistFromSongSide();
    }

    /**
     * <pre>
     * delete from aartists_bsongs where artists_id=?
     * delete from aartists where id=?
     * </pre>
     */
    @Test
    void testRemoveArtistFromArtistSide() {
        t.testRemoveArtistFromArtistSide();
    }

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into bsongs (name, id) values (?, ?)
     * insert into aartists (name, id) values (?, ?)
     * insert into aartists_bsongs (artists_id, songs_id) values (?, ?)
     * </pre>
     */
    @Test
    void testAddArtistWithMultipleSameSongs() {
        t.testAddArtistWithMultipleSameSongs();
    }

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into aartists (name, id) values (?, ?)
     * insert into bsongs (name, id) values (?, ?)
     * </pre>
     */
    @Test
    void testAddSongWithMultipleSameArtists() {
        t.testAddSongWithMultipleSameArtists();
    }
}
