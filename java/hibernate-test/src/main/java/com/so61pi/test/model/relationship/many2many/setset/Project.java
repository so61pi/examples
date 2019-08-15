package com.so61pi.test.model.relationship.many2many.setset;

import lombok.Data;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

/**
 * ManyToMany with Set -> ManyToMany with Set.
 */

@Entity
@Table(name = Project.TABLE_NAME)
@Data
public class Project {
    public final static String TABLE_NAME = "projects";
    public final static String COL_LANGUAGES = "languages";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @ManyToMany
    private Set<Language> languages = new HashSet<Language>();
}
