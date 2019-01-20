package router

import (
	"github.com/gin-gonic/gin"

	apiv1 "examples/go/library/server/api/v1"
)

func NewRouter(api *apiv1.Api) *gin.Engine {
	router := gin.Default()

	apiv1.Route(api, router.Group("/api/v1"))

	return router
}
