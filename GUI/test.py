import sys
from PyQt5.QtCore import QUrl
from PyQt5.QtGui import QGuiApplication
from PyQt5.QtQml import QQmlApplicationEngine

# Cria a aplicação Qt
app = QGuiApplication(sys.argv)

# Configura o carregador de QML
engine = QQmlApplicationEngine()

# Carrega o arquivo .ui.qml
qml_file = "C:\Users\MIroxo\Desktop\GUI\Screen01.ui.qml"
engine.load(QUrl.fromLocalFile(qml_file))

# Verifica se o carregamento foi bem-sucedido
if not engine.rootObjects():
    sys.exit(-1)

# Executa o loop de eventos
sys.exit(app.exec_())
