package com.so61pi.test.model.relationship.many2many.setset;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
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

    void testMakeRelationshipFromProjectSide() {
        log.info("testMakeRelationshipFromProjectSide begins");
        final AProject project = new AProject("project-10");
        em.persist(project);

        final BLanguage language = new BLanguage("language-10");
        em.persist(language);

        project.getLanguages().add(language);
        log.info("testMakeRelationshipFromProjectSide ends");
    }

    void testMakeRelationshipFromLanguageSide() {
        log.info("testMakeRelationshipFromLanguageSide begins");
        final AProject project = new AProject("project-20");
        em.persist(project);

        final BLanguage language = new BLanguage("language-20");
        em.persist(language);

        language.getProjects().add(project);
        log.info("testMakeRelationshipFromLanguageSide ends");
    }

    void testRemoveLanguageFromLanguageSide() {
        log.info("testRemoveLanguageFromLanguageSide begins");
        final BLanguage language = testData.getB().get(0);
        em.remove(language);
        log.info("testRemoveLanguageFromLanguageSide ends");
    }

    void testRemoveLanguageFromProjectSide() {
        log.info("testRemoveLanguageFromProjectSide begins");
        final BLanguage language = testData.getB().get(0);

        // Break project-language relationships for all projects.
        testData.getA().forEach(project -> project.getLanguages().remove(language));

        // Now remove language.
        em.remove(language);
        log.info("testRemoveLanguageFromProjectSide ends");
    }

    void testRemoveProjectFromLanguageSide() {
        log.info("testRemoveProjectFromLanguageSide begins");
        final AProject project = testData.getA().get(0);

        // Break language-project relationships for all languages.
        testData.getB().forEach(language -> language.getProjects().remove(project));

        // Now remove project.
        em.remove(project);
        log.info("testRemoveProjectFromLanguageSide ends");
    }

    void testRemoveProjectFromProjectSide() {
        log.info("testRemoveProjectFromProjectSide begins");
        final AProject project = testData.getA().get(0);
        em.remove(project);
        log.info("testRemoveProjectFromProjectSide ends");
    }

    void testAddProjectWithMultipleSameLanguages() {
        log.info("testAddProjectWithMultipleSameLanguages begins");
        final BLanguage language = new BLanguage("language-30");
        em.persist(language);

        final AProject project = new AProject("project-30");
        project.getLanguages().add(language);
        project.getLanguages().add(language);
        em.persist(project);
        log.info("testAddProjectWithMultipleSameLanguages ends");
    }

    void testAddLanguageWithMultipleSameProjects() {
        log.info("testAddLanguageWithMultipleSameProjects begins");
        final AProject project = new AProject("project-40");
        em.persist(project);

        final BLanguage language = new BLanguage("language-40");
        language.getProjects().add(project);
        language.getProjects().add(project);
        em.persist(language);
        log.info("testAddLanguageWithMultipleSameProjects ends");
    }
}
