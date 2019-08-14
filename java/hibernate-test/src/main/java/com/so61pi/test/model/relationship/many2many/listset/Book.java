package com.so61pi.test.model.relationship.many2many.listset;

import lombok.Data;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

@Entity(name = Book.TABLE_NAME)
@Table(name = Book.TABLE_NAME)
@Data
public class Book {
    public final static String TABLE_NAME = "relationship_many2many_listset__books";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @ManyToMany(mappedBy = Author.COL_BOOKS)
    private Set<Author> authors = new HashSet<Author>();
}
