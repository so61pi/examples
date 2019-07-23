package com.so61pi.example.rest;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import com.so61pi.example.jparepo.OrganizationRepository;
import com.so61pi.example.model.Organization;

@RestController
@RequestMapping("/api")
class OrganizationController {
    @Autowired
    private OrganizationRepository repository;

    @GetMapping("/organization")
    private List<Organization> getAllOrganizations() {
        return repository.findAll();
    }

    @PostMapping("/organization")
    private Organization newOrganization(@RequestBody Organization organization) {
        return repository.save(organization);
    }

    @GetMapping("/organization/{id}")
    private Optional<Organization> getOrganization(@PathVariable Long id) {
        return repository.findById(id);
    }
}
