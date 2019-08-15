package com.so61pi.test.model.relationship.many2many.setlist;

import lombok.Data;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

/**
 * ManyToMany with Set -> ManyToMany with List.
 */

@Entity
@Table(name = Artist.TABLE_NAME)
@Data
public class Artist {
    public final static String TABLE_NAME = "artists";
    public final static String COL_SONGS = "songs";

    @Id
    @GeneratedValue
    private Long id;

    @ManyToMany
    private Set<Song> songs = new HashSet<Song>();
}
