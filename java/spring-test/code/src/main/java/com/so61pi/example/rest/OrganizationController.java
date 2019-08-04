package com.so61pi.example.rest;

import java.util.List;
import java.util.Optional;

import javax.transaction.Transactional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.server.ResponseStatusException;

import com.so61pi.example.jparepo.OrganizationRepository;
import com.so61pi.example.model.Employee;
import com.so61pi.example.model.Organization;
import com.so61pi.example.model.dto.OrganizationDto;

@RestController
@RequestMapping(value = "/api/v1/organization", produces = MediaType.APPLICATION_JSON_VALUE)
class OrganizationController {
	@Autowired
	private OrganizationRepository repository;

	@GetMapping("/")
	private List<Organization> getAllOrganizations() {
		return repository.findAll();
	}

	@PostMapping(value = "/", consumes = MediaType.APPLICATION_JSON_VALUE)
	private Organization newOrganization(@RequestBody Organization org) {
		return repository.save(org);
	}

	@GetMapping("/{id}")
	private Optional<Organization> getOrganization(@PathVariable Long id) {
		return repository.findById(id);
	}

	@PutMapping("/{id}")
	private void updateOrganization(@PathVariable Long id, @RequestBody OrganizationDto orgDto) {
		Optional<Organization> optOrg = repository.findById(id);
		if (!optOrg.isPresent()) {
			throw new ResponseStatusException(HttpStatus.NOT_FOUND);
		}
		Organization org = optOrg.get();
		org.setName(orgDto.getName());
		repository.save(org);
	}

	@GetMapping("/{id}/employee")
	private List<Employee> getAllEmployees(@PathVariable Long id) {
		Optional<Organization> optOrg = repository.findById(id);
		if (!optOrg.isPresent()) {
			throw new ResponseStatusException(HttpStatus.NOT_FOUND);
		}
		return optOrg.get().getEmployees();
	}
}
