package com.so61pi.example.rest;

import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.server.ResponseStatusException;

import com.so61pi.example.jparepo.EmployeeRepository;
import com.so61pi.example.model.Employee;
import com.so61pi.example.model.dto.EmployeeDto;

@RestController
@RequestMapping(value = "/api/v1/employee", produces = MediaType.APPLICATION_JSON_VALUE)
class EmployeeController {
	@Autowired
	private EmployeeRepository repository;

	@PutMapping("/{id}")
	private void updateEmployee(@PathVariable Long id, @RequestBody EmployeeDto employeeDto) {
		Optional<Employee> optEmployee = repository.findById(id);
		if (!optEmployee.isPresent()) {
			throw new ResponseStatusException(HttpStatus.NOT_FOUND);
		}
		Employee employee = optEmployee.get();
		employee.setFirstName(employeeDto.getFirstName());
		employee.setLastName(employeeDto.getLastName());
		employee.setActive(employeeDto.getActive());
		repository.save(employee);
	}
}
