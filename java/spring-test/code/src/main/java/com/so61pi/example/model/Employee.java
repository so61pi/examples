package com.so61pi.example.model;

import lombok.Data;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;

@Data
@Entity
public class Employee {
    @Id
    @GeneratedValue
    private Long id;
    private String name;

    Employee() {}
    public Employee(String name) { this.name = name; }
}
