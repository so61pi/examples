package com.so61pi.example.jparepo;

import org.springframework.data.jpa.repository.JpaRepository;

import com.so61pi.example.model.Organization;

/**
 * CRUD class for Organization.
 *
 * @see https://docs.spring.io/spring-data/jpa/docs/2.1.9.RELEASE/reference/html/
 */
public interface OrganizationRepository extends JpaRepository<Organization, Long> {
}
