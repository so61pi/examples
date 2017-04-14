**1. Take a look at [extend-qtcreator-go-back-steps-by-patching](https://bitbucket.org/snippets/so61pi/LqEXe).**


**2. We need to modify function `SearchResultWindow::startNewSearch`, change `MAX_SEARCH_HISTORY`.**


**3. Reference**

- https://github.com/qtproject/qt-creator/blob/v4.1.0/src/plugins/coreplugin/find/searchresultwindow.cpp#L51

- https://github.com/qtproject/qt-creator/blob/v4.1.0/src/plugins/coreplugin/find/searchresultwindow.cpp#L375
