package com.so61pi.test.model.relationship.many2many.listset;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * create table astudents (id bigint not null, name varchar(255) not null, primary key (id))
 * create table astudents_bschools (students_id bigint not null, schools_id bigint not null)
 * create table bschools (id bigint not null, name varchar(255) not null, primary key (id))
 * alter table astudents_bschools add constraint FKev22q2plbwc5hkshpedmib7vw foreign key (schools_id) references bschools
 * alter table astudents_bschools add constraint FKsp6mwddrdy3r4oya4waxk68wx foreign key (students_id) references astudents
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
     * insert into astudents (name, id) values (?, ?)
     * insert into astudents (name, id) values (?, ?)
     * insert into astudents (name, id) values (?, ?)
     * insert into bschools (name, id) values (?, ?)
     * insert into bschools (name, id) values (?, ?)
     * insert into bschools (name, id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
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
     * insert into astudents (name, id) values (?, ?)
     * insert into bschools (name, id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromStudentSide() {
        t.testMakeRelationshipFromStudentSide();
    }

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into astudents (name, id) values (?, ?)
     * insert into bschools (name, id) values (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromSchoolSide() {
        t.testMakeRelationshipFromSchoolSide();
    }

    /**
     * <pre>
     * delete from bschools where id=?
     ERROR: Referential integrity constraint violation: "FKEV22Q2PLBWC5HKSHPEDMIB7VW: PUBLIC.ASTUDENTS_BSCHOOLS FOREIGN KEY(SCHOOLS_ID) REFERENCES PUBLIC.BSCHOOLS(ID) (4)"; SQL statement:
     delete from bschools where id=? [23503-199]
     * </pre>
     */
    @Test
    void testRemoveSchoolFromSchoolSide() {
        t.testRemoveSchoolFromSchoolSide();
    }

    /**
     * <pre>
     * delete from astudents_bschools where students_id=?
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * delete from astudents_bschools where students_id=?
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * delete from astudents_bschools where students_id=?
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * delete from bschools where id=?
     * </pre>
     */
    @Test
    void testRemoveSchoolFromStudentSide() {
        t.testRemoveSchoolFromStudentSide();
    }

    /**
     * <pre>
     * delete from astudents_bschools where students_id=?
     * delete from astudents where id=?
     * </pre>
     */
    @Test
    void testRemoveStudentFromSchoolSide() {
        t.testRemoveStudentFromSchoolSide();
    }

    /**
     * <pre>
     * delete from astudents_bschools where students_id=?
     * delete from astudents where id=?
     * </pre>
     */
    @Test
    void testRemoveStudentFromStudentSide() {
        t.testRemoveStudentFromStudentSide();
    }

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into bschools (name, id) values (?, ?)
     * insert into astudents (name, id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * insert into astudents_bschools (students_id, schools_id) values (?, ?)
     * </pre>
     */
    @Test
    void testAddStudentWithMultipleSameSchools() {
        t.testAddStudentWithMultipleSameSchools();
    }

    /**
     * <pre>
     * call next value for hibernate_sequence
     * call next value for hibernate_sequence
     * insert into astudents (name, id) values (?, ?)
     * insert into bschools (name, id) values (?, ?)
     * </pre>
     */
    @Test
    void testAddSchoolWithMultipleSameStudents() {
        t.testAddSchoolWithMultipleSameStudents();
    }
}
