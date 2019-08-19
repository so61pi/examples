package com.so61pi.test.model.relationship.many2one.list;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.ArrayList;
import java.util.List;

@Getter
class TestData {
    private BPost b;
    private List<AComment> a = new ArrayList<>();

    TestData() {
        // Create data.
        b = new BPost("post-0");
        a.add(new AComment("comment-0"));
        a.add(new AComment("comment-1"));
        a.add(new AComment("comment-2"));

        // Link data together.
        a.forEach(comment -> comment.setPost(b));
        a.forEach(comment -> b.getComments().add(comment));
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
        em.persist(testData.getB());
        testData.getA().forEach(em::persist);
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
        final BPost post = new BPost("post-10");
        em.persist(post);

        final AComment comment = new AComment("comment-10");
        em.persist(comment);

        comment.setPost(post);
        log.info("testMakeRelationshipFromASide ends");
    }

    void testMakeRelationshipFromBSide() {
        log.info("testMakeRelationshipFromBSide begins");
        final BPost post = new BPost("post-20");
        em.persist(post);

        final AComment comment = new AComment("comment-20");
        em.persist(comment);

        post.getComments().add(comment);
        log.info("testMakeRelationshipFromBSide ends");
    }

    void testRemoveB() {
        log.info("testRemoveB begins");
        final BPost post = testData.getB();
        em.remove(post);
        log.info("testRemoveB ends");
    }

    void testRemoveBFromASide() {
        log.info("testRemoveBFromASide begins");
        final BPost post = testData.getB();

        // A removes B (break A-B relationship).
        testData.getA().forEach(comment -> comment.setPost(null));

        // Now remove B.
        em.remove(post);
        log.info("testRemoveBFromASide ends");
    }

    void testRemoveA() {
        log.info("testRemoveA begins");
        final AComment comment = testData.getA().get(0);
        em.remove(comment);
        log.info("testRemoveA ends");
    }

    void testRemoveAFromBSide() {
        log.info("testRemoveAFromBSide begins");
        final AComment comment = testData.getA().get(0);

        // B removes A (break A-B relationship).
        testData.getB().getComments().remove(comment);

        // Now remove A.
        em.remove(comment);
        log.info("testRemoveAFromBSide ends");
    }

    void testAddBWithMultipleSameAs() {
        log.info("testAddBWithMultipleSameAs begins");
        final AComment comment = new AComment("comment-40");
        em.persist(comment);

        final BPost post = new BPost("post-40");
        post.getComments().add(comment);
        post.getComments().add(comment);
        em.persist(post);
        log.info("testAddBWithMultipleSameAs ends");
    }
}
