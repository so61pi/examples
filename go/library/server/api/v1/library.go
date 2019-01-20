package v1

import (
	"examples/go/library/server/db"
	modelsv1 "examples/go/library/server/pkg/models/v1"
	"github.com/gin-gonic/gin"
	"net/http"
	"strconv"
)

func Route(api *Api, group *gin.RouterGroup) {
	group.GET("/books", api.ListBooks)
	group.POST("/books", api.AddBooks)
	group.GET("/books/:bookId", api.GetBook)
}

type Api struct {
	db db.Database
}

func NewApi(db db.Database) (*Api, error) {
	return &Api{db}, nil
}

func (api *Api) ListBooks(c *gin.Context) {
	books, err := api.db.ListBooks()
	if err != nil {
		c.JSON(http.StatusBadRequest, modelsv1.Error{0, err.Error()})
		return
	}

	c.JSON(http.StatusOK, books)
}

func (api *Api) AddBooks(c *gin.Context) {
	books := []modelsv1.Book{}
	if err := c.ShouldBind(books); err != nil {
		c.JSON(http.StatusBadRequest, modelsv1.Error{0, err.Error()})
		return
	}

	if err := api.db.AddBooks(books); err != nil {
		c.JSON(http.StatusInternalServerError, modelsv1.Error{0, err.Error()})
		return
	}

	c.Status(http.StatusOK)
}

func (api *Api) GetBook(c *gin.Context) {
	bookIdStr := c.Param("bookId")
	bookId, err := strconv.ParseInt(bookIdStr, 10, 64)
	if err != nil {
		c.JSON(http.StatusBadRequest, modelsv1.Error{0, err.Error()})
		return
	}

	book, err := api.db.GetBookById(bookId)
	if err != nil {
		if err == db.ErrResourceNotFound {
			c.JSON(http.StatusNotFound, modelsv1.Error{0, err.Error()})
		} else {
			c.JSON(http.StatusInternalServerError, modelsv1.Error{0, err.Error()})
		}
		return
	}

	c.JSON(http.StatusOK, book)
}
