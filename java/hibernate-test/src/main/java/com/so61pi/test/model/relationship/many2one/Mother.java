package com.so61pi.test.model.relationship.many2one;

import lombok.Data;

import javax.persistence.*;

@Entity
@Table(name = Mother.TABLE_NAME)
@Data
public class Mother {
    public final static String TABLE_NAME = "mothers";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;
}
