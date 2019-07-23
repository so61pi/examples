package com.so61pi.example.jparepo;

import org.springframework.data.jpa.repository.JpaRepository;

import com.so61pi.example.model.Organization;

public interface OrganizationRepository extends JpaRepository<Organization, Long> {}
