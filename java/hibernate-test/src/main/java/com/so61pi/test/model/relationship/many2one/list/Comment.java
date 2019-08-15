package com.so61pi.test.model.relationship.many2one.list;

import lombok.Data;

import javax.persistence.*;

/**
 * ManyToOne -> OneToMany with List.
 */

@Entity
@Table(name = Comment.TABLE_NAME)
@Data
public class Comment {
    public final static String TABLE_NAME = "comments";
    public final static String COL_POST = "post";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @ManyToOne
    // @ManyToOne doesn't have mappedBy.
    private Post post;
}
