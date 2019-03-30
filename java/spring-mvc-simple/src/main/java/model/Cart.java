package model;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.hibernate.Session;

import util.HibernateUtil;

public class Cart implements Serializable {
    private List<Package> packages = new ArrayList<Package>();

    //
    //
    //
    public void addProduct(Product product, int quality) {
        product = Product.getProductById(product.getId());
        Package pack = new Package(product, quality);
        int pos = packages.indexOf(pack);
        if (pos == -1) {
            packages.add(pack);
        }
        else {
            pack = packages.get(pos);
            pack.quality += quality;
        }
    }

    public void removeProduct(Product product) {
        product = Product.getProductById(product.getId());
        Package pack = new Package(product, 0);
        packages.remove(pack);
    }

    public Map<String, String> checkOut() {
        Map<String, String> errors = new HashMap<String, String>();

        Session session = HibernateUtil.open();
        for (Package pack : packages) {
            Product product = (Product) HibernateUtil.get(session,
                    Product.class, pack.product.getId());
            if (product.getQuality() >= pack.quality) {
                product.setQuality(product.getQuality() - pack.quality);
                pack.quality = 0;
            }
            else {
                errors.put(product.getName(), "Out of stock");
            }
            HibernateUtil.update(session, product);
        }

        List<Package> newPackages = new ArrayList<Package>();
        for (Package pack : packages) {
            if (pack.quality > 0) {
                newPackages.add(pack);
            }
        }
        packages = newPackages;

        session.close();

        return errors;
    }

    //
    //
    //
    public List<Package> getPackages() {
        return packages;
    }

    public void setPackages(List<Package> packages) {
        this.packages = packages;
    }
}
