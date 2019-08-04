package com.so61pi.example.model;

import java.util.List;

import javax.persistence.CascadeType;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.OneToMany;

import lombok.Data;

/**
 * @implNote lombok.Data generates getters for all fields, a useful toString method,
 * and hashCode and equals implementations that check all non-transient fields. Will
 * also generate setters for all non-final fields, as well as a constructor.
 */
@Data
@Entity
public class Organization {
	@Id
	@GeneratedValue
	private Long id;
	private String name;

	@OneToMany(cascade = { CascadeType.ALL })
	private List<Employee> employees;

	public Organization() {
	}

	public Organization(String name) {
		this.name = name;
	}
}
