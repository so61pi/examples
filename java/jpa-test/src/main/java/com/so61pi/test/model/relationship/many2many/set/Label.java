package com.so61pi.test.model.relationship.many2many.set;

import lombok.Data;

import javax.persistence.*;

@Entity
@Table(name = Label.TABLE_NAME)
@Data
public class Label {
    public final static String TABLE_NAME = "labels";

    @Id
    @GeneratedValue
    private Long id;
}
