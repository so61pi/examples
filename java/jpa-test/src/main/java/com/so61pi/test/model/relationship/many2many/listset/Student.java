package com.so61pi.test.model.relationship.many2many.listset;

import lombok.Data;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

/**
 * ManyToMany with List -> ManyToMany with Set.
 */

@Entity
@Table(name = Student.TABLE_NAME)
@Data
public class Student {
    public final static String TABLE_NAME = "students";
    public final static String COL_BOOKS = "schools";

    @Id
    @GeneratedValue
    private Long id;

    @ManyToMany
    private List<School> schools = new ArrayList<School>();
}
