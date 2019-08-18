package com.so61pi.test.model.relationship.many2many.listset;

import lombok.*;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

@Entity
@Table(name = BSchool.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)public class BSchool {
    public final static String TABLE_NAME = "bschools";

    @Id
    @GeneratedValue
    private Long id;

    // JPA: For many-to-many bidirectional relationships either side may be the owning side.
    @ManyToMany(mappedBy = AStudent.COL_BOOKS)
    private Set<AStudent> students = new HashSet<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public BSchool(String name) {
        this.name = name;
    }
}
