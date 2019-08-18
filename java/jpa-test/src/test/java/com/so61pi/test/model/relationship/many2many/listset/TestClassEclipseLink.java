package com.so61pi.test.model.relationship.many2many.listset;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * <pre>
 * CREATE TABLE astudents (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE bschools (ID BIGINT NOT NULL, NAME VARCHAR NOT NULL, PRIMARY KEY (ID))
 * CREATE TABLE astudents_bschools (students_ID BIGINT NOT NULL, schools_ID BIGINT NOT NULL, PRIMARY KEY (students_ID, schools_ID))
 * ALTER TABLE astudents_bschools ADD CONSTRAINT FK_astudents_bschools_schools_ID FOREIGN KEY (schools_ID) REFERENCES bschools (ID)
 * ALTER TABLE astudents_bschools ADD CONSTRAINT FK_astudents_bschools_students_ID FOREIGN KEY (students_ID) REFERENCES astudents (ID)
 * </pre>
 */
class TestClassEclipseLink {
    private TestClass t = new TestClass("test-persistence-unit-eclipselink");

    /**
     * <pre>
     * UPDATE SEQUENCE SET SEQ_COUNT = SEQ_COUNT + ? WHERE SEQ_NAME = ?
     * SELECT SEQ_COUNT FROM SEQUENCE WHERE SEQ_NAME = ?
     * INSERT INTO astudents (ID, NAME) VALUES (?, ?)
     * INSERT INTO astudents (ID, NAME) VALUES (?, ?)
     * INSERT INTO astudents (ID, NAME) VALUES (?, ?)
     * INSERT INTO bschools (ID, NAME) VALUES (?, ?)
     * INSERT INTO bschools (ID, NAME) VALUES (?, ?)
     * INSERT INTO bschools (ID, NAME) VALUES (?, ?)
     * INSERT INTO astudents_bschools (schools_ID, students_ID) VALUES (?, ?)
     * INSERT INTO astudents_bschools (schools_ID, students_ID) VALUES (?, ?)
     * INSERT INTO astudents_bschools (schools_ID, students_ID) VALUES (?, ?)
     * INSERT INTO astudents_bschools (schools_ID, students_ID) VALUES (?, ?)
     * INSERT INTO astudents_bschools (schools_ID, students_ID) VALUES (?, ?)
     * INSERT INTO astudents_bschools (schools_ID, students_ID) VALUES (?, ?)
     * INSERT INTO astudents_bschools (schools_ID, students_ID) VALUES (?, ?)
     * INSERT INTO astudents_bschools (schools_ID, students_ID) VALUES (?, ?)
     * INSERT INTO astudents_bschools (schools_ID, students_ID) VALUES (?, ?)
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
     * INSERT INTO astudents (ID, NAME) VALUES (?, ?)
     * INSERT INTO bschools (ID, NAME) VALUES (?, ?)
     * INSERT INTO astudents_bschools (schools_ID, students_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromStudentSide() {
        t.testMakeRelationshipFromStudentSide();
    }

    /**
     * <pre>
     * INSERT INTO astudents (ID, NAME) VALUES (?, ?)
     * INSERT INTO bschools (ID, NAME) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testMakeRelationshipFromSchoolSide() {
        t.testMakeRelationshipFromSchoolSide();
    }

    /**
     * <pre>
     * DELETE FROM astudents_bschools WHERE (schools_ID = ?)
     * DELETE FROM bschools WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveSchoolFromSchoolSide() {
        t.testRemoveSchoolFromSchoolSide();
    }

    /**
     * <pre>
     * DELETE FROM astudents_bschools WHERE (schools_ID = ?)
     * DELETE FROM astudents_bschools WHERE ((schools_ID = ?) AND (students_ID = ?))
     * DELETE FROM astudents_bschools WHERE ((schools_ID = ?) AND (students_ID = ?))
     * DELETE FROM astudents_bschools WHERE ((schools_ID = ?) AND (students_ID = ?))
     * DELETE FROM bschools WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveSchoolFromStudentSide() {
        t.testRemoveSchoolFromStudentSide();
    }

    /**
     * <pre>
     * DELETE FROM astudents_bschools WHERE (students_ID = ?)
     * DELETE FROM astudents WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveStudentFromSchoolSide() {
        t.testRemoveStudentFromSchoolSide();
    }

    /**
     * <pre>
     * DELETE FROM astudents_bschools WHERE (students_ID = ?)
     * DELETE FROM astudents WHERE (ID = ?)
     * </pre>
     */
    @Test
    void testRemoveStudentFromStudentSide() {
        t.testRemoveStudentFromStudentSide();
    }

    /**
     * <pre>
     * INSERT INTO astudents (ID, NAME) VALUES (?, ?)
     * INSERT INTO bschools (ID, NAME) VALUES (?, ?)
     * INSERT INTO astudents_bschools (schools_ID, students_ID) VALUES (?, ?)
     * INSERT INTO astudents_bschools (schools_ID, students_ID) VALUES (?, ?)
     * Internal Exception: org.h2.jdbc.JdbcSQLIntegrityConstraintViolationException: Unique index or primary key violation: "PUBLIC.PRIMARY_KEY_39 ON PUBLIC.ASTUDENTS_BSCHOOLS(STUDENTS_ID, SCHOOLS_ID) VALUES 10"; SQL statement:
     * INSERT INTO astudents_bschools (schools_ID, students_ID) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testAddStudentWithMultipleSameSchools() {
        t.testAddStudentWithMultipleSameSchools();
    }

    /**
     * <pre>
     * INSERT INTO astudents (ID, NAME) VALUES (?, ?)
     * INSERT INTO bschools (ID, NAME) VALUES (?, ?)
     * </pre>
     */
    @Test
    void testAddSchoolWithMultipleSameStudents() {
        t.testAddSchoolWithMultipleSameStudents();
    }
}
