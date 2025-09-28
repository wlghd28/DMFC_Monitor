#ifndef Graph_H
#define Graph_H

#include <QDialog>
#include <QTimer>
#include "qcustomplot.h"

class Parent;
namespace Ui {
class Graph;
}

class Graph : public QDialog
{
    Q_OBJECT

public:
    explicit Graph(Parent *parent);
    ~Graph();

private slots:  // event
    void on_checkBox_SetGraph_AutoScale_clicked(bool checked);
    void on_doubleSpinBox_SetGraph_LeftCell_valueChanged(double arg1);
    void on_doubleSpinBox_SetGraph_RightCell_valueChanged(double arg1);
    void on_doubleSpinBox_SetGraph_HighVolt_valueChanged(double arg1);
    void on_doubleSpinBox_SetGraph_LowVolt_valueChanged(double arg1);
    void on_pushButton_SetGraph_Set_clicked();
    void on_tableWidget_itemSelectionChanged();

private slots:  // timer
    void OnTimer_DisplayData();

private slots:  // custom
    void qcustomplotwidget_graph0_onclickedgraph(QCPAbstractPlottable*, int, QMouseEvent*);


private:    // override methods
    bool eventFilter(QObject* dest, QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent *event) override;
    void reject() override;

private:
    Ui::Graph *ui;
    Parent *m_parent;

public: // member

    // info data
    double m_InfoData_Total_Voltage;
    uint16_t m_InfoData_Minimum_Cell;
    double m_InfoData_Minimum_Voltage;
    uint16_t m_InfoData_Maximum_Cell;
    double m_InfoData_Maximum_Voltage;
    double m_InfoData_Average_Voltage;

    // scaled data
    uint8_t m_ScaledData_ModuleLen;
    uint8_t m_ScaledData_ModuleSel;
    double m_ScaledData_TempSys;
    double m_ScaledData_Temp[2];
    double m_ScaledData[EXT_MAX_LEN][CH_LEN];

    // graph
    QCPItemTracer *m_Tracer_AverageVoltage;
    QCPItemTracer *m_Tracer_MaxVoltage;
    QCPItemTracer *m_Tracer_MinVoltage;
    QCPItemText *m_TracerText_AverageVoltage;
    QCPItemText *m_TracerText_MaxVoltage;
    QCPItemText *m_TracerText_MinVoltage;
    QCPItemCurve *m_TracerArrow_MaxVoltage;
    QCPItemCurve *m_TracerArrow_MinVoltage;
    QSharedPointer<QCPAxisTickerFixed> *m_QCPBar_FixedTicker;
    QCPBars *m_QCPBar_CellVoltage;
    QVector<double> m_GraphKey_Graph0_CellVoltage_Bar;
    QVector<double> m_GraphData_Graph0_CellVoltage_Bar;

    // timer
    quint32 m_TimerClock_DisplayData;
    quint32 m_TimerClock_SaveData;
    std::shared_ptr<QTimer> m_pTimer_DisplayData;
    std::shared_ptr<QTimer> m_pTimer_SaveData;
    
public: // method
    void Init();
    void InitResolution();
    void InitMember();
    void InitWidget();
    void InitTimer();
    void StartTimer();
    void StopTimer();

    void InitGraph();
    void UpdateGraph();
    void SetGraph();
    int CalTickStepGraph(quint16 num, quint16 boundary);
    void SetVisibleGraphTracer(bool b);

    void ScalingData();
    void InfoData();
    void DisplayData();
};

#endif // Graph_H
