package com.so61pi.test.model.relationship.one2one;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;

@Entity
@Table(name = BAccount.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class BAccount {
    public final static String TABLE_NAME = "baccounts";

    @Id
    @GeneratedValue
    private Long id;

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public BAccount(String name) {
        this.name = name;
    }
}
