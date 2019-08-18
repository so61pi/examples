package com.so61pi.test.model.relationship.many2one.set;

import lombok.*;

import javax.persistence.*;

/**
 * ManyToOne -> OneToMany with Set.
 */

@Entity
@Table(name = AMember.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class AMember {
    public final static String TABLE_NAME = "amembers";
    public final static String COL_FAMILY = "family";

    @Id
    @GeneratedValue
    private Long id;

    @ManyToOne
    // @ManyToOne doesn't have mappedBy.
    private BFamily family;

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public AMember(String name) {
        this.name = name;
    }
}
