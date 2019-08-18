package com.so61pi.test.model.relationship.many2many.setset;

import lombok.*;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

@Entity
@Table(name = BLanguage.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class BLanguage {
    public final static String TABLE_NAME = "blanguages";

    @Id
    @GeneratedValue
    private Long id;

    // JPA: For many-to-many bidirectional relationships either side may be the owning side.
    @ManyToMany(mappedBy = AProject.COL_LANGUAGES)
    private Set<AProject> projects = new HashSet<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public BLanguage(String name) {
        this.name = name;
    }
}
