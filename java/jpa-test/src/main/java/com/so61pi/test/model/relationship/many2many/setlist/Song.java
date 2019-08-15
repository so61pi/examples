package com.so61pi.test.model.relationship.many2many.setlist;

import lombok.Data;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

@Entity
@Table(name = Song.TABLE_NAME)
@Data
public class Song {
    public final static String TABLE_NAME = "songs";

    @Id
    @GeneratedValue
    private Long id;

    @ManyToMany(mappedBy = Artist.COL_SONGS)
    private List<Artist> artists = new ArrayList<Artist>();
}
