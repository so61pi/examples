package com.so61pi.example.jparepo;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import com.so61pi.example.model.Employee;

/**
 * CRUD class for Employee.
 *
 * @see https://docs.spring.io/spring-data/jpa/docs/2.1.9.RELEASE/reference/html/#jpa.query-methods.query-creation
 */
public interface EmployeeRepository extends JpaRepository<Employee, Long> {
	List<Employee> findByFirstName(String firstName);

	List<Employee> findByLastName(String lastName);

	List<Employee> findByActive(boolean active);

	@Query("select e from Employee e where e.firstName = ?1 and e.lastName = ?2")
	Employee findByEmailAddress(String firstName, String lastName);
}
