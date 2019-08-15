package com.so61pi.test.model.relationship.many2one.list;

import lombok.Data;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

@Entity
@Table(name = Post.TABLE_NAME)
@Data
public class Post {
    public final static String TABLE_NAME = "posts";

    @Id
    @GeneratedValue
    private Long id;

    @OneToMany(mappedBy = Comment.COL_POST)
    private List<Comment> comments = new ArrayList<Comment>();
}
