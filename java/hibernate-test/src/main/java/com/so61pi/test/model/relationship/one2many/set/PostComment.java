package com.so61pi.test.model.relationship.one2many.set;

import lombok.Data;

import javax.persistence.*;

@Entity(name = PostComment.TABLE_NAME)
@Table(name = PostComment.TABLE_NAME)
@Data
public class PostComment {
    public final static String TABLE_NAME = "relationship_one2many_set__comments";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @Column(nullable = false)
    private String content;
}
