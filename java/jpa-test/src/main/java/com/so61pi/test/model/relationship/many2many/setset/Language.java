package com.so61pi.test.model.relationship.many2many.setset;

import lombok.Data;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

@Entity
@Table(name = Language.TABLE_NAME)
@Data
public class Language {
    public final static String TABLE_NAME = "languages";

    @Id
    @GeneratedValue
    private Long id;

    @ManyToMany(mappedBy = Project.COL_LANGUAGES)
    private Set<Project> projects = new HashSet<Project>();
}
