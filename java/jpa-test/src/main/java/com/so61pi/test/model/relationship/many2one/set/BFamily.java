package com.so61pi.test.model.relationship.many2one.set;

import lombok.*;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

@Entity
@Table(name = BFamily.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class BFamily {
    public final static String TABLE_NAME = "bfamilies";

    @Id
    @GeneratedValue
    private Long id;

    @OneToMany(mappedBy = AMember.COL_FAMILY)
    private Set<AMember> members = new HashSet<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public BFamily(String name) {
        this.name = name;
    }
}
