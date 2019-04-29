## Project layout

```
root/
|- deployments/
|  |- staging/
|  `- production/
|- docker/
|  |- svc-service1.Dockerfile
|  `- svc-service2.Dockerfile
|- documents/
|  `- api/
|- services/
|  `- go/
|     |- internal/
|     |  |- config/
|     |  |- db/
|     |  |- log/
|     |  `- models/
|     |- svc-service1/
|     |  |- app/
|     |  |- handler/
|     |  |- router/
|     |  `- main.go
|     |- svc-service2/
|     |- go.mod
|     `- go.sum
`- Makefile
```
