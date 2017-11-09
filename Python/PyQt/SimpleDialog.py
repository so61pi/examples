import sys
from PyQt5.QtWidgets import *


class Form(QDialog):
    def __init__(self, parent=None):
        QDialog.__init__(self, parent)
        self.setWindowTitle("Form")

        layout = QHBoxLayout()

        self.btnOK = QPushButton("OK", self)
        self.btnOK.clicked.connect(self.btnOK_Clicked)
        layout.addWidget(self.btnOK)

        self.btnCancel = QPushButton("Cancel", self)
        self.btnCancel.clicked.connect(self.btnCancel_Clicked)
        layout.addWidget(self.btnCancel)

        self.setLayout(layout)

    def btnOK_Clicked(self):
        QMessageBox.information(self, "Title", "Hello World!")

    def btnCancel_Clicked(self):
        self.close()


if __name__ == "__main__":
    app = QApplication(sys.argv)

    f = Form()
    f.show()

    sys.exit(app.exec())
