package com.so61pi.test.model.relationship.one2many.set;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;

@Entity
@Table(name = BApartment.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class BApartment {
    public final static String TABLE_NAME = "bapartments";

    @Id
    @GeneratedValue
    private Long id;

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public BApartment(String name) {
        this.name = name;
    }
}
