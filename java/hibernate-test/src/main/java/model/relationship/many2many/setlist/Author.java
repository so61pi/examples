package model.relationship.many2many.setlist;

import lombok.Data;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

/**
 * ManyToMany with Set -> ManyToMany with List.
 */

@Entity
@Table(name = Author.TABLE_NAME)
@Data
public class Author {
    public final static String TABLE_NAME = "authors";
    public final static String COL_BOOKS = "books";

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    private Long id;

    @ManyToMany
    private Set<Book> books = new HashSet<Book>();
}
