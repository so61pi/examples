package com.so61pi.test.model.relationship.one2one;

import lombok.Data;

import javax.persistence.*;

/**
 * OneToOne.
 */

@Entity(name = Post.TABLE_NAME)
@Table(name = Post.TABLE_NAME)
@Data
public class Post {
    public final static String TABLE_NAME = "relationship_one2one__posts";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @OneToOne
    private PostDetails postDetails;
}
