package model.relationship.one2one.both;

import lombok.Data;

import javax.persistence.*;

/**
 * OneToOne -> OneToOne.
 */

@Entity
@Table(name = Post.TABLE_NAME)
@Data
public class Post {
    public final static String TABLE_NAME = "posts";
    public final static String COL_POST_DETAILS = "postDetails";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @OneToOne
    private PostDetails postDetails;
}
