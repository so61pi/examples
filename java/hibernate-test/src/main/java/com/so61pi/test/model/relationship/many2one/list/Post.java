package com.so61pi.test.model.relationship.many2one.list;

import lombok.Data;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

@Entity(name = Post.TABLE_NAME)
@Table(name = Post.TABLE_NAME)
@Data
public class Post {
    public final static String TABLE_NAME = "relationship_many2one_list__posts";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @OneToMany(mappedBy = PostComment.COL_POST)
    private List<PostComment> postComments = new ArrayList<PostComment>();
}
