#ifndef COMM_TEST_H
#define COMM_TEST_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QtGui>
#include <QMessageBox>
#include <QProcess>
#include "comm_protocol.h"

class CController;
QT_BEGIN_NAMESPACE
namespace Ui { class COMM_TEST; }
QT_END_NAMESPACE

class COMM_TEST : public QMainWindow
{
    Q_OBJECT

public:
    COMM_TEST(QWidget *parent = nullptr);
    ~COMM_TEST();

private slots:    // event slots

private slots:    // timer slots
    void OnTimerSendCmdToServer();

private:    // override methods
    bool eventFilter(QObject* dest, QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    Ui::COMM_TEST *ui;
    quint16 m_DPI_Width;
    quint16 m_DPI_Height;

public: // member
    CController *m_Controller;

    // Network
    quint16  m_TimerIndex_SendCmdToServer;
    quint16  m_TimerClock_SendCmdToServer;          // 100ms 고정
    quint16  m_TimerPeriod_SendCmdToServer_Status;  // Status Command 보내는 주기 (10 고정)
    quint16  m_TimerCount_SendCmdToServer_Status;   // Status Command 보내는 주기에 대한 카운팅
    std::shared_ptr<QTimer> m_pTimer_SendCmdToServer;

public: // method
    void Init();
    void InitMember();
    void InitWidget();
    void InitResolution();
    void SetResolution(int width, int height);
    void InitTimer();
    void StartTimer();
    void StopTimer();
};
#endif // COMM_TEST_H
