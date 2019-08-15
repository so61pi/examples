package com.so61pi.test.model.relationship.one2many.set;

import lombok.Data;

import javax.persistence.*;

@Entity
@Table(name = Apartment.TABLE_NAME)
@Data
public class Apartment {
    public final static String TABLE_NAME = "apartments";

    @Id
    @GeneratedValue
    private Long id;
}
