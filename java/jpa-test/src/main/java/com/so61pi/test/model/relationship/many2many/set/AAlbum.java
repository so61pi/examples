package com.so61pi.test.model.relationship.many2many.set;

import lombok.*;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

/**
 * ManyToMany with Set.
 */

@Entity
@Table(name = AAlbum.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)public class AAlbum {
    public final static String TABLE_NAME = "aalbums";

    @Id
    @GeneratedValue
    private Long id;

    @ManyToMany
    private Set<BLabel> labels = new HashSet<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public AAlbum(String name) {
        this.name = name;
    }
}
