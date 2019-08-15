package com.so61pi.test.model.relationship.one2one.both;

import lombok.Data;

import javax.persistence.*;

/**
 * OneToOne -> OneToOne.
 */

@Entity
@Table(name = Worker.TABLE_NAME)
@Data
public class Worker {
    public final static String TABLE_NAME = "posts";
    public final static String COL_ADDRESS = "address";

    @Id
    @GeneratedValue
    private Long id;

    @OneToOne
    private Address address;
}
