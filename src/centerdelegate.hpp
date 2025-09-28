#ifndef CENTERDELEGATE_H
#define CENTERDELEGATE_H

#include <QStyledItemDelegate>

class CenterDelegate : public QStyledItemDelegate
{
public:
    explicit CenterDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent)
    {
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        // 텍스트를 가운데 정렬하기 위해 텍스트 정렬 옵션을 설정
        opt.displayAlignment = Qt::AlignCenter;

        QStyledItemDelegate::paint(painter, opt, index);
    }
};

#endif // CENTERDELEGATE_H
