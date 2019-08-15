package com.so61pi.test.model.relationship.one2many.list;

import lombok.Data;

import javax.persistence.*;

@Entity
@Table(name = Product.TABLE_NAME)
@Data
public class Product {
    public final static String TABLE_NAME = "products";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;
}
