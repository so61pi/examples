package com.so61pi.test.model.relationship.many2many.set;

import lombok.Data;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

/**
 * ManyToMany with Set.
 */

@Entity
@Table(name = Album.TABLE_NAME)
@Data
public class Album {
    public final static String TABLE_NAME = "albums";

    @Id
    @GeneratedValue
    private Long id;

    @ManyToMany
    private Set<Label> labels = new HashSet<Label>();
}
