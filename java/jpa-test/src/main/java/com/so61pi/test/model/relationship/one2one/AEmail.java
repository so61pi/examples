package com.so61pi.test.model.relationship.one2one;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;

/**
 * OneToOne.
 */

@Entity
@Table(name = AEmail.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class AEmail {
    public final static String TABLE_NAME = "aemails";

    @Id
    @GeneratedValue
    private Long id;

    @OneToOne
    private BAccount account;

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public AEmail(String name) {
        this.name = name;
    }
}
