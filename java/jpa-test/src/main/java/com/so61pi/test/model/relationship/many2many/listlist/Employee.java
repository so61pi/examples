package com.so61pi.test.model.relationship.many2many.listlist;

import lombok.Data;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

/**
 * ManyToMany with List -> ManyToMany with List.
 */

@Entity
@Table(name = Employee.TABLE_NAME)
@Data
public class Employee {
    public final static String TABLE_NAME = "employees";
    public final static String COL_TAGS = "tags";

    @Id
    @GeneratedValue
    private Long id;

    @ManyToMany
    private List<Tag> tags = new ArrayList<Tag>();
}
