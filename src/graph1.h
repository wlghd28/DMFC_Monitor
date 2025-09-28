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
    void on_checkBox_Graph_AutoScale_Y1_clicked(bool checked);
    void on_checkBox_Graph_AutoScale_Y2_clicked(bool checked);
    void on_checkBox_Graph_AutoScale_X_clicked(bool checked);
    void on_pushButton_Graph_Clear_clicked();
    void on_pushButton_Graph_SetVisibleLegend_clicked(bool checked);
    void on_comboBox_Graph_Category_currentIndexChanged(int index);   
    void on_pushButton_Graph0_SetPeriod_clicked();

private slots:  // timer
    void OnTimerUpdateGraph();

private slots:  // custom
    void OnAxisClick_Graph0(QCPAxis* axis, QCPAxis::SelectablePart part, QMouseEvent* event);
    void OnAxisDoubleClick_Graph0(QCPAxis* axis, QCPAxis::SelectablePart part, QMouseEvent* event);


private:    // override methods
    bool eventFilter(QObject* dest, QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent *event) override;
    void reject() override;

private:
    Ui::Graph *ui;
    Parent *m_parent;

public: // member
    double m_ArrScaledData[static_cast<quint16>(CENUM_DEFINE::SCALEDDATA_BOP::LEN)];
    
    // qCustomplot Graph
    std::shared_ptr<QTimer> m_pTimer_UpdateGraph;
    quint64  m_ixAixsIndex = 0;
    quint64  m_ixAixsPeriod = 60;

    QVector<double> m_GraphKey;

    QVector<double> m_GraphData_SetCurrDC;
    QVector<double> m_GraphData_DC_IN_Volt;
    QVector<double> m_GraphData_DC_IN_Curr;
    QVector<double> m_GraphData_DC_OUT_Volt;
    QVector<double> m_GraphData_DC_OUT_Curr;
    QVector<double> m_GraphData_TS001;

    QVector<double> m_GraphData_SetCurrDC_2;
    QVector<double> m_GraphData_DC_IN_Volt_2;
    QVector<double> m_GraphData_DC_IN_Curr_2;
    QVector<double> m_GraphData_DC_OUT_Volt_2;
    QVector<double> m_GraphData_DC_OUT_Curr_2;
    QVector<double> m_GraphData_TS001_2;

    QVector<double> m_GraphData_Battery_CurrCharge;
    QVector<double> m_GraphData_Battery_CurrDisCharge;
    QVector<double> m_GraphData_Battery_Volt;

    QVector<double> m_GraphData_MSC701_Temp;
    QVector<double> m_GraphData_MSC701_Concentration;
    QVector<double> m_GraphData_FP601;
    QVector<double> m_GraphData_WP901;

    QVector<double> m_GraphData_TS901;
    QVector<double> m_GraphData_TS902;
    QVector<double> m_GraphData_TS701;
    QVector<double> m_GraphData_TS702;
    QVector<double> m_GraphData_CF903;
    QVector<double> m_GraphData_AF703;

public: // method
    void Init();
    void InitMember();
    void InitWidget();
    void InitTimer();
    void StartTimer();
    void StopTimer();
    void InitGraph();
    void SetGraph(quint8 category);
    void UpdateGraph(quint8 category);
    void ClearGraph();
}

#endif // Graph_H
