package com.so61pi.example.init;

import java.util.Arrays;

import org.springframework.boot.CommandLineRunner;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import com.so61pi.example.jparepo.OrganizationRepository;
import com.so61pi.example.model.Employee;
import com.so61pi.example.model.Organization;

import lombok.extern.slf4j.Slf4j;

@Configuration
@Slf4j
public class LoadDatabase {
	/**
	 * Add some initial data into the database.
	 */
	@Bean
	public CommandLineRunner initDatabase(OrganizationRepository repository) {
		Organization o = new Organization("organization");
		o.setEmployees(Arrays.asList(new Employee("employee-1-fn", "employee-1-ln"),
				new Employee("employee-2-fn", "employee-2-ln")));
		return args -> {
			log.info("Preloading " + repository.save(o));
		};
	}
}
