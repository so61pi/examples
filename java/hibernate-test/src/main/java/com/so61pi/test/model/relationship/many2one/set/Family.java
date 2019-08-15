package com.so61pi.test.model.relationship.many2one.set;

import lombok.Data;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

@Entity
@Table(name = Family.TABLE_NAME)
@Data
public class Family {
    public final static String TABLE_NAME = "families";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @OneToMany(mappedBy = Member.COL_FAMILY)
    private Set<Member> members = new HashSet<Member>();
}
