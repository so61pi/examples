package com.so61pi.test.model.relationship.one2one.both;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;

/**
 * OneToOne -> OneToOne.
 */

@Entity
@Table(name = AWorker.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class AWorker {
    public final static String TABLE_NAME = "aworkers";
    public final static String COL_ADDRESS = "address";

    @Id
    @GeneratedValue
    private Long id;

    @OneToOne
    private BAddress address;

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public AWorker(String name) {
        this.name = name;
    }
}
