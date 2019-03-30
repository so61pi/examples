FROM postgres:11.2-alpine

COPY --chown=nobody:nogroup 00-init.sql /docker-entrypoint-initdb.d/
COPY --chown=nobody:nogroup 50-sample.sql /docker-entrypoint-initdb.d/

