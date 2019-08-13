package model.relationship.one2one;

import lombok.Data;

import javax.persistence.*;

/**
 * OneToOne.
 */

@Entity
@Table(name = Post.TABLE_NAME)
@Data
public class Post {
    public final static String TABLE_NAME = "posts";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @OneToOne
    private PostDetails postDetails;
}
