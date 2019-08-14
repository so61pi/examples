package com.so61pi.test.model.relationship.one2one.both;

import lombok.Data;

import javax.persistence.*;

/**
 * OneToOne -> OneToOne.
 */

@Entity(name = Post.TABLE_NAME)
@Table(name = Post.TABLE_NAME)
@Data
public class Post {
    public final static String TABLE_NAME = "relationship_one2one_both__posts";
    public final static String COL_POST_DETAILS = "postDetails";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @OneToOne
    private PostDetails postDetails;
}
