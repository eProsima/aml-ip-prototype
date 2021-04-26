"""."""
import sys

from PyQt5.QtCore import Qt
from PyQt5.QtGui import QColor, QPalette
from PyQt5.QtWidgets import QApplication, QGridLayout, QHBoxLayout
from PyQt5.QtWidgets import QComboBox, QFrame, QLabel, QPushButton
from PyQt5.QtWidgets import QDialog, QFileDialog, QGroupBox, QLineEdit
from PyQt5.QtWidgets import QCheckBox, QErrorMessage, QSpinBox, QVBoxLayout

import pandas as pd


class testTypes(QDialog):
    def __init__(self, parent):
        super(testTypes, self).__init__(parent)

        self.setWindowTitle('Discovery Server: Test cases')

        self.acceptButton = QPushButton('Accept')
        self.acceptButton.setFixedSize(80, 25)
        self.acceptButton.clicked.connect(self.saveAndClose)

        self.testCasesData = pd.read_csv(parent.testsDataFile.text())
        self.testCasesData.dropna(inplace=True)
        self.testCasesData.reset_index(drop=True, inplace=True)

        self.createTestsCheckBoxList()

        bottomLayout = QVBoxLayout()
        bottomLayout.addWidget(self.acceptButton, alignment=Qt.AlignRight)

        mainLayout = QGridLayout()
        mainLayout.addLayout(self.discoveryLayout, 0, 0, 1, 2)
        mainLayout.addLayout(bottomLayout, 1, 0, 1, 2)
        self.setLayout(mainLayout)

    def createTestsCheckBoxList(self):
        self.discoveryLayout = QGridLayout()

        checkedList = [True for i in range(len(self.testCasesData.index))]
        self.checkedTests = pd.concat(
            [self.testCasesData['ID'], pd.Series(checkedList)],
            axis=1,
            keys=['test_id', 'checked'])

        self.testsCheckBox = []
        self.discoveryParadigmGroupBox = []
        xloc = 0
        idxTestId = -1
        for discovery_id, discovery_data in self.testCasesData.groupby(
            ['discovery_paradigm']
        ):
            discoveryParadigm = ' '.join(
                map(str, discovery_id.split('_'))).upper()
            self.discoveryParadigmGroupBox.append(QGroupBox(discoveryParadigm))

            internalLayout = QVBoxLayout()

            for testId, testData in discovery_data.groupby(['ID']):
                idxTestId += 1
                self.testsCheckBox.append(QCheckBox(testId))
                self.testsCheckBox[idxTestId].setChecked(True)
                self.testsCheckBox[idxTestId].toggled.connect(
                    self.changeTestState)
                internalLayout.addWidget(self.testsCheckBox[idxTestId])

            internalLayout.setAlignment(Qt.AlignTop)

            self.discoveryParadigmGroupBox[xloc].setLayout(internalLayout)
            self.discoveryLayout.addWidget(
                self.discoveryParadigmGroupBox[xloc], 0, xloc)
            xloc += 1

    def changeTestState(self):
        sender = self.sender().text()
        index = self.checkedTests.loc[
            self.checkedTests['test_id'] == sender].index[0]
        self.checkedTests.at[index, 'checked'] = not self.checkedTests.at[
            index, 'checked']

    def saveAndClose(self):
        self.finalTestCasesList = self.testCasesData.where(
            self.checkedTests['checked']).dropna()
        self.finalTestCasesList.reset_index(drop=True, inplace=True)
        self.accept()


class configFilesApp(QDialog):
    def __init__(self, parent=None):
        super(configFilesApp, self).__init__(parent)

        QApplication.setStyle('Fusion')
        self.darkPalette()
        self.setWindowTitle('Discovery Server')
        self.setGeometry(10, 10, 640, 480)

        self.buildXMLGroupBox()
        self.buildJsonGroupBox()

        self.createResetButton()
        self.createRunButton()

        nSepartors = 2
        separtorList = []
        for i in range(nSepartors):
            separtor = QFrame()
            separtorList.append(separtor)
            separtorList[i].setFrameShape(QFrame.HLine)
            separtorList[i].setFrameShadow(QFrame.Sunken)

        # Set main layout
        bottomLayout = QHBoxLayout()
        bottomLayout.addWidget(self.resetButton)
        bottomLayout.addWidget(self.runButton)

        mainLayout = QVBoxLayout()
        mainLayout.addWidget(self.xmlGroupBox)
        mainLayout.addWidget(separtorList[0])
        mainLayout.addWidget(self.jsonGroupBox)
        mainLayout.addWidget(separtorList[1])
        mainLayout.addLayout(bottomLayout)
        self.setLayout(mainLayout)

    def createResetButton(self):
        self.resetButton = QPushButton('Reset')
        self.resetButton.setFixedSize(80, 25)
        self.resetButton.clicked.connect(self.resetValues)

    def createRunButton(self):
        self.runButton = QPushButton('Run')
        self.runButton.setFixedSize(80, 25)

    def resetValues(self):
        self.xmlGroupBox.setChecked(True)

        self.rpisDataFile.setText('./resources/RPi_IPs.csv')
        self.testsDataFile.setText('./resources/Test_Types.csv')
        self.xsdFile.setText('./resources/discovery-server.xsd')

        self.participants.setValue(1)
        self.simplePubs.setValue(5)
        self.simpleSubs.setValue(5)
        self.servers.setValue(1)
        self.clients.setValue(1)
        self.dsPubs.setValue(5)
        self.dsSubs.setValue(5)

        self.leaseDuration.clear()
        self.leaseDuration.addItems(['DURATION_INFINITY', 'Duration_t'])
        self.secLeaseDuration.setValue(20)
        self.nanosecLeaseDuration.setValue(0)

        self.transportProtocol.clear()
        self.transportProtocol.addItems(['UDPv4', 'UDPv6', 'TCPv4', 'TCPv6'])

        self.localDSRunFile.setText('./run_discovery_server.bash')
        self.remoteDir.setText('~/autogenerated_xml')
        self.remoteDSInstallDir.setText('./autogenerated_xml/install')

    def buildJsonGroupBox(self):
        self.jsonGroupBox = QGroupBox('Build Json deployment files')
        self.jsonGroupBox.setCheckable(True)
        self.jsonGroupBox.setChecked(True)

        self.createFilesJsonGroupBox()

        # Set layout
        layout = QGridLayout()
        layout.addWidget(self.filesJsonGroupBox, 0, 0)
        self.jsonGroupBox.setLayout(layout)

    def createFilesJsonGroupBox(self):
        self.filesJsonGroupBox = QGroupBox('Required files and directories')

        # Local DiscoveryServer run script
        self.localDSRunFile = QLineEdit()
        self.localDSRunFile.setText('./run_discovery_server.bash')
        localDSRunLabel = QLabel('Local DiscoveryServer\nrun script:')
        localDSRunLabel.setBuddy(self.localDSRunFile)
        self.localDSRunButton = QPushButton('Browse')

        self.localDSRunButton.clicked.connect(
            lambda: self.browseBashFiles(self.localDSRunFile))

        # Remote raspfarm test directory
        self.remoteDir = QLineEdit()
        self.remoteDir.setText('~/autogenerated_xml')
        remoteDirLabel = QLabel('Remote results directory:')

        # Remote DiscoveryServer app install directory
        self.remoteDSInstallDir = QLineEdit()
        self.remoteDSInstallDir.setText('./autogenerated_xml/install')
        remoteDSInstallDirLabel = QLabel(
            'Remote DiscoveryServer\napp. install directory:')

        # Set layout
        layout = QGridLayout()
        layout.addWidget(localDSRunLabel, 0, 0)
        layout.addWidget(self.localDSRunFile, 0, 1)
        layout.addWidget(self.localDSRunButton, 0, 2)
        layout.addWidget(remoteDirLabel, 1, 0)
        layout.addWidget(self.remoteDir, 1, 1, 1, 2)
        layout.addWidget(remoteDSInstallDirLabel, 2, 0)
        layout.addWidget(self.remoteDSInstallDir, 2, 1, 1, 2)
        self.filesJsonGroupBox.setLayout(layout)

    def browseBashFiles(self, lineEdit):
        """."""
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        fileName, _ = QFileDialog.getOpenFileName(
            None,
            'QFileDialog.getOpenFileName()',
            '',
            'Bash scripts (*.bash *.sh)',
            options=options
        )
        if fileName:
            lineEdit.setText(fileName)

    def buildXMLGroupBox(self):
        self.xmlGroupBox = QGroupBox('Build XML configuration files')
        self.xmlGroupBox.setCheckable(True)
        self.xmlGroupBox.setChecked(True)

        self.createDataFilesGroupBox()
        self.createleaseDurationBox()
        self.createSimpleSettingsGroupBox()
        self.createDSSettingsGroupBox()
        self.createOtherAttributesGroupBox()

        # Set layout
        layout = QGridLayout()
        layout.addWidget(self.dataFilesGroupBox, 0, 0, 1, 2)
        layout.addWidget(self.simpleSettingsGroupBox, 1, 0, 1, 1)
        layout.addWidget(self.dsSettingsGroupBox, 1, 1, 1, 1)
        layout.addWidget(self.otherAttributesGroupBox, 2, 0, 1, 1)
        layout.addWidget(self.leaseDurationBox, 2, 1, 1, 1)
        layout.setRowStretch(1, 1)
        layout.setRowStretch(2, 1)
        layout.setRowStretch(3, 1)
        self.xmlGroupBox.setLayout(layout)

    def createOtherAttributesGroupBox(self):
        self.otherAttributesGroupBox = QGroupBox(
            'Other discovery settings')

        self.transportProtocol = QComboBox()
        self.transportProtocol.addItems(['UDPv4', 'UDPv6', 'TCPv4', 'TCPv6'])
        transportProtocolLabel = QLabel('Transport Protocol:')
        transportProtocolLabel.setBuddy(self.transportProtocol)

        layout = QGridLayout()
        layout.setAlignment(Qt.AlignTop)
        layout.addWidget(transportProtocolLabel, 0, 0)
        layout.addWidget(self.transportProtocol, 0, 1)
        self.otherAttributesGroupBox.setLayout(layout)

    def createSimpleSettingsGroupBox(self):
        self.simpleSettingsGroupBox = QGroupBox(
            'SIMPLE Discovery Configuration')

        # Participants
        self.participants = QSpinBox(self.simpleSettingsGroupBox)
        self.participants.setValue(1)
        participantsLabel = QLabel('Participants:')
        participantsLabel.setBuddy(self.participants)

        # Publishers
        self.simplePubs = QSpinBox(self.simpleSettingsGroupBox)
        self.simplePubs.setValue(5)
        simplePubsLabel = QLabel('Publishers:')
        simplePubsLabel.setBuddy(self.simplePubs)

        # Subscribers
        self.simpleSubs = QSpinBox(self.simpleSettingsGroupBox)
        self.simpleSubs.setValue(5)
        simpleSubsLabel = QLabel('Subscribers:')
        simpleSubsLabel.setBuddy(self.simpleSubs)

        layout = QGridLayout()
        layout.addWidget(participantsLabel, 0, 0)
        layout.addWidget(self.participants, 0, 1)
        layout.addWidget(simplePubsLabel, 1, 0)
        layout.addWidget(self.simplePubs, 1, 1)
        layout.addWidget(simpleSubsLabel, 2, 0)
        layout.addWidget(self.simpleSubs, 2, 1)
        self.simpleSettingsGroupBox.setLayout(layout)

    def createDSSettingsGroupBox(self):
        self.dsSettingsGroupBox = QGroupBox(
            'Discovery Server Configuration')

        # Servers
        self.servers = QSpinBox(self.dsSettingsGroupBox)
        self.servers.setValue(1)
        serversLabel = QLabel('Servers:')
        serversLabel.setBuddy(self.servers)

        # Clients
        self.clients = QSpinBox(self.dsSettingsGroupBox)
        self.clients.setValue(1)
        clientsLabel = QLabel('Clients:')
        clientsLabel.setBuddy(self.clients)

        # Publishers
        self.dsPubs = QSpinBox(self.dsSettingsGroupBox)
        self.dsPubs.setValue(5)
        dsPubsLabel = QLabel('Publishers:')
        dsPubsLabel.setBuddy(self.dsPubs)

        # Subscribers
        self.dsSubs = QSpinBox(self.dsSettingsGroupBox)
        self.dsSubs.setValue(5)
        dsSubsLabel = QLabel('Subscribers:')
        dsSubsLabel.setBuddy(self.dsSubs)

        layout = QGridLayout()
        layout.addWidget(serversLabel, 0, 0)
        layout.addWidget(self.servers, 0, 1)
        layout.addWidget(clientsLabel, 1, 0)
        layout.addWidget(self.clients, 1, 1)
        layout.addWidget(dsPubsLabel, 2, 0)
        layout.addWidget(self.dsPubs, 2, 1)
        layout.addWidget(dsSubsLabel, 3, 0)
        layout.addWidget(self.dsSubs, 3, 1)
        self.dsSettingsGroupBox.setLayout(layout)

    def browseCSVFiles(self, lineEdit):
        """."""
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        fileName, _ = QFileDialog.getOpenFileName(
            None,
            'QFileDialog.getOpenFileName()',
            '',
            'CSV Files (*.csv);;XML Schema (*.xsd)',
            options=options
        )
        if fileName:
            lineEdit.setText(fileName)

    def loadTestTypes(self):

        # TODO: Validate the path
        dialog = testTypes(self)
        if dialog.exec_():
            self.dfTestCases = dialog.finalTestCasesList
        else:
            try:
                self.dfTestCases
            except AttributeError:
                self.dfTestCases = pd.read_csv(self.testsDataFile.text())

    def createDataFilesGroupBox(self):
        self.dataFilesGroupBox = QGroupBox('Data Files')

        # RPis data
        self.rpisDataFile = QLineEdit()
        self.rpisDataFile.setText('./resources/RPi_IPs.csv')
        rpisDataLabel = QLabel('RPis Data:')
        rpisDataLabel.setBuddy(self.rpisDataFile)
        self.rpisDataButton = QPushButton('Browse')

        self.rpisDataButton.clicked.connect(
            lambda: self.browseCSVFiles(self.rpisDataFile))

        # Test cases data
        self.testsDataFile = QLineEdit()
        self.testsDataFile.setText('./resources/Test_Types.csv')
        testsDataLabel = QLabel('Test Cases Data:')
        testsDataLabel.setBuddy(self.testsDataFile)
        self.testsDataButton = QPushButton('Browse')
        self.testsDataLoadButton = QPushButton('Load')

        self.testsDataButton.clicked.connect(
            lambda: self.browseCSVFiles(self.testsDataFile))

        self.testsDataLoadButton.clicked.connect(self.loadTestTypes)

        # XML Schema for XML validation
        self.xsdFile = QLineEdit()
        self.xsdFile.setText('./resources/discovery-server.xsd')
        xsdFileLabel = QLabel('XML Schema:')
        xsdFileLabel.setBuddy(self.xsdFile)
        self.xsdFileButton = QPushButton('Browse')

        self.xsdFileButton.clicked.connect(
            lambda: self.browseCSVFiles(self.xsdFile))

        # Set layout
        layout = QGridLayout()
        layout.addWidget(rpisDataLabel, 0, 0)
        layout.addWidget(self.rpisDataFile, 0, 1, 1, 2)
        layout.addWidget(self.rpisDataButton, 0, 3)
        layout.addWidget(testsDataLabel, 1, 0)
        layout.addWidget(self.testsDataFile, 1, 1)
        layout.addWidget(self.testsDataButton, 1, 2)
        layout.addWidget(self.testsDataLoadButton, 1, 3)
        layout.addWidget(xsdFileLabel, 2, 0)
        layout.addWidget(self.xsdFile, 2, 1, 1, 2)
        layout.addWidget(self.xsdFileButton, 2, 3)
        self.dataFilesGroupBox.setLayout(layout)

    def enableLeaseDuration(self):
        if (self.leaseDuration.currentText() == 'DURATION_INFINITY'):
            self.secLeaseDuration.setDisabled(True)
            self.nanosecLeaseDuration.setDisabled(True)
        else:
            self.secLeaseDuration.setDisabled(False)
            self.nanosecLeaseDuration.setDisabled(False)

    def createleaseDurationBox(self):
        self.leaseDurationBox = QGroupBox('leaseDuration')
        self.leaseDurationBox.setCheckable(True)
        self.leaseDurationBox.setChecked(True)

        self.leaseDuration = QComboBox()
        self.leaseDuration.addItems(['DURATION_INFINITY', 'Duration_t'])

        self.secLeaseDuration = QSpinBox(self.leaseDurationBox)
        self.secLeaseDuration.setValue(20)
        self.secLeaseDuration.setMaximum(1e4)
        secLeaseDurationLabel = QLabel('sec:')
        secLeaseDurationLabel.setBuddy(self.secLeaseDuration)

        self.nanosecLeaseDuration = QSpinBox(self.leaseDurationBox)
        self.nanosecLeaseDuration.setValue(0)
        self.nanosecLeaseDuration.setMaximum(1e9)
        nanosecLeaseDurationLabel = QLabel('nanosec:')
        nanosecLeaseDurationLabel.setBuddy(self.nanosecLeaseDuration)

        self.secLeaseDuration.setDisabled(True)
        self.nanosecLeaseDuration.setDisabled(True)

        # leaseDuration callbacks
        self.leaseDuration.activated.connect(self.enableLeaseDuration)

        layout = QGridLayout()
        layout.addWidget(self.leaseDuration, 0, 0, 1, 2)
        layout.addWidget(secLeaseDurationLabel, 1, 0)
        layout.addWidget(self.secLeaseDuration, 1, 1)
        layout.addWidget(nanosecLeaseDurationLabel, 2, 0)
        layout.addWidget(self.nanosecLeaseDuration, 2, 1)
        self.leaseDurationBox.setLayout(layout)

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
    gallery = configFilesApp()
    gallery.show()
    sys.exit(app.exec_())
