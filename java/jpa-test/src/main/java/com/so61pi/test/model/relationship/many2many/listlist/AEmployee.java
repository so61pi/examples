package com.so61pi.test.model.relationship.many2many.listlist;

import lombok.*;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

/**
 * ManyToMany with List -> ManyToMany with List.
 */

@Entity
@Table(name = AEmployee.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class AEmployee {
    public final static String TABLE_NAME = "aemployees";
    public final static String COL_TAGS = "tags";

    @Id
    @GeneratedValue
    private Long id;

    // JPA: For many-to-many bidirectional relationships either side may be the owning side.
    @ManyToMany
    private List<BTag> tags = new ArrayList<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public AEmployee(String name) {
        this.name = name;
    }
}
