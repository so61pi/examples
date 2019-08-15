package com.so61pi.test.model.relationship.many2many.listlist;

import lombok.Data;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

@Entity
@Table(name = Tag.TABLE_NAME)
@Data
public class Tag {
    public final static String TABLE_NAME = "tags";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @ManyToMany(mappedBy = Employee.COL_TAGS)
    private List<Employee> employees = new ArrayList<Employee>();
}
