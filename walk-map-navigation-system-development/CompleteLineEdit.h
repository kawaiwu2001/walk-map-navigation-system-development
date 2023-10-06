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
    void setCompleter(const QString& text, QStringList& words); // ��̬����ʾ�б�
    void completeText(const QModelIndex& index); // ����б��е��ʹ�ô����Զ��������ĵ���
public:
    CompleteLineEdit(QWidget* parent = 0);
protected:
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void focusOutEvent(QFocusEvent* e);
private:
    QListView* listView; // �б�
    QStringListModel* model; // �б��model
};
