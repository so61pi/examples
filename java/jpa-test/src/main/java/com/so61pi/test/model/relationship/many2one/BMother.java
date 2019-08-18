package com.so61pi.test.model.relationship.many2one;

import lombok.*;

import javax.persistence.*;

@Entity
@Table(name = BMother.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class BMother {
    public final static String TABLE_NAME = "bmothers";

    @Id
    @GeneratedValue
    private Long id;

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public BMother(String name) {
        this.name = name;
    }
}
