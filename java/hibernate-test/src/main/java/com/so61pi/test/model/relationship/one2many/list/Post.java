package com.so61pi.test.model.relationship.one2many.list;

import lombok.Data;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

/**
 * OneToMany with List.
 */

@Entity(name = Post.TABLE_NAME)
@Table(name = Post.TABLE_NAME)
@Data
public class Post {
    public final static String TABLE_NAME = "relationship_one2many_list__posts";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @OneToMany
    private List<PostComment> postComments = new ArrayList<PostComment>();
}
