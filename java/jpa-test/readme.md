## Concepts

### Entities

- Managed by entity manager.

### Associations/Relationships/References

- OneToOne
- OneToMany
- ManyToOne
- ManyToMany

#### Owning Side

- Owners are responsible for creating/removing associations.
- In case owners are removed, the associations are removed also.
- Owned entities cannot be removed without breaking the association first.

### Persistence Context

Contains entities.

> A **persistence context** is a set of managed entity instances in which for any persistent entity identity there is a unique entity instance.

### Entity Manager

Provides APIs to manage entities.

> Within the **persistence context**, the entity instances and their lifecycle are managed by the **entity manager**.