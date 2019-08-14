package com.so61pi.test.model.relationship.many2one.set;

import lombok.Data;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

@Entity(name = Post.TABLE_NAME)
@Table(name = Post.TABLE_NAME)
@Data
public class Post {
    public final static String TABLE_NAME = "relationship_many2one_set__posts";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @OneToMany(mappedBy = PostComment.COL_POST)
    private Set<PostComment> postComments = new HashSet<PostComment>();
}
