package com.so61pi.test.model.relationship.many2one.list;

import lombok.Data;

import javax.persistence.*;

/**
 * ManyToOne -> OneToMany with List.
 */

@Entity(name = PostComment.TABLE_NAME)
@Table(name = PostComment.TABLE_NAME)
@Data
public class PostComment {
    public final static String TABLE_NAME = "relationship_many2one_list__comments";
    public final static String COL_POST = "post";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @Column(nullable = false)
    private String content;

    @ManyToOne
    // @ManyToOne doesn't have mappedBy.
    private Post post;
}
