package model.relationship.many2one;

import lombok.Data;

import javax.persistence.*;

/**
 * ManyToOne.
 */

@Entity
@Table(name = PostComment.TABLE_NAME)
@Data
public class PostComment {
    public final static String TABLE_NAME = "comments";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @Column(nullable = false)
    private String content;

    @ManyToOne
    private Post post;
}
