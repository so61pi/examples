package com.so61pi.test.model.relationship.many2one.list;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

@Entity
@Table(name = BPost.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class BPost {
    public final static String TABLE_NAME = "bposts";

    @Id
    @GeneratedValue
    private Long id;

    @OneToMany(mappedBy = AComment.COL_POST)
    private List<AComment> comments = new ArrayList<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public BPost(String name) {
        this.name = name;
    }
}
