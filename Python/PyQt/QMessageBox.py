import sys
from PyQt5.QtWidgets import *


if __name__ == "__main__":
    app = QApplication(sys.argv)

    # static
    QMessageBox.critical(None, "Title", "Text")

    # non-static
    msg = QMessageBox(QMessageBox.Information, "Title", "This is a <b>text</b>", QMessageBox.Yes | QMessageBox.No)
    msg.setDefaultButton(QMessageBox.Yes)
    msg.show()

    sys.exit(app.exec())
