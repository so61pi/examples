package com.so61pi.test.model.relationship.many2many.setlist;

import lombok.*;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

/**
 * ManyToMany with Set -> ManyToMany with List.
 */

@Entity
@Table(name = AArtist.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)public class AArtist {
    public final static String TABLE_NAME = "aartists";
    public final static String COL_SONGS = "songs";

    @Id
    @GeneratedValue
    private Long id;

    // JPA: For many-to-many bidirectional relationships either side may be the owning side.
    @ManyToMany
    private Set<BSong> songs = new HashSet<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public AArtist(String name) {
        this.name = name;
    }
}
