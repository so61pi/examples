package com.so61pi.test.model.relationship.many2many.set;

import lombok.*;

import javax.persistence.*;

@Entity
@Table(name = BLabel.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)public class BLabel {
    public final static String TABLE_NAME = "blabels";

    @Id
    @GeneratedValue
    private Long id;

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public BLabel(String name) {
        this.name = name;
    }
}
