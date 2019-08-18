package com.so61pi.test.model.relationship.many2many.listset;

import lombok.*;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

/**
 * ManyToMany with List -> ManyToMany with Set.
 */

@Entity
@Table(name = AStudent.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class AStudent {
    public final static String TABLE_NAME = "astudents";
    public final static String COL_BOOKS = "schools";

    @Id
    @GeneratedValue
    private Long id;

    // JPA: For many-to-many bidirectional relationships either side may be the owning side.
    @ManyToMany
    private List<BSchool> schools = new ArrayList<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public AStudent(String name) {
        this.name = name;
    }
}
