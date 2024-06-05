# import Qfontdatabase
from PySide6.QtGui import QFontDatabase
from PySide6.QtWidgets import QApplication, QVBoxLayout, QPushButton, QMainWindow, QScrollArea, QWidget, QLabel


class Window(QMainWindow):
    def __init__(self):
        super(Window, self).__init__()

        layout = QVBoxLayout()
        area = QScrollArea()
        area.setWidgetResizable(True)
        widget = QWidget()
        widget.setLayout(layout)
        area.setWidget(widget)
        self.setCentralWidget(area)

        for family in QFontDatabase.families():
            lb = QLabel(family)
            lb.setFont(family)
            layout.addWidget(lb)


app = QApplication([])
w = Window()
w.show()
app.exec()
