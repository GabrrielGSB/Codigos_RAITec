import sys
from random import randint
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (
  QApplication,
  QLabel,
  QMainWindow,
  QPushButton,
  QVBoxLayout,
  QWidget, QMenu, QAction
)

class AnotherWindow(QWidget):
  def __init__(self):
    super().__init__()

    layout = QVBoxLayout()
    self.label = QLabel("Another Window % d" % randint(0, 100))
    layout.addWidget(self.label)
    self.setLayout(layout)

class MainWindow(QMainWindow):
  def __init__(self):
    super().__init__()
    
  def contextMenuEvent(self, e):
    context = QMenu(self)
    context.addAction(QAction("test 1", self))
    context.addAction(QAction("test 2", self))
    context.addAction(QAction("test 3", self))
    context.exec_(e.globalPos())

app = QApplication(sys.argv)
w = MainWindow()
w.show()
app.exec_()