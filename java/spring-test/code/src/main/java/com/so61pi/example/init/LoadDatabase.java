package com.so61pi.example.init;

import java.util.Arrays;

import lombok.extern.slf4j.Slf4j;

import org.springframework.boot.CommandLineRunner;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import com.so61pi.example.jparepo.OrganizationRepository;
import com.so61pi.example.model.Employee;
import com.so61pi.example.model.Organization;

@Configuration
@Slf4j
class LoadDatabase {
    @Bean
    CommandLineRunner initDatabase(OrganizationRepository repository) {
        Organization o = new Organization("organization");
        o.setEmployees(Arrays.asList(new Employee("employee-1"), new Employee("employee-2")));
        return args -> {
            log.info("Preloading " + repository.save(o));
        };
    }
}
