package com.so61pi.test.model.relationship.one2one;

import lombok.Data;

import javax.persistence.*;

@Entity
@Table(name = Account.TABLE_NAME)
@Data
public class Account {
    public final static String TABLE_NAME = "accounts";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;
}
