package com.so61pi.test.model.relationship.many2many.listset;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.ArrayList;
import java.util.List;

@Getter
class TestData {
    private List<BSchool> b = new ArrayList<>();
    private List<AStudent> a = new ArrayList<>();

    TestData() {
        // Create data.
        b.add(new BSchool("school-0"));
        b.add(new BSchool("school-1"));
        b.add(new BSchool("school-2"));
        a.add(new AStudent("student-0"));
        a.add(new AStudent("student-1"));
        a.add(new AStudent("student-2"));

        // Link data together.
        a.forEach(student -> b.forEach(school -> student.getSchools().add(school)));
        b.forEach(school -> a.forEach(student -> school.getStudents().add(student)));
    }
}

@Slf4j
class TestClass {
    private String persistenceUnit;
    private EntityManagerFactory emf;
    private EntityManager em;
    private TestData testData;

    TestClass(String persistenceUnit) {
        this.persistenceUnit = persistenceUnit;
    }

    void setUp() {
        log.info("setUp begins");
        emf = Persistence.createEntityManagerFactory(persistenceUnit);
        em = emf.createEntityManager();

        testData = new TestData();

        // Put data into database.
        em.getTransaction().begin();
        testData.getA().forEach(em::persist);
        testData.getB().forEach(em::persist);
        em.getTransaction().commit();

        em.getTransaction().begin();
        log.info("setUp ends");
    }

    void tearDown() {
        log.info("tearDown begins");
        em.getTransaction().commit();
        em.close();
        emf.close();
        log.info("tearDown ends");
    }

    void testMakeRelationshipFromASide() {
        log.info("testMakeRelationshipFromASide begins");
        final AStudent student = new AStudent("student-10");
        em.persist(student);

        final BSchool school = new BSchool("school-10");
        em.persist(school);

        student.getSchools().add(school);
        log.info("testMakeRelationshipFromASide ends");
    }

    void testMakeRelationshipFromBSide() {
        log.info("testMakeRelationshipFromBSide begins");
        final AStudent student = new AStudent("student-20");
        em.persist(student);

        final BSchool school = new BSchool("school-20");
        em.persist(school);

        school.getStudents().add(student);
        log.info("testMakeRelationshipFromBSide ends");
    }

    void testRemoveB() {
        log.info("testRemoveB begins");
        final BSchool school = testData.getB().get(0);
        em.remove(school);
        log.info("testRemoveB ends");
    }

    void testRemoveBFromASide() {
        log.info("testRemoveBFromASide begins");
        final BSchool school = testData.getB().get(0);

        // A removes B (break A-B relationship).
        testData.getA().forEach(student -> student.getSchools().remove(school));

        // Now remove B.
        em.remove(school);
        log.info("testRemoveBFromASide ends");
    }

    void testRemoveA() {
        log.info("testRemoveA begins");
        final AStudent student = testData.getA().get(0);
        em.remove(student);
        log.info("testRemoveA ends");
    }

    void testRemoveAFromBSide() {
        log.info("testRemoveAFromBSide begins");
        final AStudent student = testData.getA().get(0);

        // B removes A (break A-B relationship).
        testData.getB().forEach(school -> school.getStudents().remove(student));

        // Now remove A.
        em.remove(student);
        log.info("testRemoveAFromBSide ends");
    }

    void testAddAWithMultipleSameBs() {
        log.info("testAddAWithMultipleSameBs begins");
        final BSchool school = new BSchool("school-30");
        em.persist(school);

        final AStudent student = new AStudent("student-30");
        student.getSchools().add(school);
        student.getSchools().add(school);
        em.persist(student);
        log.info("testAddAWithMultipleSameBs ends");
    }

    void testAddBWithMultipleSameAs() {
        log.info("testAddBWithMultipleSameAs begins");
        final AStudent student = new AStudent("student-40");
        em.persist(student);

        final BSchool school = new BSchool("school-40");
        school.getStudents().add(student);
        school.getStudents().add(student);
        em.persist(school);
        log.info("testAddBWithMultipleSameAs ends");
    }
}
