#pragma once

#include <QLineEdit>
#include <QListView>
#include <QStringList>
#include <QStringListModel>

class CompleteLineEdit : public QLineEdit {
    Q_OBJECT

signals:
    void SelectLocation();
public slots:
    void setCompleter(const QString& text, QStringList& words); // 动态的显示列表
    void completeText(const QModelIndex& index); // 点击列表中的项，使用此项自动完成输入的单词
public:
    CompleteLineEdit(QWidget* parent = 0);
protected:
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void focusOutEvent(QFocusEvent* e);
private:
    QListView* listView; // 列表
    QStringListModel* model; // 列表的model
};
