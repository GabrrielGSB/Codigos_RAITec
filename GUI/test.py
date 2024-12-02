from imports import *
# import Funcoes as func
def sen(x): return np.sin(x)

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
        self.botao = QPushButton("Começar a Coleta", self.planoDeFundo)
        self.botao.move(200, 300)  # Define a posição do botão
        self.botao.setFixedSize(150, 50)  # Define o tamanho do botão
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
        self.botao.clicked.connect(self.ComecarAquisicaoDados)  # Conecta o botão a uma função
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
       
    def ComecarAquisicaoDados(self):
        escreverCSVTeste() 

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
        self.botao_grafico.clicked.connect(self.mostrarGrafico)

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
        
    def mostrarGrafico(self):
        #Abre uma nova janela com o gráfico gerado pelo matplotlib.
        if not self.janelaGrafico:
            self.janelaGrafico = JanelaGrafico()

        novoTitulo = self.tituloGrafico.text()
        novaXlabel = self.tituloXlabel.text()
        novaYlabel = self.tituloYlabel.text()

        if novoTitulo or novaXlabel or novaYlabel:
            self.janelaGrafico.grafico.atualizarTitulo(novoTitulo)
            self.janelaGrafico.grafico.atualizarXlabel(novaXlabel)
            self.janelaGrafico.grafico.atualizarYlabel(novaYlabel)

        self.janelaGrafico.grafico.iniciarAtualizacao()
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
        self.fig = Figure(figsize=(8, 6), dpi=100)
        super().__init__(self.fig)

        self.graficoMostrado = self.fig.add_subplot(111)

        self.timer = QTimer()
        self.timer.timeout.connect(self.atualizarGrafico)

        # Configuração inicial do gráfico
        self.graficoMostrado.set_title("Gráfico", fontsize=16)
        self.graficoMostrado.set_xlabel("Eixo X", fontsize=12)
        self.graficoMostrado.set_ylabel("Eixo Y", fontsize=12)
        self.graficoMostrado.grid()
        self.line, = self.graficoMostrado.plot([], [], label="Dados", 
                                               color="blue", linewidth=1)
        self.graficoMostrado.legend()

    def iniciarAtualizacao(self):
        """Inicia a atualização periódica do gráfico."""
        self.x_data = []
        self.y_data = []
        self.timer.start(25)  

    def pararAtualizacao(self):
        """Interrompe a atualização periódica do gráfico."""
        self.timer.stop()
    
    def atualizarGrafico(self):
        """Atualiza o gráfico com novos dados."""
        # Adiciona novos pontos ao gráfico
        if len(self.x_data) > 100:  # Limita o número de pontos no gráfico
            self.x_data.pop(0)
            self.y_data.pop(0)

        novo_x = self.x_data[-1] + 0.1 if self.x_data else 0
        novo_y = sen(novo_x)
        self.x_data.append(novo_x)
        self.y_data.append(novo_y)

        # Atualiza a linha do gráfico
        self.line.set_data(self.x_data, self.y_data)
        self.graficoMostrado.set_xlim(max(0, novo_x - 50), novo_x + 1)
        self.graficoMostrado.set_ylim(-1.5, 1.5)

        self.draw()

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



