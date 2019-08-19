package com.so61pi.test.model.relationship.one2many.list;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

/**
 * OneToMany with List.
 */

@Entity
@Table(name = ACart.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class ACart {
    public final static String TABLE_NAME = "acarts";

    @Id
    @GeneratedValue
    private Long id;

    @OneToMany
    // @ManyToOne doesn't have mappedBy, JPA says so.
    private List<BProduct> products = new ArrayList<>();

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public ACart(String name) {
        this.name = name;
    }
}
