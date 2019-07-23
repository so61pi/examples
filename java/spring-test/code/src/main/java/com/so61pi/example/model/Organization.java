package com.so61pi.example.model;

import java.util.List;

import lombok.Data;

import javax.persistence.*;

@Data
@Entity
public class Organization {
    @Id
    @GeneratedValue
    private Long id;
    private String name;

    @OneToMany(cascade = {CascadeType.ALL})
    private List<Employee> employees;

    Organization() {}
    public Organization(String name) { this.name = name; }
}
