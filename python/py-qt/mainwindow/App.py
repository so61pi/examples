from PyQt5.QtWidgets import *

#
# MainWindow.py is created from MainWindow.ui which in turn is designed in QtCreator
#
# sudo apt install pyqt5-dev-tools
# pyuic5 MainWindow.ui -o MainWindow.py
#

import MainWindow


class ExampleApp(QMainWindow, MainWindow.Ui_MainWindow):
    def __init__(self, parent=None):
        QMainWindow.__init__(self, parent)
        self.setupUi(self)


if __name__ == "__main__":
    import sys
    app = QApplication(sys.argv)
    form = ExampleApp()
    form.show()
    sys.exit(app.exec())
