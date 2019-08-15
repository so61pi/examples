package com.so61pi.test.model.relationship.one2one;

import lombok.Data;

import javax.persistence.*;

/**
 * OneToOne.
 */

@Entity
@Table(name = Email.TABLE_NAME)
@Data
public class Email {
    public final static String TABLE_NAME = "emails";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @OneToOne
    private Account account;
}
