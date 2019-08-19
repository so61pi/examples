package com.so61pi.test.model.relationship.many2one.list;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;

/**
 * ManyToOne -> OneToMany with List.
 */

@Entity
@Table(name = AComment.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class AComment {
    public final static String TABLE_NAME = "acomments";
    public final static String COL_POST = "post";

    @Id
    @GeneratedValue
    private Long id;

    @ManyToOne
    // @ManyToOne doesn't have mappedBy, JPA says so.
    private BPost post;

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public AComment(String name) {
        this.name = name;
    }
}
