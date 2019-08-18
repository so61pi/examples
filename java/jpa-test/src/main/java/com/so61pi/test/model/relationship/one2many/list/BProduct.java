package com.so61pi.test.model.relationship.one2many.list;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;

@Entity
@Table(name = BProduct.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class BProduct {
    public final static String TABLE_NAME = "bproducts";

    @Id
    @GeneratedValue
    private Long id;

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public BProduct(String name) {
        this.name = name;
    }
}
