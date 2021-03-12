#ifndef ACHI_H
#define ACHI_H

#include <QMainWindow>
#include <Hole.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Achi;
}
QT_END_NAMESPACE

class Status;

class Achi : public QMainWindow {
    Q_OBJECT

public:
    Achi(QWidget *parent = nullptr);
    virtual ~Achi();

private:
    Ui::Achi *ui;
    Hole* m_holes[9];
    Hole* old;
    Status* status;
    void resetSelectable();
    void checkWinner();
    void appendWinner(QString* string, Hole::State state);

private slots:
    void play(int id);
    void reset();
    void showAbout();

};

#endif // ACHI_H
