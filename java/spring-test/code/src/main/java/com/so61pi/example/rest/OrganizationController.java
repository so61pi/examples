package com.so61pi.example.rest;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.*;

import com.so61pi.example.model.Employee;
import com.so61pi.example.jparepo.OrganizationRepository;
import com.so61pi.example.model.Organization;

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

    @GetMapping("/{orgid}")
    private Optional<Organization> getOrganization(@PathVariable Long orgid) {
        return repository.findById(id);
    }

    @PutMapping("/{orgid}")
    private Optional<Organization> editOrganization(@PathVariable Long orgid, @RequestBody Organization org) {
        return null;
    }
}

@RestController
@RequestMapping(value = "/api/v1/organization/{orgid}/employee", produces = MediaType.APPLICATION_JSON_VALUE)
class EmployeeController {
    @GetMapping("/")
    private List<Employee> getAllOrganizations(@PathVariable Long orgid) {
        return repository.findAll();
    }
}
