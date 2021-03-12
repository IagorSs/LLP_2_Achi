#include "Achi.h"
#include "ui_Achi.h"

#include <QMessageBox>
#include <QSignalMapper>

Achi::Achi(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Achi) {

    ui->setupUi(this);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    this->status = this->findChild<Status*>(QString("status"));

    // Load the pieces.
    QSignalMapper* map = new QSignalMapper(this);

    for (int id = 0; id < 9; ++id) {
        int r = id / 3;
        int c = id % 3;

        Hole* hole = this->findChild<Hole*>(QString("hole%1%2").arg(r).arg(c));
        Q_ASSERT(hole != nullptr);

        m_holes[id] = hole;
        map->setMapping(hole, id);
        QObject::connect(hole, SIGNAL(clicked()), map, SLOT(map()));
    }

    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(play(int)));

    // Compact the layout of the widgets.
    this->adjustSize();

    // Set a fixed window size.
    this->setFixedSize(this->size());
}

Achi::~Achi() {
    delete ui;
}

void Achi::play(int index) {
    Hole* hole = m_holes[index];
    Q_ASSERT(hole != nullptr);

    Hole::State playerTime = this->status->blueTime() ? Hole::BlueState : Hole::RedState;

    if(this->status->moveTime()){
        if(hole->state() == playerTime){
            this->old = hole;

            this->resetSelectable();

            for(int row = hole->row() - 1;row <= hole->row() + 1;row++){
                for(int col = hole->col() - 1;col <= hole->col() + 1;col++){
                    int currentIndex = row*3 + col;

                    if(row < 0 || row > 2 || col < 0 || col > 2) continue;
                    else if(m_holes[currentIndex]->state() == Hole::EmptyState){
                        if(index % 2 == 1 && currentIndex % 2 == 1) continue;
                        m_holes[currentIndex]->setState(Hole::SelectableState);
                    }
                }
            }
        }else if(hole->state() == Hole::SelectableState){
            hole->setState(playerTime);
            old->setState(Hole::EmptyState);
            this->resetSelectable();

            this->status->nextStatus(false);
        }else return;
    } else if(hole->state() == Hole::EmptyState){
        hole->setState(playerTime);
        this->status->nextStatus(false);
    }

    this->checkWinner();
}

void Achi::checkWinner(){
    bool hasWinner = false;
    QString* winner = new QString("Parabéns jogador ");

    for(int i=0;i<9;i+=3){
        // Checando ganhador por linha
        if(m_holes[i]->state() == m_holes[i+1]->state() && m_holes[i+1]->state() == m_holes[i+2]->state()
                && m_holes[i]->state() != Hole::SelectableState && m_holes[i]->state() != Hole::EmptyState){
            hasWinner = true;
            this->appendWinner(winner, m_holes[i]->state());
        }
    }

    // Checando ganhador por coluna
    for(int i=0;i<3;i++)
        if(m_holes[i]->state() == m_holes[i+3]->state() && m_holes[i+3]->state() == m_holes[i+6]->state()
                && m_holes[i]->state() != Hole::SelectableState && m_holes[i]->state() != Hole::EmptyState){
            hasWinner = true;
            this->appendWinner(winner, m_holes[i]->state());
        }

    // Checando ganhador por diagonal
    if(m_holes[0]->state() == m_holes[4]->state() && m_holes[4]->state() == m_holes[8]->state()
            && m_holes[0]->state() != Hole::SelectableState && m_holes[0]->state() != Hole::EmptyState){
        hasWinner = true;
        this->appendWinner(winner, m_holes[0]->state());
    } else if(m_holes[2]->state() == m_holes[4]->state() && m_holes[4]->state() == m_holes[6]->state()
              && m_holes[2]->state() != Hole::SelectableState && m_holes[2]->state() != Hole::EmptyState){
        hasWinner = true;
        this->appendWinner(winner, m_holes[2]->state());
    }

    winner->append(", você venceu");
    if(hasWinner) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Vencedor"));
        msgBox.setText(winner->simplified());

        msgBox.setIcon(QMessageBox::Information);

        QAbstractButton* restart = msgBox.addButton(tr("Reiniciar"), QMessageBox::YesRole);
        QAbstractButton* close = msgBox.addButton(tr("Fechar!"), QMessageBox::DestructiveRole);

        msgBox.exec();

        if (msgBox.clickedButton()==restart) this->reset();
        else if (msgBox.clickedButton()==close) this->close();
    }
}

void Achi::appendWinner(QString* string, Hole::State state){
    if(state == Hole::BlueState) string->append("azul");
    else string->append("vermelho");
}

void Achi::resetSelectable(){
    for(Hole* current:m_holes){
        if(current->state() == Hole::SelectableState)
            current->setState(Hole::EmptyState);
    }
}

void Achi::reset() {
    for (int id = 0; id < 9; ++id) {
        Hole* hole = m_holes[id];
        Q_ASSERT(hole != nullptr);

        m_holes[id]->reset();
    }

    status->nextStatus(true);
}

void Achi::showAbout() {
    QMessageBox::information(this, tr("Sobre"), tr("      Achi\n\nDesenvolvedores:\nIagor Sousa - iagor.penaranda@uol.com.br\nGabriel Bernalle - gabrielbbmatozinhos@gmail.com"));
}
