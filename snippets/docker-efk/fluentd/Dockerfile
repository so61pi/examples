FROM fluent/fluentd:v1.4

USER root
RUN gem install fluent-plugin-elasticsearch --version 3.3.1
RUN gem sources --clear-all

USER fluent
