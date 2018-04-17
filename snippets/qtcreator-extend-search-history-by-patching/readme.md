**1. Take a look at [extend-qtcreator-go-back-steps-by-patching](https://bitbucket.org/so61pi/snippets/src/master/qtcreator-extend-go-back-steps-by-patching/).**


**2. We need to modify function `SearchResultWindow::startNewSearch`, change `MAX_SEARCH_HISTORY`.**


**3. Reference**

- https://github.com/qtproject/qt-creator/blob/v4.1.0/src/plugins/coreplugin/find/searchresultwindow.cpp#L51

- https://github.com/qtproject/qt-creator/blob/v4.1.0/src/plugins/coreplugin/find/searchresultwindow.cpp#L375
