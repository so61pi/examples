package com.so61pi.test.model.relationship.one2one.both;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;

@Entity
@Table(name = BAddress.TABLE_NAME)
@Getter
@Setter
@NoArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class BAddress {
    public final static String TABLE_NAME = "baddresses";

    @Id
    @GeneratedValue
    private Long id;

    @OneToOne(mappedBy = AWorker.COL_ADDRESS)
    private AWorker worker;

    @EqualsAndHashCode.Include
    @Column(nullable = false)
    private String name;

    public BAddress(String name) {
        this.name = name;
    }
}
