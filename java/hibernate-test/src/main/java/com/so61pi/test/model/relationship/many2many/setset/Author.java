package com.so61pi.test.model.relationship.many2many.setset;

import lombok.Data;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

/**
 * ManyToMany with Set -> ManyToMany with Set.
 */

@Entity(name = Author.TABLE_NAME)
@Table(name = Author.TABLE_NAME)
@Data
public class Author {
    public final static String TABLE_NAME = "relationship_many2many_setset__authors";
    public final static String COL_BOOKS = "books";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @ManyToMany
    private Set<Book> books = new HashSet<Book>();
}
