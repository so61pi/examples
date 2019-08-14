package com.so61pi.test.model.relationship.many2many.listset;

import lombok.Data;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

/**
 * ManyToMany with List -> ManyToMany with Set.
 */

@Entity(name = Author.TABLE_NAME)
@Table(name = Author.TABLE_NAME)
@Data
public class Author {
    public final static String TABLE_NAME = "relationship_many2many_listset__authors";
    public final static String COL_BOOKS = "books";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @ManyToMany
    private List<Book> books = new ArrayList<Book>();
}
