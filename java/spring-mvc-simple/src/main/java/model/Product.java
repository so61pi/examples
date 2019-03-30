package model;

import java.io.Serializable;
import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;

import org.hibernate.Session;

import util.HibernateUtil;

import com.fasterxml.jackson.annotation.JsonIgnore;

@Entity
public class Product implements Serializable {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "ID")
    private int id;

    @Column(name = "NAME")
    private String name;

    @Column(name = "DESCRIPTION")
    private String description;

    @Column(name = "PRICE")
    private double price;

    @Column(name = "QUALITY")
    private int quality;

    //
    //
    //
    @Override
    public String toString() {
        return id + "";
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof Product)
            return id == ((Product) o).id;
        else
            return false;
    }

    public void cloneFrom(Product product) {
        name = product.name;
        description = product.description;
        price = product.price;
        quality = product.quality;
    }

    @JsonIgnore
    public boolean isValid() {
        return (name != null && !name.equals(""))
                && (description != null && !description.equals(""))
                && (price >= 0) && (quality >= 0);
    }

    //
    //
    //
    public static List<Product> list() {
        Session session = HibernateUtil.open();
        List<Product> r = session.createCriteria(Product.class).list();
        session.close();
        return r;

    }

    public static void add(Product product) {
        Session session = HibernateUtil.open();
        Product p = (Product) HibernateUtil.get(session, Product.class,
                product.id);
        if (p == null) {
            HibernateUtil.save(session, product);
        }
        else {
            p.cloneFrom(product);
            HibernateUtil.update(session, p);
        }
        session.close();
    }

    public static void remove(Product product) {
        Session session = HibernateUtil.open();
        HibernateUtil.delete(session,
                HibernateUtil.get(session, Product.class, product.id));
        session.close();
    }

    public static Product getProductById(int id) {
        Session session = HibernateUtil.open();
        Product r = (Product) HibernateUtil.get(session, Product.class, id);
        session.close();
        return r;
    }

    //
    //
    //
    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public int getQuality() {
        return quality;
    }

    public void setQuality(int quality) {
        this.quality = quality;
    }
}
