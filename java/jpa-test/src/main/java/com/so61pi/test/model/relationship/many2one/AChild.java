package com.so61pi.test.model.relationship.many2one;

import lombok.*;

import javax.persistence.*;

/**
 * ManyToOne.
 */

@Entity
@Table(name = AChild.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class AChild {
    public final static String TABLE_NAME = "achildren";

    @Id
    @GeneratedValue
    private Long id;

    @ManyToOne
    private BMother mother;

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public AChild(String name) {
        this.name = name;
    }
}
