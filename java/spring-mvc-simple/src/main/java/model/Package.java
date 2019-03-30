package model;

import java.io.Serializable;

public class Package implements Serializable {
    public Product product;
    public int quality;

    public Package(Product product, int quality) {
        this.product = product;
        this.quality = quality;
    }

    @Override
    public String toString() {
        return product.toString();
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof Package)
            return product.equals(((Package) o).product);
        else
            return false;
    }
}
