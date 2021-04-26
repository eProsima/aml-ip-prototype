"""."""
import sys

from PyQt5.QtCore import Qt
from PyQt5.QtGui import QColor, QPalette
from PyQt5.QtWidgets import QApplication, QGridLayout, QHBoxLayout
from PyQt5.QtWidgets import QCheckBox, QErrorMessage, QSpinBox, QVBoxLayout
from PyQt5.QtWidgets import QComboBox, QFrame, QLabel, QPushButton
from PyQt5.QtWidgets import QDialog, QFileDialog, QGroupBox, QLineEdit
from PyQt5.QtWidgets import QTabWidget, QWidget


class DialogApp(QDialog):
    def __init__(self, parent=None):
        super(DialogApp, self).__init__(parent)

        QApplication.setStyle('Fusion')
        self.darkPalette()
        self.setWindowTitle('AML IP Prototype')
        self.setGeometry(10, 10, 640, 480)

        # Set Tabs layout
        self.buildTabs()

        # Set bottom layout
        self.createCancelButton()
        self.createResetButton()
        self.createRunButton()

        bottomLayout = QHBoxLayout()
        bottomLayout.addWidget(self.cancelButton)
        bottomLayout.addWidget(self.resetButton)
        bottomLayout.addWidget(self.runButton)

        mainLayout = QVBoxLayout()
        mainLayout.addWidget(self.tabs)
        mainLayout.addLayout(bottomLayout)
        self.setLayout(mainLayout)

    def createResetButton(self):
        self.resetButton = QPushButton('Reset')
        self.resetButton.setFixedSize(80, 25)
        self.resetButton.clicked.connect(self.resetValues)

    def createRunButton(self):
        self.runButton = QPushButton('Run')
        self.runButton.setFixedSize(80, 25)
        self.runButton.clicked.connect(self.run)

    def createCancelButton(self):
        self.cancelButton = QPushButton('Cancel')
        self.cancelButton.setFixedSize(80, 25)
        self.cancelButton.clicked.connect(self.closeWindow)

    def buildTabs(self):
        self.tabDL = self.createNodeLayoutTab()
        self.tabEngine = self.createNodeLayoutTab()
        self.tabDS = self.createDSLayoutTab()

        self.tabs = QTabWidget()
        self.tabs.addTab(self.tabDL, 'AML DL Participant')
        self.tabs.addTab(self.tabEngine, 'AML Engine Participant')
        self.tabs.addTab(self.tabDS, 'AML Discovery Server Participant')

    def createNodeLayoutTab(self):
        tab = QWidget()

        dlLayout = QVBoxLayout()
        pushButton1 = QPushButton("PyQt5 button")
        dlLayout.addWidget(pushButton1)
        tab.setLayout(dlLayout)

        return tab

    def createDSLayoutTab(self):
        tab = QWidget()

        dlLayout = QVBoxLayout()
        pushButton1 = QPushButton("DS button")
        dlLayout.addWidget(pushButton1)
        tab.setLayout(dlLayout)

        return tab

###
    def resetValues(self):
        print('Reset pressed')

    def closeWindow(self):
        print('Closing the window')
        self.close()

    def run(self):
        print('Run pressed')
###

    def darkPalette(self):
        # Now use a palette to switch to dark colors:
        self.palette = QApplication.palette()
        self.palette.setColor(QPalette.Window, QColor(53, 53, 53))
        self.palette.setColor(QPalette.WindowText, Qt.white)
        self.palette.setColor(QPalette.Base, QColor(25, 25, 25))
        self.palette.setColor(QPalette.AlternateBase, QColor(53, 53, 53))
        self.palette.setColor(QPalette.ToolTipBase, Qt.white)
        self.palette.setColor(QPalette.ToolTipText, Qt.white)
        self.palette.setColor(QPalette.Text, Qt.white)
        self.palette.setColor(QPalette.Button, QColor(53, 53, 53))
        self.palette.setColor(QPalette.ButtonText, Qt.white)
        self.palette.setColor(QPalette.BrightText, Qt.red)
        self.palette.setColor(QPalette.Link, QColor(42, 130, 218))
        self.palette.setColor(QPalette.Highlight, QColor(42, 130, 218))
        self.palette.setColor(QPalette.HighlightedText, Qt.black)
        self.palette.setColor(QPalette.Disabled, QPalette.WindowText, Qt.black)
        self.palette.setColor(
            QPalette.Disabled,
            QPalette.ToolTipBase,
            Qt.black)
        self.palette.setColor(
            QPalette.Disabled,
            QPalette.ToolTipText,
            Qt.black)
        self.palette.setColor(QPalette.Disabled, QPalette.Text, Qt.black)
        self.palette.setColor(QPalette.Disabled, QPalette.ButtonText, Qt.black)
        self.palette.setColor(
            QPalette.Disabled,
            QPalette.HighlightedText,
            Qt.black)
        self.palette.setColor(QPalette.Disabled, QPalette.Window, Qt.black)
        self.palette.setColor(
            QPalette.Disabled,
            QPalette.Text,
            QColor(53, 53, 53))
        QApplication.setPalette(self.palette)


if __name__ == '__main__':

    app = QApplication(sys.argv)
    gallery = DialogApp()
    gallery.show()
    sys.exit(app.exec_())
