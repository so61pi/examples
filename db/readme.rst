.. contents:: Table of Contents

Versioning
==========

Migration Types
---------------

- Have both up and down paths.

  * Down path can be helpful when we evaluate different strategies of updating schema. 
  * Not all changes are reversible. If a change affects not only the schema, but also the data (e.g. ``DROP TABLE``), it is irreversible.

- Have only up path.

Versioning Tactics
------------------

Migration Script Naming
~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: none

    <YYYY><MM><DD><HH><MM><SS>_<COMMENT>

    db/migrations/2019-08-15-133237_create_posts/up.sql
    db/migrations/2019-08-15-133237_create_posts/down.sql

When migrations are run, all migrations that are not in database are run in alphabetical order. In case there are migration scripts with lower timestamp than in database but they are not in database yet, they will still be executed. Hence, keeping the order of the scripts at merge time is important.

For example, in our current dev database we have version `2019-08-15-050000`, then some one merges `2019-08-15-040000` in. Now if we install new instance of our application, `2019-08-15-040000` will be run before `2019-08-15-050000`. But for upgrading current dev database, `2019-08-15-040000` is after already existing versions. This may or may not lead to unexpected consequence.

Tracking
~~~~~~~~

.. code-block:: sql

    CREATE TABLE schema_changes (
      id SERIAL PRIMARY KEY,
      version TEXT NOT NULL, -- <YYYY><MM><DD><HH><MM><SS>_<COMMENT>
      date_applied TIMESTAMP WITH TIME ZONE
    )

Versioning
~~~~~~~~~~

- All changes are migration scripts.

  * Releasing

    Releases are finalized on release branches. This scheme is suitable when we control all installations.

    In case we need to change the database schema on the release branch, we create a migration script for that on release branch (v2), and also pick it to the dev branch. Note that the picked version on dev must not conflict with the original one, otherwise we cannot upgrade our application from v2 (contains original version) to v3 (contains picked version). Specifically, the up path of picked version must be a no-op in case original one is already applied, and the down path of original must be a no-op if the picked version is already reverted.

    This is simple, but we cannot remove old migration scripts as all new installations invoke all them. It can also lead to errors as the number of scripts grows.

  * Applying migrations

    .. code-block:: none

        applicable change scripts = migrations in source code - schema_changes
        apply change scripts

- One baseline script for each application version.

  * Releasing

    Releases are finalized on release branches. This scheme is suitable when users install the software on their own.

    - Imagine we have some migrations like this on master.

      .. code-block:: none

          src/
          └── db/
              ├── baseline-details/
              └── migrations/
                  ├── 0000-00-00-000000_baseline/             <-- empty at this point
                  ├── 2019-08-15-000000_v1/                   <-- only a marker, always empty
                  ├── 2019-08-16-000000_add_table/
                  ├── 2019-08-17-000000_add_column/
                  └── 2019-08-18-000000_change_column_type/

    - Then we add a new commit with empty v2 migration script to clearly separate v1 and v2. It also will help when we merge release branch back to master.

      .. code-block:: none

          src/
          └── db/
              ├── baseline-details/
              └── migrations/
                  ├── 0000-00-00-000000_baseline/
                  ├── 2019-08-15-000000_v1/
                  ├── 2019-08-16-000000_add_table/
                  ├── 2019-08-17-000000_add_column/
                  ├── 2019-08-18-000000_change_column_type/
                  └── 2019-08-19-000000_v2/                   <-- added on master, always empty

    - Create a release branch from the commit right before the above commit. Then we do all the testing. In case we need to add a new fix with a migration, we add one on both release and master branches (and they must be compatible). Finally, we create a new baseline from all current migrations.

      .. code-block:: none

          src/
          └── db/
              ├── baseline-details/                           <-- move all v1 migrations to baseline-details
              │   │                                               this means current baseline equals to baseline-details
              │   ├── 2019-08-15-000000_v1/
              │   ├── 2019-08-16-000000_add_table/
              │   ├── 2019-08-17-000000_add_column/
              │   └── 2019-08-18-000000_change_column_type/
              └── migrations/
                  ├── 0000-00-00-000000_baseline/             <-- update to new baseline = 2019-08-16-000000 + 2019-08-17-000000 + 2019-08-18-000000
                  │                                               can be generated by exporting schema from database.
                  └── 0000-00-00-000000_v1_new_install/       <-- used to mark new installs from this particular baseline

      Note that baseline is only used for new install. If upgrading is an option, we also want to create a migration package that holds all migrations in a particular version. For instance, upgrading version 1 to version 2 requires all migrations from v2 to before v3.

    - Now we need to update baseline on master by merging release branch.

    .. code-block:: none

        src/
        └── db/
            ├── baseline-details/
            │   ├── 2019-08-15-000000_v1/
            │   ├── 2019-08-16-000000_add_table/
            │   ├── 2019-08-17-000000_add_column/
            │   └── 2019-08-18-000000_change_column_type/
            └── migrations/
                ├── 0000-00-00-000000_baseline/             <-- new baseline merged back from release branch
                │                                           <-- we don't keep v1_new_install here as it is useless and will make the schema version table in database polluted
                ├── 2019-08-19-000000_v2/
                └── 2019-08-20-000000_add_table/            <-- someone already adds new migration script in v2

  * Applying migrations

    .. code-block:: none

        if schema_changes table does not exist {
          apply baseline
        }
        applicable change scripts = migrations in source code - schema_changes
        apply change scripts

When merging migration scripts from feature branch to main development branch, we must make sure the scripts on the source branch have higher versions than the ones in the dest branch.

We also have to take care of migrations in code, like migrating data with complex logic.

References
----------

- https://odetocode.com/blogs/scott/archive/2008/02/03/versioning-databases-branching-and-merging.aspx
