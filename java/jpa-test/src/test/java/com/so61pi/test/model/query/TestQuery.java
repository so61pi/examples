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

    @Test
    void test() {
        CriteriaBuilder cb = em.getCriteriaBuilder();

        CriteriaQuery<AProject> criteriaQuery = cb.createQuery(AProject.class);
        Root<AProject> r = criteriaQuery.from(AProject.class);

        CriteriaQuery<AProject> select = criteriaQuery.select(r);
        TypedQuery<AProject> query = em.createQuery(select);
        query.getResultList();
    }
}
