#ifndef STATUS_H
#define STATUS_H

#include <QObject>
#include <QLabel>

class Status : public QLabel {
    Q_OBJECT
    Q_PROPERTY(bool moveTime READ moveTime)
    Q_PROPERTY(bool blueTime READ blueTime)

public:
    explicit Status(QWidget* parent);
    virtual ~Status();
    void nextStatus(bool resetted);

    bool moveTime() const { return m_moveTime; }
    bool blueTime() const { return m_blueTime; }

private:
    bool m_moveTime;
    bool m_blueTime;

    int m_round;

    void reset();
    void nextRound();
};

#endif // STATUS_H
