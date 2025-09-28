#include "cmycombobox_uart.h"

// : QComboBox(parent) 이 코드가 없으면 위젯이 표시되지 않음.
// 이유는 추후에 공부 할 것.
CMyComboBox_UART::CMyComboBox_UART(QWidget *parent): QComboBox(parent)
{
    m_parent = parent;
}

/*
    override methods
*/
void CMyComboBox_UART::showPopup()
{
    GetSerialPortsInfo();
    QComboBox::showPopup();
}


/*
    Methods
*/
void CMyComboBox_UART::GetSerialPortsInfo()
{
    clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos)
    {
        addItem(info.portName());
    }
    setCurrentIndex(-1);
}


