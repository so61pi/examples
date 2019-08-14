package com.so61pi.test.model.relationship.many2many.list;

import lombok.Data;

import javax.persistence.*;

@Entity(name = Book.TABLE_NAME)
@Table(name = Book.TABLE_NAME)
@Data
public class Book {
    public final static String TABLE_NAME = "relationship_many2many_list__books";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;
}
