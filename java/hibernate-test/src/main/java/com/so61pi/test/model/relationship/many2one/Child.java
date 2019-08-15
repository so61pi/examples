package com.so61pi.test.model.relationship.many2one;

import lombok.Data;

import javax.persistence.*;

/**
 * ManyToOne.
 */

@Entity
@Table(name = Child.TABLE_NAME)
@Data
public class Child {
    public final static String TABLE_NAME = "children";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @ManyToOne
    private Mother mother;
}
