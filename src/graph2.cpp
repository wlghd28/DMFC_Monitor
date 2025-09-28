#include "Graph.h"
#include "ui_Graph.h"
#include "Parent.h"
#include "ccontroller.h"

Graph::Graph(Parent *parent) :
    QDialog(parent),
    ui(new Ui::Graph),
    m_parent(parent),
    m_QCPBar_FixedTicker(nullptr),
    m_QCPBar_CellVoltage(nullptr),
    m_pTimer_DisplayData(nullptr),
    m_pTimer_SaveData(nullptr)
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
void Graph::on_checkBox_SetGraph_AutoScale_clicked(bool checked)
{
    if(checked)
    {

    }
    else
    {

    }
}

void Graph::on_doubleSpinBox_SetGraph_LeftCell_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    SetGraph();
}

void Graph::on_doubleSpinBox_SetGraph_RightCell_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    SetGraph();
}

void Graph::on_doubleSpinBox_SetGraph_HighVolt_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    SetGraph();
}

void Graph::on_doubleSpinBox_SetGraph_LowVolt_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    SetGraph();
}

void Graph::on_pushButton_SetGraph_Set_clicked()
{
    SetGraph();
}


void Graph::on_tableWidget_itemSelectionChanged()
{
    QItemSelectionModel* table_selection_model = ui->tableWidget->selectionModel();
    QModelIndexList table_selected_rows = table_selection_model->selectedRows();
    QCPDataSelection qcp_selection;

    for(int i = 0; i < table_selected_rows.length(); i++)
    {
        qcp_selection.addDataRange(QCPDataRange(table_selected_rows[i].row() + 1 - m_QCPBar_CellVoltage->width() / 2, table_selected_rows[i].row() + 1 + m_QCPBar_CellVoltage->width() / 2));
    }

    m_QCPBar_CellVoltage->setSelection(qcp_selection);
}

/*
    Timer slots
*/
void Graph::OnTimer_DisplayData()
{
    ScalingData();
    if(m_parent->GetTabCurrentIndex() == static_cast<quint16>(CENUM_DEFINE::TAPINDEX_DISPLAY::GRAPH))
    {
        InfoData();
        UpdateGraph();
        DisplayData();
    }
}

/*
    Custom slots
*/
void Graph::qcustomplotwidget_graph0_onclickedgraph(QCPAbstractPlottable* plotItem, int num, QMouseEvent* event)
{
    event->accept();
    double dKey = plotItem->interface1D()->dataMainKey(num);
    double dVal = plotItem->interface1D()->dataMainValue(num);

    qDebug() << "Key : " << dKey;
    qDebug() << "Value : " << dVal;

    ui->tableWidget->setCurrentCell(dKey - 1, 0);
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
void Graph::Init()
{
    //InitResolution();
    InitMember();
    InitWidget();
    InitTimer();
    InitGraph();
}

void Graph::InitResolution()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen)
    {
        double width_org = width(), height_org = height();
        double width_cur = 0, height_cur = 0;
        double wratio = 0, hratio = 0;

        QRect geometry = screen->geometry();
        width_cur  = (double)geometry.width();
        height_cur = (double)geometry.height();
        wratio = width_cur / width_org;
        hratio = height_cur / height_org;

        QList<QWidget*> widgets = QApplication::allWidgets();
        for (QWidget *w : widgets)
        {
            QRect rect_org, rect_cur;
            rect_org = w->geometry();
            rect_cur.setLeft((int)((double)rect_org.left() * wratio));
            rect_cur.setRight((int)((double)rect_org.right() * wratio));
            rect_cur.setTop((int)((double)rect_org.top() * wratio));
            rect_cur.setBottom((int)((double)rect_org.bottom() * wratio));
            w->setGeometry(rect_cur);
        }
    }
}

void Graph::InitMember()
{
    QSettings settings(m_parent->m_strName_Enterprise, m_parent->m_strName_Program);

    m_InfoData_Total_Voltage = 0;
    m_InfoData_Minimum_Cell = 0;
    m_InfoData_Minimum_Voltage = 0;
    m_InfoData_Maximum_Cell = 0;
    m_InfoData_Maximum_Voltage = 0;
    m_InfoData_Average_Voltage = 0;
    memset(m_scaledData, 0, sizeof(m_scaledData));

    m_TimerClock_DisplayData = 1000;

}

void Graph::InitWidget()
{
    // Context Menu
    //setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(customContextMenuRequest(const QPoint&)));


    // Table
    QStringList tableCVM_Header_Horizon;
    tableCVM_Header_Horizon << "Voltage (V)";
    ui->tableWidget->setHorizontalHeaderLabels(tableCVM_Header_Horizon);

    for(int i = 0; i < ui->tableWidget->rowCount() * ui->tableWidget->columnCount(); i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(""));
        ui->tableWidget->item(i, 0)->setTextAlignment(Qt::AlignCenter);
    }
}

void Graph::InitTimer()
{
    m_pTimer_DisplayData = std::make_shared<QTimer>();
    connect(m_pTimer_DisplayData.get(), SIGNAL(timeout()), this, SLOT(OnTimer_DisplayData()));

    m_pTimer_SaveData = std::make_shared<QTimer>();
    connect(m_pTimer_SaveData.get(), SIGNAL(timeout()), this, SLOT(OnTimer_SaveData()));
}

void Graph::StartTimer()
{
    if(m_parent->m_Controller->GetCheckNetworkConnection())
    {
        SetVisibleGraphTracer(true);
        m_pTimer_DisplayData->start(m_TimerClock_DisplayData);
        if(ui->checkBox_DataManagement_SaveData->isChecked()) m_pTimer_SaveData->start(m_TimerClock_SaveData);
    }
}

void Graph::StopTimer()
{
    m_pTimer_DisplayData->stop();
    m_pTimer_SaveData->stop();
}

void Graph::InitGraph()
{
    ui->qcustomplotwidget_Graph0->installEventFilter(this);

    m_QCPBar_CellVoltage = new QCPBars(ui->qcustomplotwidget_Graph0->xAxis, ui->qcustomplotwidget_Graph0->yAxis);
    m_QCPBar_CellVoltage->setAntialiased(false);
    //m_QCPBar_CellVoltage->setStackingGap(1);
    //m_QCPBar_CellVoltage->setName("Cell Voltage");
    m_QCPBar_CellVoltage->setPen(QColor(255, 255, 0));
    m_QCPBar_CellVoltage->setBrush(QColor(255, 255, 0));
    m_QCPBar_CellVoltage->setWidth(0.5);
    m_QCPBar_CellVoltage->setSelectable(QCP::SelectionType::stMultipleDataRanges);

    double barwidth = m_QCPBar_CellVoltage->width() / 2;

    // Graph0
    ui->qcustomplotwidget_Graph0->axisRect()->setRangeDrag(Qt::Vertical);
    ui->qcustomplotwidget_Graph0->axisRect()->setRangeZoom(Qt::Vertical);
    ui->qcustomplotwidget_Graph0->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectAxes|QCP::iSelectPlottables);
    ui->qcustomplotwidget_Graph0->setBackground(QBrush(Qt::black));
    ui->qcustomplotwidget_Graph0->legend->setTextColor(QColor(255, 255, 255));
    ui->qcustomplotwidget_Graph0->legend->setBrush(QBrush(Qt::black));
    ui->qcustomplotwidget_Graph0->xAxis->setVisible(true);
    ui->qcustomplotwidget_Graph0->yAxis->setVisible(true);
    ui->qcustomplotwidget_Graph0->xAxis->setLabelColor(Qt::white);
    ui->qcustomplotwidget_Graph0->xAxis->setBasePen(QPen(Qt::white));
    ui->qcustomplotwidget_Graph0->xAxis->setTickLabelColor(Qt::white);
    ui->qcustomplotwidget_Graph0->xAxis->setLabel("Cell Channel");
    ui->qcustomplotwidget_Graph0->xAxis->setPadding(20);
    ui->qcustomplotwidget_Graph0->xAxis->setRange(1 - barwidth, CH_LEN + barwidth);
    ui->qcustomplotwidget_Graph0->xAxis->setSelectableParts(QCPAxis::SelectablePart::spNone);
    ui->qcustomplotwidget_Graph0->yAxis->setLabelColor(Qt::white);
    ui->qcustomplotwidget_Graph0->yAxis->setBasePen(QPen(Qt::white));
    ui->qcustomplotwidget_Graph0->yAxis->setTickLabelColor(Qt::white);
    ui->qcustomplotwidget_Graph0->yAxis->setLabel("Volt (V)");
    ui->qcustomplotwidget_Graph0->yAxis->setPadding(20);
    ui->qcustomplotwidget_Graph0->yAxis->setRange(-2.5, 2.5);
    ui->qcustomplotwidget_Graph0->yAxis->setSelectableParts(QCPAxis::SelectablePart::spNone);
    ui->qcustomplotwidget_Graph0->yAxis2->setPadding(30);
    ui->qcustomplotwidget_Graph0->yAxis2->setVisible(true);
    ui->qcustomplotwidget_Graph0->yAxis2->setSelectedParts(QCPAxis::SelectablePart::spNone);
    ui->qcustomplotwidget_Graph0->legend->setVisible(false);

    // ItemTracer
    m_Tracer_AverageVoltage = new QCPItemTracer(ui->qcustomplotwidget_Graph0);
    m_Tracer_AverageVoltage->position->setTypeX(QCPItemPosition::ptAxisRectRatio);
    m_Tracer_AverageVoltage->position->setTypeY(QCPItemPosition::ptPlotCoords);
    m_Tracer_AverageVoltage->position->setAxisRect(ui->qcustomplotwidget_Graph0->yAxis->axisRect());
    m_Tracer_AverageVoltage->position->setCoords(0, 0);
    m_Tracer_AverageVoltage->setPen(QPen(QColor(255, 127, 0)));
    m_Tracer_AverageVoltage->setVisible(false);
    m_Tracer_MaxVoltage = new QCPItemTracer(ui->qcustomplotwidget_Graph0);
    m_Tracer_MaxVoltage->setGraph((QCPGraph*)m_QCPBar_CellVoltage);
    m_Tracer_MaxVoltage->setInterpolating(true);
    m_Tracer_MaxVoltage->setStyle(QCPItemTracer::tsCircle);
    m_Tracer_MaxVoltage->setPen(QPen(Qt::green));
    m_Tracer_MaxVoltage->setBrush(Qt::green);
    m_Tracer_MaxVoltage->setSize(7);
    m_Tracer_MaxVoltage->setVisible(false);
    m_Tracer_MinVoltage = new QCPItemTracer(ui->qcustomplotwidget_Graph0);
    m_Tracer_MinVoltage->setGraph((QCPGraph*)m_QCPBar_CellVoltage);
    m_Tracer_MinVoltage->setInterpolating(true);
    m_Tracer_MinVoltage->setStyle(QCPItemTracer::tsCircle);
    m_Tracer_MinVoltage->setPen(QPen(Qt::red));
    m_Tracer_MinVoltage->setBrush(Qt::red);
    m_Tracer_MinVoltage->setSize(7);
    m_Tracer_MinVoltage->setVisible(false);

    // TracerText
    m_TracerText_AverageVoltage = new QCPItemText(ui->qcustomplotwidget_Graph0);
    m_TracerText_AverageVoltage->setClipToAxisRect(false);
    m_TracerText_AverageVoltage->setPadding(QMargins(3, 0, 3, 0));
    m_TracerText_AverageVoltage->setBrush(QBrush(Qt::black));
    m_TracerText_AverageVoltage->setPen(QPen(Qt::black));
    m_TracerText_AverageVoltage->setColor(QColor(255, 127, 0));
    m_TracerText_AverageVoltage->setText("Avg");
    m_TracerText_AverageVoltage->setPositionAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    m_TracerText_AverageVoltage->position->setParentAnchor(m_Tracer_AverageVoltage->position);
    m_TracerText_AverageVoltage->setVisible(false);
    m_TracerText_MaxVoltage = new QCPItemText(ui->qcustomplotwidget_Graph0);
    m_TracerText_MaxVoltage->position->setType(QCPItemPosition::ptPlotCoords);
    m_TracerText_MaxVoltage->setPositionAlignment(Qt::AlignCenter|Qt::AlignTop);
    m_TracerText_MaxVoltage->position->setCoords(0, 0);
    m_TracerText_MaxVoltage->setText("Max");
    m_TracerText_MaxVoltage->setTextAlignment(Qt::AlignCenter);
    m_TracerText_MaxVoltage->setFont(QFont(font().family()));
    m_TracerText_MaxVoltage->setColor(Qt::green);
    m_TracerText_MaxVoltage->setPadding(QMargins(0, 0, 0, 0));
    m_TracerText_MaxVoltage->setVisible(false);
    m_TracerText_MinVoltage = new QCPItemText(ui->qcustomplotwidget_Graph0);
    m_TracerText_MinVoltage->position->setType(QCPItemPosition::ptPlotCoords);
    m_TracerText_MinVoltage->setPositionAlignment(Qt::AlignCenter|Qt::AlignTop);
    m_TracerText_MinVoltage->position->setCoords(0, 0);
    m_TracerText_MinVoltage->setText("Min");
    m_TracerText_MinVoltage->setTextAlignment(Qt::AlignCenter);
    m_TracerText_MinVoltage->setFont(QFont(font().family()));
    m_TracerText_MinVoltage->setColor(Qt::red);
    m_TracerText_MinVoltage->setPadding(QMargins(0, 0, 0, 0));
    m_TracerText_MinVoltage->setVisible(false);

    // TracerArrow
    m_TracerArrow_MaxVoltage = new QCPItemCurve(ui->qcustomplotwidget_Graph0);
    m_TracerArrow_MaxVoltage->start->setParentAnchor(m_TracerText_MaxVoltage->bottom);
    m_TracerArrow_MaxVoltage->startDir->setParentAnchor(m_TracerArrow_MaxVoltage->start);
    m_TracerArrow_MaxVoltage->startDir->setCoords(0, 0);
    m_TracerArrow_MaxVoltage->end->setParentAnchor(m_Tracer_MaxVoltage->position);
    m_TracerArrow_MaxVoltage->end->setCoords(0, 0);
    m_TracerArrow_MaxVoltage->endDir->setParentAnchor(m_TracerArrow_MaxVoltage->end);
    m_TracerArrow_MaxVoltage->endDir->setCoords(0, 0);
    m_TracerArrow_MaxVoltage->setHead(QCPLineEnding::esSpikeArrow);
    m_TracerArrow_MaxVoltage->setTail(QCPLineEnding(QCPLineEnding::esBar, (m_TracerText_MaxVoltage->right->pixelPosition().x() - m_TracerText_MaxVoltage->left->pixelPosition().x())));
    m_TracerArrow_MaxVoltage->setPen(QColor(Qt::green));
    m_TracerArrow_MaxVoltage->setVisible(false);
    m_TracerArrow_MinVoltage = new QCPItemCurve(ui->qcustomplotwidget_Graph0);
    m_TracerArrow_MinVoltage->start->setParentAnchor(m_TracerText_MinVoltage->top);
    m_TracerArrow_MinVoltage->startDir->setParentAnchor(m_TracerArrow_MinVoltage->start);
    m_TracerArrow_MinVoltage->startDir->setCoords(0, 0);
    m_TracerArrow_MinVoltage->end->setParentAnchor(m_Tracer_MinVoltage->position);
    m_TracerArrow_MinVoltage->end->setCoords(0, 0);
    m_TracerArrow_MinVoltage->endDir->setParentAnchor(m_TracerArrow_MinVoltage->end);
    m_TracerArrow_MinVoltage->endDir->setCoords(0, 0);
    m_TracerArrow_MinVoltage->setHead(QCPLineEnding::esSpikeArrow);
    m_TracerArrow_MinVoltage->setTail(QCPLineEnding(QCPLineEnding::esBar, (m_TracerText_MinVoltage->right->pixelPosition().x() - m_TracerText_MinVoltage->left->pixelPosition().x())));
    m_TracerArrow_MinVoltage->setPen(QColor(Qt::red));
    m_TracerArrow_MinVoltage->setVisible(false);

    // FixedTicker
    m_QCPBar_FixedTicker = new QSharedPointer<QCPAxisTickerFixed>(new QCPAxisTickerFixed);
    (*m_QCPBar_FixedTicker)->setTickStep(1);
    (*m_QCPBar_FixedTicker)->setTickCount(1);
    ui->qcustomplotwidget_Graph0->xAxis->setTicker(*m_QCPBar_FixedTicker);

    for(int i = 0; i < CH_LEN * EXT_MAX_LEN; i++) m_GraphKey_Graph0_CellVoltage_Bar.append((double)(i + 1));
    for(int i = 0; i < CH_LEN * EXT_MAX_LEN; i++) m_GraphData_Graph0_CellVoltage_Bar.append(0);

    QFont font;
    font.setBold(true);
    font.setPointSize(10);
    font.setFamily(QString("D2Coding"));
    ui->qcustomplotwidget_Graph0->yAxis->setLabelFont(font);
    ui->qcustomplotwidget_Graph0->yAxis->setTickLabelFont(font);
    ui->qcustomplotwidget_Graph0->yAxis2->setLabelFont(font);
    ui->qcustomplotwidget_Graph0->yAxis2->setTickLabelFont(font);
    ui->qcustomplotwidget_Graph0->xAxis->setLabelFont(font);
    ui->qcustomplotwidget_Graph0->xAxis->setTickLabelFont(font);
    ui->qcustomplotwidget_Graph0->legend->setFont(font);

    ui->qcustomplotwidget_Graph0->replot();

    // QCustomPlot Signal
    // 축(axis) 클릭 이벤트를 처리할 슬롯(Slot) 등록
    connect(ui->qcustomplotwidget_Graph0, SIGNAL(plottableClick(QCPAbstractPlottable*, int, QMouseEvent*)),
            this,SLOT(qcustomplotwidget_graph0_onclickedgraph(QCPAbstractPlottable*, int, QMouseEvent*)));
}

void Graph::UpdateGraph()
{
    int sin_max = m_InfoData_Maximum_Voltage >= 0 ? 1 : -1;
    int sin_min = m_InfoData_Minimum_Voltage >= 0 ? 1 : -1;

    if(ui->checkBox_SetGraph_AutoScale->isChecked())
        ui->qcustomplotwidget_Graph0->yAxis->rescale();

    for(int i = 0; i < EXT_MAX_LEN; i++)
    {
        for(int j = 0; j < CH_LEN; j++)
        {
            m_GraphData_Graph0_CellVoltage_Bar[i * CH_LEN + j] = m_scaledData[i][j];
        }
    }

    m_QCPBar_CellVoltage->setData(m_GraphKey_Graph0_CellVoltage_Bar, m_GraphData_Graph0_CellVoltage_Bar);

    m_Tracer_AverageVoltage->position->setCoords(1, m_InfoData_Average_Voltage);

    m_TracerArrow_MaxVoltage->start->setParentAnchor(m_InfoData_Maximum_Voltage >= 0 ? m_TracerText_MaxVoltage->bottom : m_TracerText_MaxVoltage->top);
    m_TracerArrow_MinVoltage->start->setParentAnchor(m_InfoData_Minimum_Voltage >= 0 ? m_TracerText_MinVoltage->bottom : m_TracerText_MinVoltage->top);
    m_TracerArrow_MaxVoltage->startDir->setCoords(0, 10 * sin_max);
    m_TracerArrow_MaxVoltage->end->setCoords(0, -5 * sin_max);
    m_TracerArrow_MaxVoltage->endDir->setCoords(0, -10 * sin_max);
    m_TracerArrow_MinVoltage->startDir->setCoords(0, 10 * sin_min);
    m_TracerArrow_MinVoltage->end->setCoords(0, -5 * sin_min);
    m_TracerArrow_MinVoltage->endDir->setCoords(0, -10 * sin_min);
    m_Tracer_MinVoltage->setGraphKey(m_InfoData_Minimum_Cell + 1);
    m_Tracer_MaxVoltage->setGraphKey(m_InfoData_Maximum_Cell + 1);
    m_TracerText_MaxVoltage->position->setPixelPosition(QPointF(m_QCPBar_CellVoltage->dataPixelPosition(m_InfoData_Maximum_Cell).x(), m_QCPBar_CellVoltage->dataPixelPosition(m_InfoData_Maximum_Cell).y() - (30 * sin_max)));
    m_TracerText_MinVoltage->position->setPixelPosition(QPointF(m_QCPBar_CellVoltage->dataPixelPosition(m_InfoData_Minimum_Cell).x(), m_QCPBar_CellVoltage->dataPixelPosition(m_InfoData_Minimum_Cell).y() - (30 * sin_min)));

    ui->qcustomplotwidget_Graph0->replot();
}

void Graph::SetGraph()
{
    double leftcell = ui->doubleSpinBox_SetGraph_LeftCell->value();
    double rightcell = ui->doubleSpinBox_SetGraph_RightCell->value();
    double lowvolt = ui->doubleSpinBox_SetGraph_LowVolt->value();
    double highvolt = ui->doubleSpinBox_SetGraph_HighVolt->value();
    double barwidth = (m_QCPBar_CellVoltage->width() / 2) * (leftcell > rightcell ? -1 : 1);
    double abscell = abs((leftcell - barwidth) - (rightcell + barwidth));
    int tickstep = CalTickStepGraph((quint16)abscell, CH_LEN);

    if(tickstep > 0) (*m_QCPBar_FixedTicker)->setTickStep(CalTickStepGraph((quint16)abscell, CH_LEN));
    else (*m_QCPBar_FixedTicker)->setTickStep(qRound(abscell / CH_LEN));

    ui->qcustomplotwidget_Graph0->xAxis->setRangeReversed(leftcell > rightcell);
    ui->qcustomplotwidget_Graph0->yAxis->setRangeReversed(lowvolt > highvolt);
    ui->qcustomplotwidget_Graph0->xAxis->setRange(leftcell - barwidth, rightcell + barwidth);
    ui->qcustomplotwidget_Graph0->yAxis->setRange(lowvolt, highvolt);
    ui->qcustomplotwidget_Graph0->replot();
}

int Graph::CalTickStepGraph(quint16 num, quint16 boundary)
{
    quint16 num_sqrt = sqrt(num);

    for(int i = 1; i <= num_sqrt; i++)
    {
        if(num % i == 0)
        {
            quint16 ret = num / i;
            if(ret <= boundary)
            {
                return i;
            }
        }
    }

    return 0;
}

void Graph::SetVisibleGraphTracer(bool b)
{
    m_Tracer_AverageVoltage->setVisible(b);
    m_TracerText_AverageVoltage->setVisible(b);
    m_Tracer_MaxVoltage->setVisible(b);
    m_Tracer_MinVoltage->setVisible(b);
    m_TracerText_MaxVoltage->setVisible(b);
    m_TracerText_MinVoltage->setVisible(b);
    m_TracerArrow_MaxVoltage->setVisible(b);
    m_TracerArrow_MinVoltage->setVisible(b);
}

void Graph::ScalingData()
{
    m_ScaledData_ModuleLen = 1;
    m_ScaledData_ModuleSel = 0;

    m_ScaledData_TempSys = (double)(m_parent->m_Controller->m_STC_MAIN_RecvBuf.Status.TempSys + 100) / (double)2;

    for(int i = 0; i < 2; i++)
    {
        m_ScaledData_Temp[i] = 0;
    }

    for(int j = 0; j < CH_LEN; j++)
    {
        m_scaledData[m_ScaledData_ModuleSel][j] = (double)m_parent->m_Controller->m_STC_MAIN_RecvBuf.Status.CVM[j] * (double)0.02;
    }
}

void Graph::InfoData()
{
    int r = 0, c = 0;
    m_InfoData_Total_Voltage = 0;
    m_InfoData_Minimum_Cell = 0;
    m_InfoData_Minimum_Voltage = m_scaledData[0][0];
    m_InfoData_Maximum_Cell = 0;
    m_InfoData_Maximum_Voltage = m_scaledData[0][0];

    for(int i = 0; i < CH_LEN * m_ScaledData_ModuleLen; i++)
    {
        r = i / CH_LEN;
        c = i % CH_LEN;

        m_InfoData_Total_Voltage += m_scaledData[r][c];

        if(m_scaledData[r][c] < m_InfoData_Minimum_Voltage)
        {
            m_InfoData_Minimum_Voltage = m_scaledData[r][c];
            m_InfoData_Minimum_Cell = r * CH_LEN + c;
        }

        if(m_scaledData[r][c] > m_InfoData_Maximum_Voltage)
        {
            m_InfoData_Maximum_Voltage = m_scaledData[r][c];
            m_InfoData_Maximum_Cell = r * CH_LEN + c;
        }
    }

    m_InfoData_Average_Voltage = m_InfoData_Total_Voltage / (double)(CH_LEN * m_ScaledData_ModuleLen);
}

void Graph::DisplayData()
{
    ui->doubleSpinBox_TotalVoltage->setValue(m_InfoData_Total_Voltage);
    ui->doubleSpinBox_MinimumChannel->setValue((double)(m_InfoData_Minimum_Cell + 1));
    ui->doubleSpinBox_MinimumVoltage->setValue(m_InfoData_Minimum_Voltage);
    ui->doubleSpinBox_MaximumChannel->setValue((double)(m_InfoData_Maximum_Cell + 1));
    ui->doubleSpinBox_MaximumVoltage->setValue(m_InfoData_Maximum_Voltage);
    ui->doubleSpinBox_AverageVoltage->setValue(m_InfoData_Average_Voltage);
    ui->doubleSpinBox_TempSys->setValue(m_ScaledData_TempSys);
    ui->doubleSpinBox_Temp0->setValue(m_ScaledData_Temp[0]);
    ui->doubleSpinBox_Temp1->setValue(m_ScaledData_Temp[1]);

    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        for(int j = 0; j < m_ScaledData_ModuleLen; j++)
        {
            ui->tableWidget->item(j * CH_LEN + i, 0)->setText(QString::asprintf("%.2lf", m_scaledData[j][i]));
        }
    }
}