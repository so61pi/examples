FROM python:3.6.5

RUN pip install robotframework
RUN pip install --upgrade robotframework-sshlibrary

ARG groupid
ARG userid
RUN addgroup --quiet --gid ${groupid} worker
RUN adduser --quiet --uid ${userid} --disabled-password --gecos '' --ingroup worker worker

USER worker
WORKDIR /code
CMD robot test.robot
