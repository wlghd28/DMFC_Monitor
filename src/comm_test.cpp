#include "COMM_TEST.h"
#include "ui_COMM_TEST.h"
#include "ccontroller.h"

COMM_TEST::COMM_TEST(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::COMM_TEST)
    , m_DPI_Width(1920)
    , m_DPI_Height(1080)
    , m_Controller(nullptr)
    , m_TimerIndex_SendCmdToServer(0)
    , m_TimerClock_SendCmdToServer(100)
    , m_TimerPeriod_SendCmdToServer_Status(10)
    , m_TimerCount_SendCmdToServer_Status(0)
    , m_pTimer_SendCmdToServer(nullptr)
{
    ui->setupUi(this);
    Init();
}

COMM_TEST::~COMM_TEST()
{
    delete ui;
}


/*
    Event slots
*/



/*
    Timer slots
*/
void COMM_TEST::OnTimerSendCmdToServer()
{
    switch(m_TimerIndex_SendCmdToServer)
    {
    case SER_CMD_RESET:
        m_Controller->SendCmdToServer(SER_CMD_RESET);
        break;

    case SER_CMD_SET_STATE:
        m_Controller->SendCmdToServer(SER_CMD_SET_STATE);
        m_TimerIndex_SendCmdToServer = SER_CMD_STATUS;
        m_TimerCount_SendCmdToServer_Status = 0;
        break;

    case SER_CMD_GET_INFO:
        m_Controller->SendCmdToServer(SER_CMD_GET_INFO);
        m_TimerIndex_SendCmdToServer = SER_CMD_STATUS;
        m_TimerCount_SendCmdToServer_Status = 0;
        break;

    case SER_CMD_STATUS:
        if(m_FCC->GetCheckState_AutoSend())
        {
            if(++m_TimerCount_SendCmdToServer_Status >= m_TimerPeriod_SendCmdToServer_Status)
            {
                m_Controller->SendCmdToServer(SER_CMD_STATUS);
                m_TimerCount_SendCmdToServer_Status = 0;
            }
        }
        break;

    case SER_CMD_SET_SETTING:
        m_Controller->SendCmdToServer(SER_CMD_SET_SETTING);
        m_TimerIndex_SendCmdToServer = SER_CMD_STATUS;
        m_TimerCount_SendCmdToServer_Status = 0;
        break;

    case SER_CMD_SET_BLOWER:
        m_Controller->SendCmdToServer(SER_CMD_SET_BLOWER);
        m_TimerIndex_SendCmdToServer = SER_CMD_STATUS;
        m_TimerCount_SendCmdToServer_Status = 0;
        break;

    case SER_CMD_SET_FAN_AIR:
        m_Controller->SendCmdToServer(SER_CMD_SET_FAN_AIR);
        m_TimerIndex_SendCmdToServer = SER_CMD_STATUS;
        m_TimerCount_SendCmdToServer_Status = 0;
        break;

    case SER_CMD_SET_FAN_FUEL:
        m_Controller->SendCmdToServer(SER_CMD_SET_FAN_FUEL);
        m_TimerIndex_SendCmdToServer = SER_CMD_STATUS;
        m_TimerCount_SendCmdToServer_Status = 0;
        break;

    case SER_CMD_SET_PUMP_WATER:
        m_Controller->SendCmdToServer(SER_CMD_SET_PUMP_WATER);
        m_TimerIndex_SendCmdToServer = SER_CMD_STATUS;
        m_TimerCount_SendCmdToServer_Status = 0;
        break;

    case SER_CMD_SET_FAN_DC:
        m_Controller->SendCmdToServer(SER_CMD_SET_FAN_DC);
        m_TimerIndex_SendCmdToServer = SER_CMD_STATUS;
        m_TimerCount_SendCmdToServer_Status = 0;
        break;

    case SER_CMD_SET_PUMP_METHANOL:
        m_Controller->SendCmdToServer(SER_CMD_SET_PUMP_METHANOL);
        m_TimerIndex_SendCmdToServer = SER_CMD_STATUS;
        m_TimerCount_SendCmdToServer_Status = 0;
        break;

    case SER_CMD_SET_PUMP_DILUTION:
        m_Controller->SendCmdToServer(SER_CMD_SET_PUMP_DILUTION);
        m_TimerIndex_SendCmdToServer = SER_CMD_STATUS;
        m_TimerCount_SendCmdToServer_Status = 0;
        break;

    case SER_CMD_SET_PUMP_FUEL:
        m_Controller->SendCmdToServer(SER_CMD_SET_PUMP_FUEL);
        m_TimerIndex_SendCmdToServer = SER_CMD_STATUS;
        m_TimerCount_SendCmdToServer_Status = 0;
        break;

    case SER_CMD_SET_DC_CURR:
        m_Controller->SendCmdToServer(SER_CMD_SET_DC_CURR);
        m_TimerIndex_SendCmdToServer = SER_CMD_STATUS;
        m_TimerCount_SendCmdToServer_Status = 0;
        break;

    case SER_CMD_DOWNLOAD_RDY:
        break;

    case SER_CMD_DOWNLOAD:
        break;

    case SER_CMD_DOWNLOAD_REBOOT:
        break;

    case SER_CMD_DOWNLOAD_RSP:
        break;

    default:
        break;
    }
}

/*
    Custom slots
*/



/*
    override Mothods
*/
bool COMM_TEST::eventFilter(QObject* dest, QEvent* event)
{
    switch(event->type())
    {
    case QEvent::WindowStateChange:
        break;

    default:
        break;
    }

    return QObject::eventFilter(dest, event);
}

void COMM_TEST::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   // Your code here.
   //resize(event->size());
}

void COMM_TEST::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void COMM_TEST::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu contextMenu(this); // 컨텍스트 메뉴 생성

    // 컨텍스트 메뉴에 원하는 동작 항목들 추가
    QAction *action1 = contextMenu.addAction("3840 x 2160");
    QAction *action2 = contextMenu.addAction("2560 x 1440");
    QAction *action3 = contextMenu.addAction("1920 x 1080");
//    QAction *action4 = contextMenu.addAction("1600 x 900");
//    QAction *action5 = contextMenu.addAction("1280 x 720");
//    QAction *action6 = contextMenu.addAction("1024 x 768");
//    QAction *action7 = contextMenu.addAction("800 x 600");

    // 컨텍스트 메뉴를 실행
    QAction *selectedAction = contextMenu.exec(event->globalPos());

    // 선택된 항목에 대한 처리 (필요한 경우)
    if (selectedAction == action1)
    {
        SetResolution(3840, 2160);
        return;
    }

    if (selectedAction == action2)
    {
        SetResolution(2560, 1440);
        return;
    }

    if (selectedAction == action3)
    {
        SetResolution(1920, 1080);
        return;
    }

//    if (selectedAction == action4)
//    {
//        SetResolution(1600, 900);
//        return;
//    }

//    if (selectedAction == action5)
//    {
//        SetResolution(1280, 720);
//        return;
//    }

//    if (selectedAction == action6)
//    {
//        SetResolution(1024, 768);
//        return;
//    }

//    if (selectedAction == action7)
//    {
//        SetResolution(800, 600);
//        return;
//    }
}

/*
    Methods
*/
void COMM_TEST::Init()
{
    // 빌드 날짜 프로그램 타이틀바에 표시
    this->setWindowTitle(QString::asprintf("%s (ver %s %s)", m_strName_Program.toLocal8Bit().data(), __DATE__, __TIME__));

    InitTimer();
    InitMember();
    InitWidget();
    InitResolution();
}

void COMM_TEST::InitMember()
{
    m_Controller = new CController(this);

    if(m_Controller == nullptr)
    {
        QMessageBox::warning(this, "Error", "Creating Controller was failed!!");
        exit(EXIT_FAILURE);
    }
}

void COMM_TEST::InitWidget()
{

}

void COMM_TEST::InitResolution()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen)
    {
        double width_org = m_DPI_Width, height_org = m_DPI_Height;
        double width_cur = 0, height_cur = 0;
        double wratio = 0, hratio = 0;

        QRect geometry = screen->geometry();
        width_cur  = (double)geometry.width();
        height_cur = (double)geometry.height();
        wratio = width_cur / width_org;
        hratio = height_cur / height_org;
        if(wratio < 1 || hratio < 1) return;

        QList<QWidget*> widgets = QApplication::allWidgets();
        for (QWidget *w : widgets)
        {
            QRect rect_org, rect_cur;
            rect_org = w->geometry();
            rect_cur.setLeft(qRound((double)rect_org.left() * wratio));
            rect_cur.setRight(qRound((double)rect_org.right() * wratio));
            rect_cur.setTop(qRound((double)rect_org.top() * hratio));
            rect_cur.setBottom(qRound((double)rect_org.bottom() * hratio));
            w->setGeometry(rect_cur);
        }

        m_DPI_Width = (quint16)width_cur;
        m_DPI_Height = (quint16)height_cur;
        if(m_BOP) m_BOP->LogoScaling();
    }
}

void COMM_TEST::SetResolution(int width, int height)
{
    setVisible(false);
    double width_org = m_DPI_Width, height_org = m_DPI_Height;
    double width_cur = width, height_cur = height;
    double wratio = 0, hratio = 0;

    wratio = width_cur / width_org;
    hratio = height_cur / height_org;

    QList<QWidget*> widgets = QApplication::allWidgets();
    for (QWidget *w : widgets)
    {
        QRect rect_org, rect_cur;
        rect_org = w->geometry();
        rect_cur.setLeft(qRound((double)rect_org.left() * wratio));
        rect_cur.setRight(qRound((double)rect_org.right() * wratio));
        rect_cur.setTop(qRound((double)rect_org.top() * hratio));
        rect_cur.setBottom(qRound((double)rect_org.bottom() * hratio));
        w->setGeometry(rect_cur);
    }

    m_DPI_Width = (quint16)width;
    m_DPI_Height = (quint16)height;
    setVisible(true);
    move(0, 0);
}

void COMM_TEST::InitTimer()
{
    m_pTimer_SendCmdToServer = std::make_shared<QTimer>();
    connect(m_pTimer_SendCmdToServer.get(), SIGNAL(timeout()), this, SLOT(OnTimerSendCmdToServer()));
}

void COMM_TEST::StartTimer()
{
    if(m_Controller->GetCheckNetworkConnection())
        m_pTimer_SendCmdToServer->start(m_TimerClock_SendCmdToServer);
}

void COMM_TEST::StopTimer()
{
    m_pTimer_SendCmdToServer->stop();
}

void COMM_TEST::SetTimerIndex_SendCmdToServer(quint16 cmd)
{
    m_TimerIndex_SendCmdToServer = cmd;
}