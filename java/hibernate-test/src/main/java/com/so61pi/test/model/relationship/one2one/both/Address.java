package com.so61pi.test.model.relationship.one2one.both;

import lombok.Data;

import javax.persistence.*;

@Entity
@Table(name = Address.TABLE_NAME)
@Data
public class Address {
    public final static String TABLE_NAME = "addresses";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @OneToOne(mappedBy = Worker.COL_ADDRESS)
    private Worker worker;
}
