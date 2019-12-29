FROM fluent/fluentd:v1.8

USER root
RUN gem install fluent-plugin-elasticsearch --version 3.8.0
RUN gem sources --clear-all

USER fluent
