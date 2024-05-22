import pandas as pd

from PySide6.QtWidgets import QApplication, QVBoxLayout, QPushButton, QMainWindow, QScrollArea, QWidget
from PySide6.QtGui import QIcon

data = pd.read_csv('freedesktop-icon.txt', sep='\t', header=None, skiprows=1)
data.columns = ['Name', 'Description']


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

        for n, (name, desc) in data.iterrows():
            btn = QPushButton(name)
            btn.setToolTip(desc)
            btn.setIcon(QIcon.fromTheme(name))
            btn.setStyleSheet('text-align:left;')
            layout.addWidget(btn)


app = QApplication([])
w = Window()
w.show()
app.exec()
