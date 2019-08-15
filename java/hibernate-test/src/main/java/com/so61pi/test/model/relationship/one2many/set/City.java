package com.so61pi.test.model.relationship.one2many.set;

import lombok.Data;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

/**
 * OneToMany with Set.
 */

@Entity
@Table(name = City.TABLE_NAME)
@Data
public class City {
    public final static String TABLE_NAME = "cities";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @OneToMany
    private Set<Apartment> apartments = new HashSet<Apartment>();
}
