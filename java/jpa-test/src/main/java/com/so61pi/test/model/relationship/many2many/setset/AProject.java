package com.so61pi.test.model.relationship.many2many.setset;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

/**
 * ManyToMany with Set -> ManyToMany with Set.
 */

@Entity
@Table(name = AProject.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class AProject {
    public final static String TABLE_NAME = "aprojects";
    public final static String COL_LANGUAGES = "languages";

    @Id
    @GeneratedValue
    private Long id;

    // JPA: For many-to-many bidirectional relationships either side may be the owning side.
    @ManyToMany
    private Set<BLanguage> languages = new HashSet<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public AProject(String name) {
        this.name = name;
    }
}
