#include "Status.h"

Status::Status(QWidget* parent) : QLabel(parent),
    m_moveTime(false), m_blueTime(false),
    m_round(0){

    this->nextStatus(true);
}

Status::~Status() {
}

void Status::nextStatus(bool resetted) {
    if(resetted) this->reset();
    else this->nextRound();

    this->setText(QString("Fase de %1: vez do jogador %2")
                      .arg(this->m_moveTime ? "mover":"colocar",
                           this->m_blueTime ? "azul":"vermelho")
                  );
}

void Status::nextRound(){
    this->m_round++;
    this->m_moveTime = this->m_round > 5;
    this->m_blueTime = this->m_round % 2 == 1;
}

void Status::reset(){
    this->m_blueTime = false;
    this->m_moveTime = false;
    this->m_round = 0;
}
