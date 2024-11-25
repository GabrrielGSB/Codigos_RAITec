from imports import *
from myClasses import aba1

class MainWindow(QMainWindow):
  def __init__(self):
    super().__init__()

    self.setWindowTitle("setGeometry Example")
    self.setFixedSize(1000, 650)

    tabs = QTabWidget()
    tabs.setTabPosition(QTabWidget.North)
    tabs.setMovable(False)

    tabs.addTab(aba1('im1.png'), "aba1")
    # tabs.addTab(definirPlanoDeFundo('im1.png'), "aba2")
    # a = definirPlanoDeFundo('im.jpg')

    self.setCentralWidget(tabs)
    # tabs.setParent(self)


app = QApplication(sys.argv)
window = MainWindow()
window.show()
app.exec_()