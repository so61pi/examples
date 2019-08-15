package com.so61pi.test.model.relationship.many2many.listset;

import lombok.Data;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

@Entity
@Table(name = School.TABLE_NAME)
@Data
public class School {
    public final static String TABLE_NAME = "schools";

    @Id
    @GeneratedValue
    private Long id;

    @ManyToMany(mappedBy = Student.COL_BOOKS)
    private Set<Student> students = new HashSet<Student>();
}
