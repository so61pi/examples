package com.so61pi.example.model;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;

import lombok.Data;

/**
 * @implNote lombok.Data generates getters for all fields, a useful toString method,
 * and hashCode and equals implementations that check all non-transient fields. Will
 * also generate setters for all non-final fields, as well as a constructor.
 */
@Data
@Entity
public class Employee {
	@Id
	@GeneratedValue
	private Long id;
	private String firstName;
	private String lastName;
	private Boolean active = true;

	public Employee() {
	}

	public Employee(String firstName, String lastName) {
		this.firstName = firstName;
		this.lastName = lastName;
	}
}
