package com.so61pi.test.model.relationship.many2one.set;

import lombok.Data;

import javax.persistence.*;

/**
 * ManyToOne -> OneToMany with Set.
 */

@Entity
@Table(name = Member.TABLE_NAME)
@Data
public class Member {
    public final static String TABLE_NAME = "members";
    public final static String COL_FAMILY = "family";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @ManyToOne
    // @ManyToOne doesn't have mappedBy.
    private Family family;
}
