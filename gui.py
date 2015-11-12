from PyQt4.QtGui import * 
from PyQt4.QtCore import *
from PyQt4 import QtGui 
import sys
import commands
import subprocess

global table
data1 = []

def main():  
    app 	= QApplication(sys.argv)
    table 	= QTableWidget()
    tableItem0 	= QTableWidgetItem()
    tableItem1 	= QTableWidgetItem()
    tableItem2 	= QTableWidgetItem()
    tableItem3 	= QTableWidgetItem()
    
    table.setWindowTitle("Bluetooth Pairing Demo")
    table.resize(600, 200)
    table.setRowCount(4)
    table.setColumnCount(5)
    table.setHorizontalHeaderLabels(QString("Bluetooth MAC Address; Name;RSSI; command;IP Address").split(";"))
    global data1
    data2 = []
    data3 = []
    output = commands.getoutput("hcitool scan")
    d = output.split( );
    print d
    i = 2
    for index in range(len(d) / 2 - 1):
        data1.append(d[i]);
        i = i + 1;
        item1 = QtGui.QTableWidgetItem(data1[index])
        cmd = "hcitool cc " + data1[index]
        output = commands.getoutput(cmd)
        cmd = "hcitool rssi " + data1[index]
        output = commands.getoutput(cmd)
        d1 = output.split( )
        table.setItem(index,0,item1)
        data2.append(d[i]);
        if (len(d1) > 3):
            data3.append(d1[3])
            item3 = QtGui.QTableWidgetItem(data3[index])
            table.setItem(index,2,item3)
        i = i + 1
        item2 = QtGui.QTableWidgetItem(data2[index])
        table.setItem(index,1,item2)
        btn_sell = QtGui.QPushButton('Get IP')
        btn_sell.clicked.connect(lambda : handleButtonClicked(table))
        table.setCellWidget(index,3,btn_sell)

    table.verticalHeader().setStretchLastSection(False)
    table.horizontalHeader().setStretchLastSection(False)
    table.resizeColumnsToContents()
    table.verticalHeader().setStretchLastSection(True)
    table.horizontalHeader().setStretchLastSection(True)
    table.show()
    return app.exec_()

def handleButtonClicked(table):
    global data1
    button = QtGui.qApp.focusWidget()
    # or button = self.sender()
    index = table.indexAt(button.pos())
    if index.isValid():
        print(index.row(), index.column())
        print(data1[index.row()])
        p = subprocess.Popen(['./clien', data1[index.row()]],stdout=subprocess.PIPE)
        table.setItem(index.row(),index.column() + 1,  QtGui.QTableWidgetItem(p.stdout.readline()))
        print "hello" + p.stdout.readline()
        

if __name__ == '__main__':
    main()
