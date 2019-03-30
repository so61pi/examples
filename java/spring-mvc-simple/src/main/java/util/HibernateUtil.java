package util;

import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.HibernateException;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;
import org.hibernate.criterion.Criterion;
import org.hibernate.criterion.Restrictions;
import org.hibernate.service.ServiceRegistry;

public class HibernateUtil {
    private static final SessionFactory sessionFactory;

    static {
        Configuration configuration = new Configuration();
        configuration.configure();
        ServiceRegistry serviceRegistry = new StandardServiceRegistryBuilder()
                .applySettings(configuration.getProperties()).build();
        sessionFactory = configuration.buildSessionFactory(serviceRegistry);
    }

    public static SessionFactory getSessionFactory() {
        return sessionFactory;
    }

    public static Session open() {
        return sessionFactory.openSession();
    }

    public static void save(Session session, Object o) {
        Transaction tx = null;
        try {
            tx = session.beginTransaction();
            session.save(o);
            tx.commit();
        }
        catch (HibernateException e) {
            if (tx != null)
                tx.rollback();
            ExceptionHandler.handle(e);
        }
    }

    public static Object get(Session session, Class oClass, int id) {
        Transaction tx = null;
        Object r = null;
        try {
            tx = session.beginTransaction();
            r = session.get(oClass, id);
            tx.commit();
        }
        catch (HibernateException e) {
            if (tx != null)
                tx.rollback();
            ExceptionHandler.handle(e);
        }

        return r;
    }

    public static void update(Session session, Object o) {
        Transaction tx = null;
        try {
            tx = session.beginTransaction();
            session.update(o);
            tx.commit();
        }
        catch (HibernateException e) {
            if (tx != null)
                tx.rollback();
            ExceptionHandler.handle(e);
        }
    }

    public static void delete(Session session, Object o) {
        Transaction tx = null;
        try {
            tx = session.beginTransaction();
            session.delete(o);
            tx.commit();
        }
        catch (HibernateException e) {
            if (tx != null)
                tx.rollback();
            ExceptionHandler.handle(e);
        }
    }

    public static Object findFirst(Session session, Class oClass, String field,
            String value) {
        List r = find(session, oClass, field, value);
        if (r != null && r.size() != 0)
            return r.get(0);
        else
            return null;
    }

    public static List find(Session session, Class oClass, String field,
            String value) {
        Criterion criteria[] = { Restrictions.like(field, value) };
        return findComplex(session, oClass, criteria);
    }

    public static List findComplex(Session session, Class oClass,
            Criterion[] criteria) {
        Transaction tx = null;
        List r = null;
        try {
            tx = session.beginTransaction();
            Criteria cr = session.createCriteria(oClass);
            if (criteria != null)
                for (int i = 0; i < criteria.length; ++i)
                    cr.add(criteria[i]);
            r = cr.list();
            tx.commit();
        }
        catch (HibernateException e) {
            if (tx != null)
                tx.rollback();
            ExceptionHandler.handle(e);
        }
        return r;
    }

}
