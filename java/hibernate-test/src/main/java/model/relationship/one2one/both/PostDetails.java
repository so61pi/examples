package model.relationship.one2one.both;

import lombok.Data;

import javax.persistence.*;

@Entity
@Table(name = PostDetails.TABLE_NAME)
@Data
public class PostDetails {
    public final static String TABLE_NAME = "post_details";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @Column(nullable = false)
    private String content;

    @OneToOne(mappedBy = Post.COL_POST_DETAILS)
    private Post post;
}
