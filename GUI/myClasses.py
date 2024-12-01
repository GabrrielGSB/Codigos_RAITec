from imports import *
import Funcoes as func

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

        self.Selecao = QComboBox()
        self.Selecao.move(200,150)
        self.Selecao.addItems(["One", "Two", "Three"])
        self.Selecao.currentIndexChanged.connect(self.index_changed)
        self.Selecao.currentTextChanged.connect(self.text_changed)
        self.Selecao.setParent(self.planoDeFundo)
        self.Selecao.setStyleSheet("""
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
        self.Selecao.show()

        self.imagemMaquina = QLabel(self.planoDeFundo)
        self.imagemMaquina.move(650, 50)  # Define a posição da imagem

        self.atualizarImagem(self.Selecao.currentText())

        #Definição do texto de seleção
        self.texto_label = QLabel("Selecione da máquina para receber dados", self.planoDeFundo)
        self.texto_label.move(50, 100)  
        self.texto_label.setFont(QFont("Arial", 14, QFont.Bold))  
        self.texto_label.setStyleSheet("color: white;")  
        self.texto_label.show()

        #Definição do botão utilizado
        self.botao = QPushButton("Clique Aqui", self.planoDeFundo)
        self.botao.move(200, 300)  # Define a posição do botão
        self.botao.setFixedSize(100, 50)  # Define o tamanho do botão
        self.botao.setStyleSheet("""
                                QPushButton {
                                    background-color: darkgreen;
                                    color: white;
                                    border: 2px solid black;
                                    border-radius: 10px;
                                    font-size: 14px;
                                    font-weight: bold;
                                }
                                QPushButton:hover {
                                    background-color: lightgreen;
                                }
                                QPushButton:pressed {
                                    background-color: green;
                                    border: 4px solid white;
                                }
                                """)
        self.botao.clicked.connect(self.acao_botao)  # Conecta o botão a uma função
        self.botao.show()

    def atualizarImagem(self, texto):
        if   texto == "One":
            caminho_imagem = "im.jpg"  
        elif texto == "Two":
            caminho_imagem = "im2.png"
        elif texto == "Three":
            caminho_imagem = "im3.png"
        else:
            caminho_imagem = "im.jpg"

        # Atualiza o pixmap da QLabel com a nova imagem
        pixmap = QPixmap(caminho_imagem)
        self.imagemMaquina.setPixmap(pixmap.scaled(300, 300))  # Ajusta o tamanho da imagem

    def index_changed(self, i): # i is an int
        print("")

    def text_changed(self, s): # s is a str
        self.atualizarImagem(self.Selecao.currentText())
       
    def acao_botao(self):
        print("Botão foi clicado!")   


class aba2(QWidget):
    def __init__(self, imagemFundo):
        super().__init__()
        self.imagemFundo = imagemFundo

        # Define o plano de fundo
        self.planoDeFundo = definirPlanoDeFundo(self.imagemFundo)
        self.planoDeFundo.setGeometry(0, 0, self.width(), self.height())

        self.layout = QVBoxLayout(self)
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.addWidget(self.planoDeFundo)

        # Botão para abrir o gráfico em uma nova janela
        self.botao_grafico = QPushButton("Mostrar Gráfico", self.planoDeFundo)
        self.botao_grafico.move(50, 500)
        self.botao_grafico.setFixedSize(150, 50)
        self.botao_grafico.setStyleSheet("""
            QPushButton {
                background-color: blue;
                color: white;
                font-size: 14px;
                font-weight: bold;
                border-radius: 10px;
            }
            QPushButton:hover {
                background-color: lightblue;
            }
        """)
        self.botao_grafico.clicked.connect(self.mostrar_grafico)

        # Caixas de textos para alterar os textos do gráfico
        self.tituloGrafico = QLineEdit(self.planoDeFundo)
        self.tituloGrafico.setPlaceholderText("Digite o título do gráfico")
        self.tituloGrafico.move(50, 120)
        self.tituloGrafico.setFixedSize(300, 40)
        self.tituloGrafico.setStyleSheet("""
            QLineEdit {
                border: 2px solid gray;
                border-radius: 10px;
                padding: 5px;
                font-size: 14px;
            }
            QLineEdit:focus {
                border-color: blue;
            }
        """)

        self.tituloXlabel = QLineEdit(self.planoDeFundo)
        self.tituloXlabel.setPlaceholderText("Digite o nome do eixo X")
        self.tituloXlabel.move(50, 200)
        self.tituloXlabel.setFixedSize(300, 40)
        self.tituloXlabel.setStyleSheet("""
                                            QLineEdit {
                                                border: 2px solid gray;
                                                border-radius: 10px;
                                                padding: 5px;
                                                font-size: 14px;
                                            }
                                            QLineEdit:focus {
                                                border-color: blue;
                                            }
                                        """)        
        
        self.tituloYlabel = QLineEdit(self.planoDeFundo)
        self.tituloYlabel.setPlaceholderText("Digite o nome do eixo Y")
        self.tituloYlabel.move(50, 300)
        self.tituloYlabel.setFixedSize(300, 40)
        self.tituloYlabel.setStyleSheet("""
                                            QLineEdit {
                                                border: 2px solid gray;
                                                border-radius: 10px;
                                                padding: 5px;
                                                font-size: 14px;
                                            }
                                            QLineEdit:focus {
                                                border-color: blue;
                                            }
                                        """)

        self.janelaGrafico = None
        
    def mostrar_grafico(self):
        """
        Abre uma nova janela com o gráfico gerado pelo matplotlib.
        """
        if not self.janelaGrafico:
            self.janelaGrafico = JanelaGrafico()

        novoTitulo = self.tituloGrafico.text()
        novaXlabel = self.tituloXlabel.text()
        novaYlabel = self.tituloYlabel.text()
        if novoTitulo:
            self.janelaGrafico.grafico.atualizarTitulo(novoTitulo)
        elif novaXlabel:
            self.janelaGrafico.grafico.atualizarXlabel(novaXlabel)
        elif novaYlabel:
            self.janelaGrafico.grafico.atualizarYlabel(novaYlabel)

        self.janelaGrafico.show()

class JanelaGrafico(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Gráfico Externo")
        self.setGeometry(100, 100, 800, 600)

        # Criação do gráfico usando Matplotlib
        self.grafico = Grafico(self)
        self.setCentralWidget(self.grafico)


class Grafico(FigureCanvas):
    def __init__(self, parent=None):

        # Configuração da figura do Matplotlib
        self.fig = Figure(figsize=(8, 6), dpi=100)
        super().__init__(self.fig)

        # Gráfico de exemplo
        self.plotarGrafico()

    def plotarGrafico(self):
        """
        Método para criar o gráfico. Pode ser personalizado.
        """
        self.graficoMostrado = self.fig.add_subplot(111)

        # Gerar dados para o gráfico
        x = np.linspace(0, 10, 100)
        y = np.sin(x)

        # Configuração do gráfico
        self.graficoMostrado.plot(x, y, label="Seno", color="blue", linewidth=2)
        self.graficoMostrado.set_title("titulo", fontsize=16)
        self.graficoMostrado.set_xlabel("Eixo X", fontsize=12)
        self.graficoMostrado.set_ylabel("Eixo Y", fontsize=12)
        self.graficoMostrado.legend()
        self.graficoMostrado.grid()

    def atualizarTitulo(self, novoTitulo):
        self.graficoMostrado.set_title(novoTitulo, fontsize=16)
        print("1")
        self.draw()

    def atualizarXlabel(self, novoTexto):
        self.graficoMostrado.set_xlabel(novoTexto, fontsize=12)
        print("2")
        self.draw()

    def atualizarYlabel(self, novoTexto):
        self.graficoMostrado.set_ylabel(novoTexto, fontsize=12)
        print("3")
        self.draw()


# def definirPlanoDeFundo(imagem):
#     """
#     Placeholder para a função de plano de fundo.
#     """
#     return QWidget()


# if __name__ == "__main__":
#     app = QApplication(sys.argv)
#     window = aba2("background.jpg")
#     window.show()
#     sys.exit(app.exec_())


