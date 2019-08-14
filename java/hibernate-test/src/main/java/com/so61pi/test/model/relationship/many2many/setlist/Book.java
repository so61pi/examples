package com.so61pi.test.model.relationship.many2many.setlist;

import lombok.Data;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

@Entity(name = Book.TABLE_NAME)
@Table(name = Book.TABLE_NAME)
@Data
public class Book {
    public final static String TABLE_NAME = "relationship_many2many_setlist__books";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @ManyToMany(mappedBy = Author.COL_BOOKS)
    private List<Author> authors = new ArrayList<Author>();
}
