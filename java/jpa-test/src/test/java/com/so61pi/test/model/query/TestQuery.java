package com.so61pi.test.model.query;

import com.so61pi.test.model.relationship.many2many.setset.AProject;
import com.so61pi.test.model.relationship.many2many.setset.BLanguage;
import lombok.Getter;
import lombok.extern.slf4j.Slf4j;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import javax.persistence.TypedQuery;
import javax.persistence.criteria.CriteriaBuilder;
import javax.persistence.criteria.CriteriaQuery;
import javax.persistence.criteria.Root;
import java.util.ArrayList;
import java.util.List;

@Getter
class TestData {
    private List<BLanguage> b = new ArrayList<>();
    private List<AProject> a = new ArrayList<>();

    TestData() {
        // Create data.
        b.add(new BLanguage("language-0"));
        b.add(new BLanguage("language-1"));
        b.add(new BLanguage("language-2"));
        a.add(new AProject("project-0"));
        a.add(new AProject("project-1"));
        a.add(new AProject("project-2"));

        // Link data together.
        a.forEach(project -> b.forEach(language -> project.getLanguages().add(language)));
        b.forEach(language -> a.forEach(project -> language.getProjects().add(project)));
    }
}

@Slf4j
class TestClass {
    private EntityManagerFactory emf;
    private EntityManager em;

    @BeforeEach
    void setUp() {
        log.info("setUp begins");
        emf = Persistence.createEntityManagerFactory("test-persistence-unit-hibernate");
        em = emf.createEntityManager();

        TestData testData = new TestData();

        // Put data into database.
        em.getTransaction().begin();
        testData.getA().forEach(em::persist);
        testData.getB().forEach(em::persist);
        em.getTransaction().commit();

        em.getTransaction().begin();
        log.info("setUp ends");
    }

    @AfterEach
    void tearDown() {
        log.info("tearDown begins");
        em.getTransaction().commit();
        em.close();
        emf.close();
        log.info("tearDown ends");
    }

    /**
     * select aproject0_.id as id1_16_, aproject0_.name as name2_16_ from aprojects aproject0_
     */
    @Test
    void testSelect() {
        // https://docs.oracle.com/javaee/7/tutorial/persistence-criteria001.htm

        // Use an EntityManager instance to create a CriteriaBuilder object.
        CriteriaBuilder cb = em.getCriteriaBuilder();

        // Create a query object by creating an instance of the CriteriaQuery interface.
        // This query object's attributes will be modified with the details of the query.
        // To create a typesafe query, specify the type of the query when you create the CriteriaQuery object.
        CriteriaQuery<AProject> cq = cb.createQuery(AProject.class);

        // Call the from method of the query object to set the FROM clause of the query and to specify the root of the query.
        Root<AProject> project = cq.from(AProject.class);

        // Call the select method of the query object, passing in the query root, to set the SELECT clause of the query.
        CriteriaQuery<AProject> select = cq.select(project);

        // Now, use the query object to create a TypedQuery<T> object that can be executed against the data source.
        // The modifications to the query object are captured to create a ready-to-execute query
        TypedQuery<AProject> query = em.createQuery(select);

        // Execute the query by calling the getResultList method on the TypedQuery<T> object.
        // Because this query returns a collection of entities, the result is stored in a List.
        query.getResultList();
    }
}
