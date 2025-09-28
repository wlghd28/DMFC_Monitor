#include "Graph.h"
#include "ui_Graph.h"
#include "Parent.h"
#include "ccontroller.h"

Graph::Graph(Parent *parent) :
    QDialog(parent),
    ui(new Ui::Graph),
    m_parent(parent),
{
    ui->setupUi(this);
    Init();
}

Graph::~Graph()
{
    delete ui;
}

/*
    Event Slots
*/
void Graph::on_checkBox_Graph_AutoScale_Y1_clicked(bool checked)
{
    Q_UNUSED(checked);
}

void Graph::on_checkBox_Graph_AutoScale_Y2_clicked(bool checked)
{
    Q_UNUSED(checked);
}

void Graph::on_checkBox_Graph_AutoScale_X_clicked(bool checked)
{
    Q_UNUSED(checked);
}

void Graph::on_pushButton_Graph_Clear_clicked()
{
    ClearGraph();
}

void Graph::on_pushButton_Graph_SetVisibleLegend_clicked(bool checked)
{
    ui->qcustomplotwidget_Graph0->legend->setVisible(checked);
    ui->qcustomplotwidget_Graph0->replot();
    //SetGraph(ui->comboBox_CENUM_DEFINE::GRAPH_CATEGORY->currentIndex());
}

void Graph::on_comboBox_Graph_Category_currentIndexChanged(int index)
{
    SetGraph(index);
}

void Graph::on_pushButton_Graph0_SetPeriod_clicked()
{
    m_ixAixsPeriod = ui->spinBox_Graph0_SetPeriod->value() * 60;
}


/*
    Timer slots
*/
void Graph::OnTimerUpdateGraph()
{
    UpdateGraph(ui->comboBox_Graph_Category->currentIndex());
}

/*
    custom slots
*/
void Graph::OnAxisClick_Graph0(QCPAxis* axis, QCPAxis::SelectablePart part, QMouseEvent* event)
{
    // 클릭된 축 정보 출력 예시
    qDebug() << "Axis clicked: " << axis->axisType() << ", Part: " << part;
    qDebug() << "Mouse event: " << event->x() << ", " << event->y();

    if(axis == ui->qcustomplotwidget_Graph0->yAxis)
    {
        ui->qcustomplotwidget_Graph0->axisRect()->setRangeDragAxes(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->axisRect()->setRangeZoomAxes(ui->qcustomplotwidget_Graph0->yAxis, ui->qcustomplotwidget_Graph0->yAxis);
    }

    if(axis == ui->qcustomplotwidget_Graph0->yAxis2)
    {
        ui->qcustomplotwidget_Graph0->axisRect()->setRangeDragAxes(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis2);
        ui->qcustomplotwidget_Graph0->axisRect()->setRangeZoomAxes(ui->qcustomplotwidget_Graph0->yAxis2, ui->qcustomplotwidget_Graph0->yAxis2);
    }

    if(axis == ui->qcustomplotwidget_Graph0->xAxis)
    {
        ui->qcustomplotwidget_Graph0->axisRect()->setRangeZoomAxes(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->xAxis);
    }
}

void Graph::OnAxisDoubleClick_Graph0(QCPAxis* axis, QCPAxis::SelectablePart part, QMouseEvent* event)
{
    // 클릭된 축 정보 출력 예시
    qDebug() << "Axis clicked: " << axis->axisType() << ", Part: " << part;
    qDebug() << "Mouse event: " << event->x() << ", " << event->y();

    if(axis == ui->qcustomplotwidget_Graph0->yAxis)
    {

    }

    if(axis == ui->qcustomplotwidget_Graph0->yAxis2)
    {

    }

    if(axis == ui->qcustomplotwidget_Graph0->xAxis)
    {

    }
}

/*
    override Mothods
*/
bool Graph::eventFilter(QObject* dest, QEvent* event)
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

void Graph::resizeEvent(QResizeEvent* event)
{
   QDialog::resizeEvent(event);
   // Your code here.
   //resize(event->size());
}

void Graph::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void Graph::reject()
{
    return;
}

/*
    Methods
*/
void CBOP::Init()
{
    // event setting
    installEventFilter(this);

    InitMember();
    InitWidget();
    InitGraph();
    InitTimer();
}

void CBOP::InitMember()
{
    memset(m_ArrScaledData, 0, sizeof(m_ArrScaledData));

    // Data Management
    m_TimerClock_SaveBOPData = 1000;
}

void CBOP::InitWidget()
{
    // textbrowser setting
    ui->textBrowser_Status->setUndoRedoEnabled(false);
    ui->textBrowser_Status->document()->setMaximumBlockCount(300);
}

void CBOP::InitTimer()
{
    m_nowActionStartTime = QTime(0, 0, 0);
    m_pTimer_UpdateGraph = std::make_shared<QTimer>();
    connect(m_pTimer_UpdateGraph.get(), SIGNAL(timeout()), this, SLOT(OnTimerUpdateGraph()));
}

void CBOP::StartTimer()
{
    if(m_parent->m_Controller->GetCheckNetworkConnection())
    {
        m_pTimer_UpdateGraph->start(1000);
    }
}

void CBOP::StopTimer()
{
    m_pTimer_UpdateGraph->stop();
}

void CBOP::InitGraph()
{
    // BOP Graph0
    ui->qcustomplotwidget_Graph0->axisRect()->setRangeDragAxes(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
    ui->qcustomplotwidget_Graph0->axisRect()->setRangeZoomAxes(ui->qcustomplotwidget_Graph0->yAxis, ui->qcustomplotwidget_Graph0->yAxis);
    ui->qcustomplotwidget_Graph0->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectAxes);
    ui->qcustomplotwidget_Graph0->setBackground(QBrush(Qt::black));
    ui->qcustomplotwidget_Graph0->legend->setTextColor(QColor(255, 255, 255));
    ui->qcustomplotwidget_Graph0->legend->setBrush(QBrush(Qt::black));
    ui->qcustomplotwidget_Graph0->xAxis->setVisible(true);
    ui->qcustomplotwidget_Graph0->yAxis->setVisible(true);
    ui->qcustomplotwidget_Graph0->legend->setVisible(false);
    ui->qcustomplotwidget_Graph0->xAxis->setLabel("Real Time(s)");
    ui->qcustomplotwidget_Graph0->xAxis->setLabelColor(Qt::white);
    ui->qcustomplotwidget_Graph0->xAxis->setBasePen(QPen(Qt::white));
    ui->qcustomplotwidget_Graph0->xAxis->setTickLabelColor(Qt::white);
    ui->qcustomplotwidget_Graph0->axisRect()->addAxis(QCPAxis::atRight);
    ui->qcustomplotwidget_Graph0->axisRect()->axis(QCPAxis::atRight, 1)->setLabel("Pump");
    ui->qcustomplotwidget_Graph0->axisRect()->axis(QCPAxis::atRight, 1)->setLabelColor(Qt::white);
    ui->qcustomplotwidget_Graph0->axisRect()->axis(QCPAxis::atRight, 1)->setBasePen(QPen(Qt::white));
    ui->qcustomplotwidget_Graph0->axisRect()->axis(QCPAxis::atRight, 1)->setTickLabels(true);
    ui->qcustomplotwidget_Graph0->axisRect()->axis(QCPAxis::atRight, 1)->setTickLabelColor(Qt::white);
    ui->qcustomplotwidget_Graph0->axisRect()->axis(QCPAxis::atRight, 1)->setRange(-4, 4);
    ui->qcustomplotwidget_Graph0->axisRect()->axis(QCPAxis::atRight, 1)->setVisible(false);
    //ui->qcustomplotwidget_Graph0->xAxis->setRange(0, 60);

    // Init BOP Graph0 ComboBox
    ui->comboBox_Graph_Category->lineEdit()->setReadOnly(true);
    ui->comboBox_Graph_Category->lineEdit()->setAlignment(Qt::AlignCenter);
    ui->comboBox_Graph_Category->setItemDelegate(new CenterDelegate(ui->comboBox_Graph_Category));
    ui->comboBox_Graph_Category->addItem("Stack1");
    ui->comboBox_Graph_Category->addItem("Stack2");
    ui->comboBox_Graph_Category->addItem("Battery");
    ui->comboBox_Graph_Category->addItem("Fuel");
    ui->comboBox_Graph_Category->addItem("Cooling");
    ui->comboBox_Graph_Category->setCurrentIndex(3);

    QFont font;
    font.setBold(true);
    font.setPointSize(6);
    font.setFamily(QString("D2Coding"));
    ui->qcustomplotwidget_Graph0->yAxis->setLabelFont(font);
    ui->qcustomplotwidget_Graph0->yAxis->setTickLabelFont(font);
    ui->qcustomplotwidget_Graph0->yAxis2->setLabelFont(font);
    ui->qcustomplotwidget_Graph0->yAxis2->setTickLabelFont(font);
    ui->qcustomplotwidget_Graph0->xAxis->setLabelFont(font);
    ui->qcustomplotwidget_Graph0->xAxis->setTickLabelFont(font);
    ui->qcustomplotwidget_Graph0->axisRect()->axis(QCPAxis::atRight, 1)->setLabelFont(font);
    ui->qcustomplotwidget_Graph0->axisRect()->axis(QCPAxis::atRight, 1)->setTickLabelFont(font);
    ui->qcustomplotwidget_Graph0->legend->setFont(font);

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm:ss");
    ui->qcustomplotwidget_Graph0->xAxis->setTicker(dateTicker);

    // QCustomPlot Signal
    // 축(axis) 클릭, 더블클릭 이벤트를 처리할 슬롯(Slot) 등록
    connect(ui->qcustomplotwidget_Graph0, SIGNAL(axisClick(QCPAxis*, QCPAxis::SelectablePart, QMouseEvent*)),
            this, SLOT(OnAxisClick_Graph0(QCPAxis*, QCPAxis::SelectablePart, QMouseEvent*)));
    connect(ui->qcustomplotwidget_Graph0, SIGNAL(axisDoubleClick(QCPAxis*, QCPAxis::SelectablePart, QMouseEvent*)),
            this, SLOT(OnAxisDoubleClick_Graph0(QCPAxis*, QCPAxis::SelectablePart, QMouseEvent*)));
}

void CBOP::SetGraph(quint8 category)
{
    switch(category)
    {
    case static_cast<quint16>(CENUM_DEFINE::GRAPH_CATEGORY::STACK):
    {
        ui->qcustomplotwidget_Graph0->clearItems();
        QCPItemText *textLabel = new QCPItemText(ui->qcustomplotwidget_Graph0);
        textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignLeft);
        textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
        textLabel->position->setCoords(0.01, 0.01); // place position at center/top of axis rect
        textLabel->setText("Stack1");
        textLabel->setColor(Qt::white);
        textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
        textLabel->setPen(QPen(Qt::black)); // show black border around text

        ui->qcustomplotwidget_Graph0->clearGraphs();
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis2);
        ui->qcustomplotwidget_Graph0->yAxis->setLabel("Curr (A) | Volt (V)");
        ui->qcustomplotwidget_Graph0->yAxis->setLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis->setBasePen(QPen(Qt::white));
        ui->qcustomplotwidget_Graph0->yAxis->setTickLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis->setRange(0, 100);
        ui->qcustomplotwidget_Graph0->yAxis2->setLabel("Temp (℃)");
        ui->qcustomplotwidget_Graph0->yAxis2->setLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis2->setBasePen(QPen(Qt::white));
        ui->qcustomplotwidget_Graph0->yAxis2->setTickLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis2->setRange(0, 100);
        ui->qcustomplotwidget_Graph0->yAxis2->setVisible(true);
        ui->qcustomplotwidget_Graph0->graph(0)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(0)->setPen(QColor(0, 255, 255));
        ui->qcustomplotwidget_Graph0->graph(0)->setName("DC/DC set curr");
        ui->qcustomplotwidget_Graph0->graph(1)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(1)->setPen(QColor(255, 255, 0));
        ui->qcustomplotwidget_Graph0->graph(1)->setName("DC/DC out volt");
        ui->qcustomplotwidget_Graph0->graph(2)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(2)->setPen(QColor(255, 0, 255));
        ui->qcustomplotwidget_Graph0->graph(2)->setName("DC/DC out curr");
        ui->qcustomplotwidget_Graph0->graph(3)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(3)->setPen(QColor(196, 0, 0));
        ui->qcustomplotwidget_Graph0->graph(3)->setName("Stack out volt");
        ui->qcustomplotwidget_Graph0->graph(4)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(4)->setPen(QColor(0, 196, 0));
        ui->qcustomplotwidget_Graph0->graph(4)->setName("Stack out curr");
        ui->qcustomplotwidget_Graph0->graph(5)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(5)->setPen(QColor(196, 196, 0));
        ui->qcustomplotwidget_Graph0->graph(5)->setName("Stack temp");
        ui->qcustomplotwidget_Graph0->replot();
    }
        break;
    case static_cast<quint16>(CENUM_DEFINE::GRAPH_CATEGORY::STACK2):
    {
        ui->qcustomplotwidget_Graph0->clearItems();
        QCPItemText *textLabel = new QCPItemText(ui->qcustomplotwidget_Graph0);
        textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignLeft);
        textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
        textLabel->position->setCoords(0.01, 0.01); // place position at center/top of axis rect
        textLabel->setText("Stack2");
        textLabel->setColor(Qt::white);
        textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
        textLabel->setPen(QPen(Qt::black)); // show black border around text

        ui->qcustomplotwidget_Graph0->clearGraphs();
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis2);
        ui->qcustomplotwidget_Graph0->yAxis->setLabel("Curr (A) | Volt (V)");
        ui->qcustomplotwidget_Graph0->yAxis->setLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis->setBasePen(QPen(Qt::white));
        ui->qcustomplotwidget_Graph0->yAxis->setTickLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis->setRange(0, 100);
        ui->qcustomplotwidget_Graph0->yAxis2->setLabel("Temp (℃)");
        ui->qcustomplotwidget_Graph0->yAxis2->setLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis2->setBasePen(QPen(Qt::white));
        ui->qcustomplotwidget_Graph0->yAxis2->setTickLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis2->setRange(0, 100);
        ui->qcustomplotwidget_Graph0->yAxis2->setVisible(true);
        ui->qcustomplotwidget_Graph0->graph(0)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(0)->setPen(QColor(0, 255, 255));
        ui->qcustomplotwidget_Graph0->graph(0)->setName("DC/DC set curr");
        ui->qcustomplotwidget_Graph0->graph(1)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(1)->setPen(QColor(255, 255, 0));
        ui->qcustomplotwidget_Graph0->graph(1)->setName("DC/DC out volt");
        ui->qcustomplotwidget_Graph0->graph(2)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(2)->setPen(QColor(255, 0, 255));
        ui->qcustomplotwidget_Graph0->graph(2)->setName("DC/DC out curr");
        ui->qcustomplotwidget_Graph0->graph(3)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(3)->setPen(QColor(196, 0, 0));
        ui->qcustomplotwidget_Graph0->graph(3)->setName("Stack out volt");
        ui->qcustomplotwidget_Graph0->graph(4)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(4)->setPen(QColor(0, 196, 0));
        ui->qcustomplotwidget_Graph0->graph(4)->setName("Stack out curr");
        ui->qcustomplotwidget_Graph0->graph(5)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(5)->setPen(QColor(196, 196, 0));
        ui->qcustomplotwidget_Graph0->graph(5)->setName("Stack temp");
        ui->qcustomplotwidget_Graph0->replot();
    }
        break;

    case static_cast<quint16>(CENUM_DEFINE::GRAPH_CATEGORY::BATTERY):
    {
        ui->qcustomplotwidget_Graph0->clearItems();
        QCPItemText *textLabel = new QCPItemText(ui->qcustomplotwidget_Graph0);
        textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignLeft);
        textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
        textLabel->position->setCoords(0.01, 0.01); // place position at center/top of axis rect
        textLabel->setText("Battery");
        textLabel->setColor(Qt::white);
        textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
        textLabel->setPen(QPen(Qt::black)); // show black border around text

        ui->qcustomplotwidget_Graph0->clearGraphs();
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->yAxis->setLabel("Curr (A) | Volt (V)");
        ui->qcustomplotwidget_Graph0->yAxis->setLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis->setBasePen(QPen(Qt::white));
        ui->qcustomplotwidget_Graph0->yAxis->setTickLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis->setRange(0, 100);
        ui->qcustomplotwidget_Graph0->yAxis2->setVisible(false);
        ui->qcustomplotwidget_Graph0->graph(0)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(0)->setPen(QColor(0, 255, 0));
        ui->qcustomplotwidget_Graph0->graph(0)->setName("Charge curr");
        ui->qcustomplotwidget_Graph0->graph(1)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(1)->setPen(QColor(255, 0, 0));
        ui->qcustomplotwidget_Graph0->graph(1)->setName("Discharge curr");
        ui->qcustomplotwidget_Graph0->graph(2)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(2)->setPen(QColor(255, 255, 0));
        ui->qcustomplotwidget_Graph0->graph(2)->setName("Volt");
        ui->qcustomplotwidget_Graph0->replot();
    }
        break;
    case static_cast<quint16>(CENUM_DEFINE::GRAPH_CATEGORY::FUELSUPPLYSYSTEM):
    {
        ui->qcustomplotwidget_Graph0->clearItems();
        QCPItemText *textLabel = new QCPItemText(ui->qcustomplotwidget_Graph0);
        textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignLeft);
        textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
        textLabel->position->setCoords(0.01, 0.01); // place position at center/top of axis rect
        textLabel->setText("Fuel");
        textLabel->setColor(Qt::white);
        textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
        textLabel->setPen(QPen(Qt::black)); // show black border around text

        ui->qcustomplotwidget_Graph0->clearGraphs();
        QCPGraph *graph1 = ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        QCPGraph *graph2 = ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis2);
        QCPGraph *graph3 = ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->axisRect()->axis(QCPAxis::atRight, 1));
        QCPGraph *graph4 = ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->axisRect()->axis(QCPAxis::atRight, 1));
        ui->qcustomplotwidget_Graph0->yAxis->setLabel("Concentration (MMol)");
        ui->qcustomplotwidget_Graph0->yAxis->setLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis->setBasePen(QPen(Qt::white));
        ui->qcustomplotwidget_Graph0->yAxis->setTickLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis->setRange(0, 1500);
        ui->qcustomplotwidget_Graph0->yAxis2->setLabel("Temp (℃)");
        ui->qcustomplotwidget_Graph0->yAxis2->setLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis2->setBasePen(QPen(Qt::white));
        ui->qcustomplotwidget_Graph0->yAxis2->setTickLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis2->setRange(0, 60);
        ui->qcustomplotwidget_Graph0->yAxis2->setVisible(true);
        graph1->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        graph1->setPen(QColor(0, 255, 255));
        graph1->setName("Fuel concentration");
        graph2->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        graph2->setPen(QColor(255, 255, 0));
        graph2->setName("Fuel temp");
        graph3->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        graph3->setPen(QColor(0, 255, 0));
        graph3->setName("Methanol pump power");
        graph4->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        graph4->setPen(QColor(0, 0, 255));
        graph4->setName("Water pump power");
        ui->qcustomplotwidget_Graph0->replot();         
    }
        break;
    case static_cast<quint16>(CENUM_DEFINE::GRAPH_CATEGORY::COOLINGSYSTEM):
    {
        ui->qcustomplotwidget_Graph0->clearItems();
        QCPItemText *textLabel = new QCPItemText(ui->qcustomplotwidget_Graph0);
        textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignLeft);
        textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
        textLabel->position->setCoords(0.01, 0.01); // place position at center/top of axis rect
        textLabel->setText("Cooling");
        textLabel->setColor(Qt::white);
        textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
        textLabel->setPen(QPen(Qt::black)); // show black border around text

        ui->qcustomplotwidget_Graph0->clearGraphs();
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis2);
        ui->qcustomplotwidget_Graph0->addGraph(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis2);
        ui->qcustomplotwidget_Graph0->yAxis->setLabel("Temp (℃)");
        ui->qcustomplotwidget_Graph0->yAxis->setLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis->setBasePen(QPen(Qt::white));
        ui->qcustomplotwidget_Graph0->yAxis->setTickLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis->setRange(0, 100);
        ui->qcustomplotwidget_Graph0->yAxis2->setLabel("Fan power (%)");
        ui->qcustomplotwidget_Graph0->yAxis2->setLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis2->setBasePen(QPen(Qt::white));
        ui->qcustomplotwidget_Graph0->yAxis2->setTickLabelColor(Qt::white);
        ui->qcustomplotwidget_Graph0->yAxis2->setRange(0, 100);
        ui->qcustomplotwidget_Graph0->yAxis2->setVisible(true);
        ui->qcustomplotwidget_Graph0->graph(0)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(0)->setPen(QColor(255, 0, 0));
        ui->qcustomplotwidget_Graph0->graph(0)->setName("Cathode in temp");
        ui->qcustomplotwidget_Graph0->graph(1)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(1)->setPen(QColor(0, 255, 255));
        ui->qcustomplotwidget_Graph0->graph(1)->setName("Cathode out temp");
        ui->qcustomplotwidget_Graph0->graph(2)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(2)->setPen(QColor(255, 255, 0));
        ui->qcustomplotwidget_Graph0->graph(2)->setName("Anode in temp");
        ui->qcustomplotwidget_Graph0->graph(3)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(3)->setPen(QColor(0, 0, 255));
        ui->qcustomplotwidget_Graph0->graph(3)->setName("Anode out temp");
        ui->qcustomplotwidget_Graph0->graph(4)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(4)->setPen(QColor(128, 128, 255));
        ui->qcustomplotwidget_Graph0->graph(4)->setName("Cathode fan power");
        ui->qcustomplotwidget_Graph0->graph(5)->setScatterStyle((QCPScatterStyle(QCPScatterStyle::ssCircle, 1)));
        ui->qcustomplotwidget_Graph0->graph(5)->setPen(QColor(255, 128, 128));
        ui->qcustomplotwidget_Graph0->graph(5)->setName("Anode fan power");      
        ui->qcustomplotwidget_Graph0->replot();
    }
        break;

    default:
        break;
    }
}

void CBOP::UpdateGraph(quint8 category)
{
    //ui->graphwidget_Main_Test->graph(0)->addData(m_ixAixsIndex++, nRand); // 메모리가 계속증가함..

    double currentTime(QDateTime::currentDateTime().toTime_t());

    if(m_ixAixsIndex >= 3600)
    {
        m_GraphKey.removeFirst();

        m_GraphData_SetCurrDC.removeFirst();
        m_GraphData_DC_IN_Volt.removeFirst();
        m_GraphData_DC_IN_Curr.removeFirst();
        m_GraphData_DC_OUT_Volt.removeFirst();
        m_GraphData_DC_OUT_Curr.removeFirst();
        m_GraphData_TS001.removeFirst();

        m_GraphData_SetCurrDC_2.removeFirst();
        m_GraphData_DC_IN_Volt_2.removeFirst();
        m_GraphData_DC_IN_Curr_2.removeFirst();
        m_GraphData_DC_OUT_Volt_2.removeFirst();
        m_GraphData_DC_OUT_Curr_2.removeFirst();
        m_GraphData_TS001_2.removeFirst();

        m_GraphData_Battery_CurrCharge.removeFirst();
        m_GraphData_Battery_CurrDisCharge.removeFirst();
        m_GraphData_Battery_Volt.removeFirst();

        m_GraphData_MSC701_Temp.removeFirst();
        m_GraphData_MSC701_Concentration.removeFirst();
        m_GraphData_FP601.removeFirst();
        m_GraphData_WP901.removeFirst();

        m_GraphData_TS901.removeFirst();
        m_GraphData_TS902.removeFirst();
        m_GraphData_TS701.removeFirst();
        m_GraphData_TS702.removeFirst();
        m_GraphData_CF903.removeFirst();
        m_GraphData_AF703.removeFirst();

        //ui->graphwidget_Main_Test->xAxis->moveRange(1);
    }
    else
        m_ixAixsIndex++;

    //m_GraphKey.append(dbGraphTime);
    m_GraphKey.append(currentTime);

    m_GraphData_SetCurrDC.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::SETCURRDC_1)]);
    m_GraphData_DC_IN_Volt.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::VOLTDC_IN_1)]);
    m_GraphData_DC_IN_Curr.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::CURRDC_IN_1)]);
    m_GraphData_DC_OUT_Volt.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::VOLTDC_OUT_1)]);
    m_GraphData_DC_OUT_Curr.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::CURRDC_OUT_1)]);
    m_GraphData_TS001.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::STACK_TEMP_1)]);

    m_GraphData_SetCurrDC_2.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::SETCURRDC_2)]);
    m_GraphData_DC_IN_Volt_2.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::VOLTDC_IN_2)]);
    m_GraphData_DC_IN_Curr_2.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::CURRDC_IN_2)]);
    m_GraphData_DC_OUT_Volt_2.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::VOLTDC_OUT_2)]);
    m_GraphData_DC_OUT_Curr_2.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::CURRDC_OUT_2)]);
    m_GraphData_TS001_2.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::STACK_TEMP_2)]);

    m_GraphData_Battery_CurrCharge.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::BATTERY_CURRCHARGE)]);
    m_GraphData_Battery_CurrDisCharge.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::BATTERY_CURRDISCHARGE)]);
    m_GraphData_Battery_Volt.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::BATTERY_VOLT)]);

    m_GraphData_MSC701_Concentration.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::METHANOL_CONCENTRATION)]);
    m_GraphData_MSC701_Temp.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::DILUTION_TEMP)]);
    m_GraphData_FP601.append((quint8)m_parent->m_Controller->m_STC_MAIN_RecvBuf.Ctrl.PumpMethanol > 0);
    m_GraphData_WP901.append((quint8)m_parent->m_Controller->m_STC_MAIN_RecvBuf.Ctrl.PumpWater > 0);

    m_GraphData_TS901.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::CATHODE_TEMP_1)]);
    m_GraphData_TS902.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::CATHODE_TEMP_2)]);
    m_GraphData_TS701.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::ANODE_TEMP_1)]);
    m_GraphData_TS702.append(m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA::ANODE_TEMP_2)]);
    m_GraphData_CF903.append(m_parent->m_Controller->m_STC_MAIN_RecvBuf.Ctrl.FanAir);
    m_GraphData_AF703.append(m_parent->m_Controller->m_STC_MAIN_RecvBuf.Ctrl.FanFuel);

    switch(category)
    {
    case static_cast<quint16>(CENUM_DEFINE::GRAPH_CATEGORY::STACK):
    {
        ui->qcustomplotwidget_Graph0->graph(0)->setData(m_GraphKey, m_GraphData_SetCurrDC);
        ui->qcustomplotwidget_Graph0->graph(1)->setData(m_GraphKey, m_GraphData_DC_OUT_Volt);
        ui->qcustomplotwidget_Graph0->graph(2)->setData(m_GraphKey, m_GraphData_DC_OUT_Curr);
        ui->qcustomplotwidget_Graph0->graph(3)->setData(m_GraphKey, m_GraphData_DC_IN_Volt);
        ui->qcustomplotwidget_Graph0->graph(4)->setData(m_GraphKey, m_GraphData_DC_IN_Curr);
        ui->qcustomplotwidget_Graph0->graph(5)->setData(m_GraphKey, m_GraphData_TS001);
    }
        break;
    case static_cast<quint16>(CENUM_DEFINE::GRAPH_CATEGORY::STACK2):
    {
        ui->qcustomplotwidget_Graph0->graph(0)->setData(m_GraphKey, m_GraphData_SetCurrDC_2);
        ui->qcustomplotwidget_Graph0->graph(1)->setData(m_GraphKey, m_GraphData_DC_OUT_Volt_2);
        ui->qcustomplotwidget_Graph0->graph(2)->setData(m_GraphKey, m_GraphData_DC_OUT_Curr_2);
        ui->qcustomplotwidget_Graph0->graph(3)->setData(m_GraphKey, m_GraphData_DC_IN_Volt_2);
        ui->qcustomplotwidget_Graph0->graph(4)->setData(m_GraphKey, m_GraphData_DC_IN_Curr_2);
        ui->qcustomplotwidget_Graph0->graph(5)->setData(m_GraphKey, m_GraphData_TS001_2);
    }
        break;
    case static_cast<quint16>(CENUM_DEFINE::GRAPH_CATEGORY::BATTERY):
    {
        ui->qcustomplotwidget_Graph0->graph(0)->setData(m_GraphKey, m_GraphData_Battery_CurrCharge);
        ui->qcustomplotwidget_Graph0->graph(1)->setData(m_GraphKey, m_GraphData_Battery_CurrDisCharge);
        ui->qcustomplotwidget_Graph0->graph(2)->setData(m_GraphKey, m_GraphData_Battery_Volt);
    }
        break;
    case static_cast<quint16>(CENUM_DEFINE::GRAPH_CATEGORY::FUELSUPPLYSYSTEM):
    {
        ui->qcustomplotwidget_Graph0->graph(0)->setData(m_GraphKey, m_GraphData_MSC701_Concentration);
        ui->qcustomplotwidget_Graph0->graph(1)->setData(m_GraphKey, m_GraphData_MSC701_Temp);
        ui->qcustomplotwidget_Graph0->graph(2)->setData(m_GraphKey, m_GraphData_FP601);
        ui->qcustomplotwidget_Graph0->graph(3)->setData(m_GraphKey, m_GraphData_WP901);
    }
        break;
    case static_cast<quint16>(CENUM_DEFINE::GRAPH_CATEGORY::COOLINGSYSTEM):
    {
        ui->qcustomplotwidget_Graph0->graph(0)->setData(m_GraphKey, m_GraphData_TS901);
        ui->qcustomplotwidget_Graph0->graph(1)->setData(m_GraphKey, m_GraphData_TS902);
        ui->qcustomplotwidget_Graph0->graph(2)->setData(m_GraphKey, m_GraphData_TS701);
        ui->qcustomplotwidget_Graph0->graph(3)->setData(m_GraphKey, m_GraphData_TS702);
        ui->qcustomplotwidget_Graph0->graph(4)->setData(m_GraphKey, m_GraphData_CF903);
        ui->qcustomplotwidget_Graph0->graph(5)->setData(m_GraphKey, m_GraphData_AF703);
    }
        break;

    default:
        break;
    }

    if(ui->checkBox_Graph_AutoScale_Y1->isChecked()) ui->qcustomplotwidget_Graph0->yAxis->rescale();
    if(ui->checkBox_Graph_AutoScale_Y2->isChecked()) ui->qcustomplotwidget_Graph0->yAxis2->rescale();
    if(ui->checkBox_Graph_AutoScale_X->isChecked())
    {
        double xAxisStart = m_GraphKey.back() - m_ixAixsPeriod;
        xAxisStart = xAxisStart < m_GraphKey.front() ? m_GraphKey.front() : xAxisStart;
        ui->qcustomplotwidget_Graph0->xAxis->setRange(xAxisStart, m_GraphKey.back());
    }
    ui->qcustomplotwidget_Graph0->replot();
}

void CBOP::ClearGraph()
{
    m_ixAixsIndex = 0;

    m_GraphKey.clear();

    m_GraphData_SetCurrDC.clear();
    m_GraphData_DC_IN_Volt.clear();
    m_GraphData_DC_IN_Curr.clear();
    m_GraphData_DC_OUT_Volt.clear();
    m_GraphData_DC_OUT_Curr.clear();
    m_GraphData_TS001.clear();

    m_GraphData_SetCurrDC_2.clear();
    m_GraphData_DC_IN_Volt_2.clear();
    m_GraphData_DC_IN_Curr_2.clear();
    m_GraphData_DC_OUT_Volt_2.clear();
    m_GraphData_DC_OUT_Curr_2.clear();
    m_GraphData_TS001_2.clear();

    m_GraphData_Battery_CurrCharge.clear();
    m_GraphData_Battery_CurrDisCharge.clear();
    m_GraphData_Battery_Volt.clear();

    m_GraphData_MSC701_Temp.clear();
    m_GraphData_MSC701_Concentration.clear();
    m_GraphData_FP601.clear();
    m_GraphData_WP901.clear();

    m_GraphData_TS901.clear();
    m_GraphData_TS902.clear();
    m_GraphData_TS701.clear();
    m_GraphData_TS702.clear();
    m_GraphData_CF903.clear();
    m_GraphData_AF703.clear();
}
