package com.so61pi.test.model.relationship.many2many.list;

import lombok.Data;

import javax.persistence.*;

@Entity
@Table(name = Book.TABLE_NAME)
@Data
public class Book {
    public final static String TABLE_NAME = "books";

    @Id
    @GeneratedValue
    private Long id;
}
