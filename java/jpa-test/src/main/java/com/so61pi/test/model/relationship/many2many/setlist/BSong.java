package com.so61pi.test.model.relationship.many2many.setlist;

import lombok.*;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

@Entity
@Table(name = BSong.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)public class BSong {
    public final static String TABLE_NAME = "bsongs";

    @Id
    @GeneratedValue
    private Long id;

    // JPA: For many-to-many bidirectional relationships either side may be the owning side.
    @ManyToMany(mappedBy = AArtist.COL_SONGS)
    private List<AArtist> artists = new ArrayList<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public BSong(String name) {
        this.name = name;
    }
}
