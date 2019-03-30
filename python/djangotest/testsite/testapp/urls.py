from django.conf.urls import url

from . import views

urlpatterns = [
    # ex: /testapp/
    url(r'^$', views.index, name='index'),
    # ex: /testapp/data/1/
    url(r'^data.json$', views.data, name='data'),
]
