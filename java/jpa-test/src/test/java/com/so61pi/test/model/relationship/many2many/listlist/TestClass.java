package com.so61pi.test.model.relationship.many2many.listlist;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.ArrayList;
import java.util.List;

@Getter
class TestData {
    private List<BTag> b = new ArrayList<>();
    private List<AEmployee> a = new ArrayList<>();

    TestData() {
        // Create data.
        b.add(new BTag("tag-0"));
        b.add(new BTag("tag-1"));
        b.add(new BTag("tag-2"));
        a.add(new AEmployee("employee-0"));
        a.add(new AEmployee("employee-1"));
        a.add(new AEmployee("employee-2"));

        // Link data together.
        a.forEach(employee -> b.forEach(tag -> employee.getTags().add(tag)));
        b.forEach(tag -> a.forEach(employee -> tag.getEmployees().add(employee)));
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

    void testMakeRelationshipFromEmployeeSide() {
        log.info("testMakeRelationshipFromEmployeeSide begins");
        final AEmployee employee = new AEmployee("employee-10");
        em.persist(employee);

        final BTag tag = new BTag("tag-10");
        em.persist(tag);

        employee.getTags().add(tag);
        log.info("testMakeRelationshipFromEmployeeSide ends");
    }

    void testMakeRelationshipFromTagSide() {
        log.info("testMakeRelationshipFromTagSide begins");
        final AEmployee employee = new AEmployee("employee-20");
        em.persist(employee);

        final BTag tag = new BTag("tag-20");
        em.persist(tag);

        tag.getEmployees().add(employee);
        log.info("testMakeRelationshipFromTagSide ends");
    }

    void testRemoveTagFromTagSide() {
        log.info("testRemoveTagFromTagSide begins");
        final BTag tag = testData.getB().get(0);
        em.remove(tag);
        log.info("testRemoveTagFromTagSide ends");
    }

    void testRemoveTagFromEmployeeSide() {
        log.info("testRemoveTagFromEmployeeSide begins");
        final BTag tag = testData.getB().get(0);

        // Break employee-tag relationships for all employees.
        testData.getA().forEach(employee -> employee.getTags().remove(tag));

        // Now remove tag.
        em.remove(tag);
        log.info("testRemoveTagFromEmployeeSide ends");
    }

    void testRemoveEmployeeFromTagSide() {
        log.info("testRemoveEmployeeFromTagSide begins");
        final AEmployee employee = testData.getA().get(0);

        // Break tag-employee relationships for all tags.
        testData.getB().forEach(tag -> tag.getEmployees().remove(employee));

        // Now remove employee.
        em.remove(employee);
        log.info("testRemoveEmployeeFromTagSide ends");
    }

    void testRemoveEmployeeFromEmployeeSide() {
        log.info("testRemoveEmployeeFromEmployeeSide begins");
        final AEmployee employee = testData.getA().get(0);
        em.remove(employee);
        log.info("testRemoveEmployeeFromEmployeeSide ends");
    }

    void testAddEmployeeWithMultipleSameTags() {
        log.info("testAddEmployeeWithMultipleSameTags begins");
        final BTag tag = new BTag("tag-30");
        em.persist(tag);

        final AEmployee employee = new AEmployee("employee-30");
        employee.getTags().add(tag);
        employee.getTags().add(tag);
        em.persist(employee);
        log.info("testAddEmployeeWithMultipleSameTags ends");
    }

    void testAddTagWithMultipleSameEmployees() {
        log.info("testAddTagWithMultipleSameEmployees begins");
        final AEmployee employee = new AEmployee("employee-40");
        em.persist(employee);

        final BTag tag = new BTag("tag-40");
        tag.getEmployees().add(employee);
        tag.getEmployees().add(employee);
        em.persist(tag);
        log.info("testAddTagWithMultipleSameEmployees ends");
    }
}
