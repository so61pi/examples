package com.so61pi.test.model.relationship.many2many.list;

import lombok.*;

import javax.persistence.*;

@Entity
@Table(name = BBook.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class BBook {
    public final static String TABLE_NAME = "bbooks";

    @Id
    @GeneratedValue
    private Long id;

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public BBook(String name) {
        this.name = name;
    }
}
