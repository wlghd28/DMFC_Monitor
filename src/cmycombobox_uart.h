#ifndef CMyComboBox_UART_H
#define CMyComboBox_UART_H

#include <QComboBox>
#include <QtSerialPort/QSerialPortInfo>

class CMyComboBox_UART : public QComboBox
{
public:
    CMyComboBox_UART(QWidget *parent = nullptr);

public: // override methods
    void showPopup() override;

public:
    // members
    QWidget* m_parent = nullptr;

    // methods
    void GetSerialPortsInfo();
};

#endif // CMyComboBox_UART_H
