package core;

import java.io.Serializable;
import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;

import org.hibernate.Session;

import utils.HibernateUtil;

@Entity
public class Product implements Serializable {
    private static final long serialVersionUID = -4018744124930051753L;

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "PRODUCT_ID")
    private int productId;

    @Column(name = "NAME")
    private String name;

    @Column(name = "DESCRIPTION")
    private String description;

    @Column(name = "QUALITY")
    private int quality;

    @Column(name = "PRICE")
    private double price;

    //
    public static List<Product> getProductList() {
        return HibernateUtil.findComplex(Product.class, null);
    }

    public static void addProduct(String name, String description, int quality,
            double price) {
        try {
            Product p = new Product();
            p.name = name;
            p.description = description;
            p.quality = quality;
            p.price = price;

            Session session = HibernateUtil.getSessionFactory().openSession();
            HibernateUtil.save(session, p);
            session.close();
        }
        catch (Exception e) {
        }
    }

    public static void updateProduct(int productId, String name,
            String description, int quality, double price) {
        try {
            Session session = HibernateUtil.getSessionFactory().openSession();

            Product p = (Product) HibernateUtil.get(session, Product.class,
                    productId);

            p.name = name;
            p.description = description;
            p.quality = quality;
            p.price = price;
            HibernateUtil.update(session, p);

            session.close();
        }
        catch (Exception e) {
        }
    }

    public static void removeProduct(int productId) {
        try {
            Session session = HibernateUtil.getSessionFactory().openSession();
            Product p = (Product) HibernateUtil.get(session, Product.class,
                    productId);
            HibernateUtil.delete(session, p);
            session.close();
        }
        catch (Exception e) {
        }
    }

    //
    //
    //
    public int getProductId() {
        return productId;
    }

    public void setProductId(int productId) {
        this.productId = productId;
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

    public int getQuality() {
        return quality;
    }

    public void setQuality(int quality) {
        if (quality < 0)
            quality = 0;
        this.quality = quality;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        if (price < 0)
            price = 0;
        this.price = price;
    }

}
