package com.so61pi.test.model.relationship.one2many.list;

import lombok.Data;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

/**
 * OneToMany with List.
 */

@Entity
@Table(name = Cart.TABLE_NAME)
@Data
public class Cart {
    public final static String TABLE_NAME = "carts";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @OneToMany
    private List<Product> products = new ArrayList<Product>();
}
