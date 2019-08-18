package com.so61pi.test.model.relationship.one2many.set;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

/**
 * OneToMany with Set.
 */

@Entity
@Table(name = ACity.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class ACity {
    public final static String TABLE_NAME = "acities";

    @Id
    @GeneratedValue
    private Long id;

    @OneToMany
    private Set<BApartment> apartments = new HashSet<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public ACity(String name) {
        this.name = name;
    }
}
