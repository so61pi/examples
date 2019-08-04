- Use `mvn spring-boot:run` to run this web application.
- Swagger documentation can be found at:

  * http://localhost:8080/v2/api-docs
  * http://localhost:8080/swagger-ui.html

- Debugging

```shell
# mvnDebug script simply sets MAVEN_DEBUG_OPTS to `-Xdebug -Xrunjdwp:transport=dt_socket,server=y,suspend=y,address=8000` before running mvn.
mvnDebug spring-boot:run
```

```shell
# To debug application on another host, we have to make JDWP listen on all IPs instead of only localhost.
export MAVEN_DEBUG_OPTS='-Xdebug -Xrunjdwp:transport=dt_socket,server=y,suspend=y,address=*:8000'
mvn spring-boot:run
```
