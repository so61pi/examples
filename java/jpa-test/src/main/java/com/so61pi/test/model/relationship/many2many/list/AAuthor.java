package com.so61pi.test.model.relationship.many2many.list;

import lombok.*;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

/**
 * ManyToMany with List.
 */

@Entity
@Table(name = AAuthor.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class AAuthor {
    public final static String TABLE_NAME = "aauthors";

    @Id
    @GeneratedValue
    private Long id;

    @ManyToMany
    private List<BBook> books = new ArrayList<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public AAuthor(String name) {
        this.name = name;
    }
}
