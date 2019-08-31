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

    db/migrations/20160815133237_create_posts/up.sql
    db/migrations/20160815133237_create_posts/down.sql

Tracking
~~~~~~~~

.. code-block:: sql

    CREATE TABLE schema_changes (
      id SERIAL PRIMARY KEY,
      version TEXT NOT NULL, # <YYYY><MM><DD><HH><MM><SS>_<COMMENT>
      date_applied TIMESTAMP WITH TIME ZONE
    )

Versioning
~~~~~~~~~~

- All changes are migration scripts.

  * Releasing

    Releases are finalized on release branches.

    In case we need to change the database schema on the release branch, we create a migration script for that on release branch, and also pick it to the dev branch. Note that the picked version on dev must not conflict with the original one, otherwise we cannot upgrade our application from v2 (contains original version) to v3 (contains picked version).

  * Applying migrations

    .. code-block:: none

        applicable change scripts = source code - schema_changes
        apply change scripts

- One baseline script for each application version.

  * Releasing

    Releases are finalized on release branches.

    When new application version is about to be released, new baseline (v2) is created before release branch (v2). The content of new baseline must be equivalent to all migration scripts plus previous baseline (v1).

    Old migration scripts must be kept so already installed instances of application can be upgraded to new version. Baselines are for new installs only.

    (I don't know if this strategy is better than the first one, while we still have to maintain both all migration scripts and baselines. And we still have the same caution when fixing schema on release branch.)

  * Applying migrations

    .. code-block:: none

        if schema_changes table already exists {
          applicable change scripts = source code - schema_changes
          apply change scripts
        } else {
          apply baseline
        }

References
----------

- https://odetocode.com/blogs/scott/archive/2008/02/03/versioning-databases-branching-and-merging.aspx
