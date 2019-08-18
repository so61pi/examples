package com.so61pi.test.model.relationship.many2many.listlist;

import lombok.*;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

@Entity
@Table(name = BTag.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class BTag {
    public final static String TABLE_NAME = "btags";

    @Id
    @GeneratedValue
    private Long id;

    // JPA: For many-to-many bidirectional relationships either side may be the owning side.
    @ManyToMany(mappedBy = AEmployee.COL_TAGS)
    private List<AEmployee> employees = new ArrayList<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public BTag(String name) {
        this.name = name;
    }
}
