package com.so61pi.example.model.dto;

import lombok.Data;

@Data
public class EmployeeDto {
	private Long id;
	private String firstName;
	private String lastName;
	private Boolean active = true;
}
