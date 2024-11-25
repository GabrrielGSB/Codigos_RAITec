from imports import *

class Color(QWidget):
    def __init__(self, color):
        super().__init__()

        self.setAutoFillBackground(True)
        palette = self.palette()
        palette.setColor(QPalette.Window, QColor(color))
        self.setPalette(palette)

class DesenharRetangulo(QWidget):
    def __init__(self, color, largura=100, altura=100, pos_x=0, pos_y=0):
        super().__init__()
        self.color = color  # Cor do retângulo
        self.largura = largura  # Largura do retângulo
        self.altura = altura  # Altura do retângulo
        self.pos_x = pos_x  # Posição horizontal
        self.pos_y = pos_y  # Posição vertical

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)  # Habilita bordas suaves
        
        # Define a cor do retângulo
        painter.setBrush(QColor(self.color))
        painter.setPen(Qt.NoPen)  # Remove bordas

        # Desenha o retângulo na posição especificada
        painter.drawRect(self.pos_x, self.pos_y, self.largura, self.altura)

class Tab1(QWidget):
    def __init__(self):
        super().__init__()

        self.layout = QHBoxLayout()
        self.layout.addWidget(Color("red"))
        self.layout.addWidget(Color("green"))
        self.setLayout(self.layout)

class definirPlanoDeFundo(QWidget):
    def __init__(self, caminho):
        super().__init__()
        self.caminho = caminho

    def paintEvent(self, event):
        painter = QPainter(self)
    
        planoDeFundo = QPixmap(self.caminho)  
        
        painter.drawPixmap(0, 0, self.width(), self.height(), planoDeFundo)

class aba1(QWidget):
    def __init__(self, imagemFundo):
        super().__init__()

        self.imagemFundo = imagemFundo

        self.planoDeFundo = definirPlanoDeFundo(self.imagemFundo)
        self.planoDeFundo.setGeometry(0, 0, self.width(), self.height())

        self.layout = QVBoxLayout(self)
        self.layout.setContentsMargins(0, 0, 0, 0) 
        self.layout.addWidget(self.planoDeFundo)

        self.widget = QComboBox()
        self.widget.addItems(["One", "Two", "Three"])
        self.widget.currentIndexChanged.connect(self.index_changed)
        self.widget.currentTextChanged.connect(self.text_changed)
        self.widget.setParent(self.planoDeFundo)
        self.widget.move(100,100)

        self.widget.setStyleSheet("""
    QComboBox {
        border: 1px solid black;
        border-radius: 10px;
        padding: 5px;
        background-color: lightblue;
        color: darkblue;
    }
    QComboBox QAbstractItemView {
        border: 1px solid gray;
        selection-background-color: lightgray;
        background: white;
    }
""")
        self.widget.show()


    def index_changed(self, i): # i is an int
        print(i)
    def text_changed(self, s): # s is a str
        print(s)
       
        
