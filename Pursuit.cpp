#include <QMessageBox>
#include <QSignalMapper>

#include "Pursuit.h"
#include "ui_Pursuit.h"
#include "Player.h"


Pursuit::Pursuit(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Pursuit),
      m_player(Player::player(Player::Red)) {
    ui->setupUi(this);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    QSignalMapper* map = new QSignalMapper(this);
    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 7; ++col) {
            QString cellName = QString("cell%1%2").arg(row).arg(col);
            Cell* cell = this->findChild<Cell*>(cellName);
            Q_ASSERT(cell->row() == row && cell->col() == col);
            m_board[row][col] = cell;

            int id = row * 7 + col;
            map->setMapping(cell, id);
            QObject::connect(cell, SIGNAL(clicked(bool)), map, SLOT(map()));
        }
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(play(int)));
#else
    QObject::connect(map, SIGNAL(mappedInt(int)), this, SLOT(play(int)));
#endif

    // When the turn ends, switch the player.
    QObject::connect(this, SIGNAL(turnEnded()), this, SLOT(switchPlayer()));


    // Reset the game.
    this->reset();
    this->adjustSize();
    this->setFixedSize(this->size());
}

Pursuit::~Pursuit() {
    delete ui;
}

void Pursuit::play(int id) {

    Cell* cell = m_board[id / 7][id % 7];
    switch (this->phase) {
            case 1:
        cellAux=cell;
        this->enable=false;
        this->enable2=false;
                this->move(id);

                m_board[id/7][id%7]->setPlayable(false);
                if(this->checkgamerOver(this->phase)){
                    break;
               }

                break;
            case 2:

                if(cell->isPlayable()){
                    enable=true;
                    if((player==1)){
                        this->cellAux->setPlayer(NULL);
                        cell->setPlayer(this->m_player);

                        cellAux=cellBlue;
                        cellRed=cell;
                        player++;
                    }
                    else{
                        this->cellAux->setPlayer(NULL);
                        cell->setPlayer(this->m_player);
                        cellAux=cellRed;
                        cellBlue=cell;
                        player--;
                    }
                    this->changePhase();
                    if(checkgamerOver(this->phase)){
                        break;
                    }
                    this->resetPlayable();
                    this->updateStatusBar();


                    remove(id);


                    break;
                }
                else{
                    this->phase--;
                    cellAux->click();

                    break;

                }
            case 3:
        if(this->enable){
            if(cell->isPlayable()){
                this->resetPlayable();
                if(cell->isBlocked()){
                    cell->setState(Cell::Empty);

                }
                this->changePhase();
                this->switchPlayer();
                cellAux->click();
                break;
            }
            else{

            }
            break;
        }
        break;
            default:
                Q_UNREACHABLE();
}
}
void Pursuit::resetPlayable(){
    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 7; ++col) {
            Cell* cell = m_board[row][col];
            cell->setPlayable(false);
        }
    }
}
void Pursuit:: endGame(Player * player){
    QString msg=QString("Parabens, %1 venceu").arg(player->name());
    this->msgBox.setText(msg);
    this->msgBox.exec();
    this->resetPlayable();
    this->reset();
}

void Pursuit:: changePhase(){
    this->phase++;
    if(this->phase>=4){
        this->phase=1;
    }
}
void Pursuit:: changePosition(Cell* cell){
    cell->setPlayer(m_player);
}


bool Pursuit::checkgamerOver(int x){
    bool boole=false;
    if(this->cellRed==this->cellBlue && x==3){
       // emit gamerOver();
        this->endGame(this->cellAux->player());
        return true;
    }
    else if(x==2){
        for (int row = 0; row < 7; ++row) {
            for (int col = 0; col < 7; ++col) {
                Cell* cell = m_board[row][col];
                if(cell->isPlayable()){
                    boole=true;
                }
            }
        }

        if(!boole){
            this->endGame(this->cellAux->player()->other());
            return true;
        }
    }

    return false;


}


void Pursuit::switchPlayer() {
    // Switch the player.
    m_player = m_player->other();

    // Finally, update the status bar.
    this->updateStatusBar();
}

void Pursuit::reset() {
    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 7; ++col) {
            Cell* cell = m_board[row][col];
            cell->reset();

            if ((row + col) % 2 == 0){
                cell->setState(Cell::Blocked);

            }
        }
    }

    m_player = Player::player(Player::Red);

    m_board[0][3]->setState(Cell::Blocked);
    m_board[1][3]->setPlayer(m_player);
    cellRed=m_board[1][3];

    m_board[5][3]->setPlayer(m_player->other());
    cellBlue=m_board[5][3];
    m_board[6][3]->setState(Cell::Blocked);
    this->boolean=true;
    this->phase=1;
    this->resetPlayable();
    this->updateStatusBar();
    this->player=1;
    this->play(10);
}

void Pursuit::showAbout() {
    QMessageBox::information(this, tr("About"),
        tr("Pursuit\n\nCaio Rangel Nunes - caioran65@gmail.com\nEmanuel Victor Fonseca - emanuel.fonseca81@gmail.com"));
}

void Pursuit::updateStatusBar() {
    if(boolean){
    ui->statusbar->showMessage(tr("Vez do %1 Mover")
        .arg(m_player->name()));
        boolean=false;
    }
    else if(!boolean){
        ui->statusbar->showMessage(tr("Vez do %1 Tirar")
        .arg(m_player->name()));
        boolean=true;
    }
}
void Pursuit::remove(int id) {
    this->VizinhosBloked(id/7, id%7);
    //emit turnEnded();
}

void Pursuit::move(int id) {
    //Cell* cell = m_board[id / 7][id % 7];
    
    this->linhaEsquerda(id);
    this->linhaDireita(id);
    this->colunaCima(id);
    this->colunaBaixo(id);
    this->diagonalDbaixo(id);
    this->diagonalDCima(id);
    this->diagonalEBaixo(id);
    this->diagonalECima(id);
    /*
    if(this->boolean){
        this->boolean=false;
    }else{
        this->boolean=true;
    }
    */


    this->changePhase();
    }
bool Pursuit::isCheckVizinho(int row, int col){
    if((m_board[row][col]->isBlocked())){
        return true;
    }
    return false;

}
bool Pursuit::isVizinhos(int row, int col){
    if(col>0){
        if(isCheckVizinho(row,col-1)){
            return true;
        }
        if(row>0){
            if(isCheckVizinho(row-1,col-1)){
                return true;
            }

        }
        if(row<6){
            if(isCheckVizinho(row+1,col-1)){
            return true;
            }
        }

    }
    if(col<6){
        if(isCheckVizinho(row,col+1)){
           return true;
        }
        if(row>0){
            if(isCheckVizinho(row-1,col+1)){
            return true;
            }
        }

        if(row<6){
             if(isCheckVizinho(row+1,col+1)){
                return true;
             }
        }
    }
     if(row>0){
         if(isCheckVizinho(row-1,col)){
         return true;
         }
    }
     if(row<6){
        if(isCheckVizinho(row+1,col)){
            return true;
        }
     }
    return false;
}

void Pursuit::VizinhosBloked(int row, int col){
    if(col>0){
        if(isCheckVizinho(row,col-1)){
            m_board[row][col-1]->setPlayable(true);
        }
    }
    if(col>0 && row>0){
        if(isCheckVizinho(row-1,col-1)){
            m_board[row-1][col-1]->setPlayable(true);
        }
    }
    if(row>0){
        if(isCheckVizinho(row-1,col)){
            m_board[row-1][col]->setPlayable(true);
        }
    }
    if(row<6){
        if(isCheckVizinho(row+1,col)){
            m_board[row+1][col]->setPlayable(true);
        }
    }
    if(col>0 && row<6){
        if(isCheckVizinho(row+1,col-1)){
            m_board[row+1][col-1]->setPlayable(true);
        }
    }
    if(col<6 && row>0){
        if(isCheckVizinho(row-1,col+1)){
            m_board[row-1][col+1]->setPlayable(true);
        }
    }
    if(col<6){
        if(isCheckVizinho(row,col+1)){
            m_board[row][col+1]->setPlayable(true);
        }
    }
    if(col<6 && row<6){
        if(isCheckVizinho(row+1,col+1)){
            m_board[row+1][col+1]->setPlayable(true);
        }
    }
}

void Pursuit:: linhaEsquerda(int id){
    int j=id%7 - 1;
    for( int col=j; col>=0; col--){
        if(m_board[id/7][col]->player()!=NULL){
                        m_board[id/7][col]->setPlayable(true);
                        break;
        }
        else if(m_board[id/7][col]->isBlocked()){
            if(col==(j)){
                break;
            }
            else{ 
                if(isVizinhos(id/7, col+1)){
                    m_board[id/7][col+1]->setPlayable(true);

                }
                break;
            }
        }
        if(col==0){
            if(isVizinhos(id/7, col)){
                 m_board[id/7][col]->setPlayable(true);
            }

        }
    }
    
}

void Pursuit:: linhaDireita(int id){
    int j=id%7 + 1;
    for( int col=j; col<=6; col++){
        if(m_board[id/7][col]->player()!=NULL){
            m_board[id/7][col]->setPlayable(true);
            break;
        }
        else if(m_board[id/7][col]->isBlocked()){
            if(col==(j)){
                break;
            }
                else{

                    if(isVizinhos(id/7, col-1)){
                    m_board[id/7][col-1]->setPlayable(true);


                }
                    break;

            }
        }

        if(col==6){
            if(isVizinhos(id/7, col)){
                m_board[id/7][col]->setPlayable(true);
            }
        }
    }
    
}


void Pursuit:: colunaCima(int id){
int i=id/7 - 1;
for( int row=i; row>=0; row--){
    if(m_board[row][id%7]->player()!=NULL){
                    m_board[row][id%7]->setPlayable(true);
                    break;
    }
    else if(m_board[row][id%7]->isBlocked()){
        if(row==(i)){
            break;
        }
        else{
            if(isVizinhos(row+1, id%7)){
                m_board[row+1][id%7]->setPlayable(true);

            }
            break;
        }
    }
    if(row==0){
        if(isVizinhos(row, id%7)){
             m_board[row][id%7]->setPlayable(true);
        }

    }
}

}


void Pursuit:: colunaBaixo(int id){
    int i=id/7 + 1;
    for( int row=i; row<=6; row++){
        if(m_board[row][id%7]->player()!=NULL){
                        m_board[row][id%7]->setPlayable(true);
                        break;
        }
        else if(m_board[row][id%7]->isBlocked()){
            if(row==(i)){
                break;
            }
            else{
                if(isVizinhos(row-1, id%7)){
                    m_board[row-1][id%7]->setPlayable(true);

                }
                break;
            }
        }
        if(row==6){
            if(isVizinhos(row, id%7)){
                 m_board[row][id%7]->setPlayable(true);
            }

        }
    }

    }

void Pursuit:: diagonalDCima(int id){
    int j=id%7+1;
    int i=id/7-1;
    int col=j;
    int row=i;
    for(; col<=6 && row>=0; col++ , row--){
        if(m_board[row][col]->player()!=NULL){
            m_board[row][col]->setPlayable(true);
            break;
        }
        else if(m_board[row][col]->isBlocked()){
            if(col==(j)){
                break;
            }
            else{ 
                if(isVizinhos(row+1, col-1)){
                m_board[row+1][col-1]->setPlayable(true);

                }
                break;
            }
        }

        if(col==6 || row==0){
            if(isVizinhos(row, col)){
                m_board[row][col]->setPlayable(true);
            }
        }
    }
    
    
}

void Pursuit:: diagonalDbaixo(int id){
    int j=id%7+1;
    int i=id/7+1;
    int col=j;
    int row=i;
    for( ; col<=7 && row<=6; col++, row++){
        if(m_board[row][col]->player()!=NULL){
            m_board[row][col]->setPlayable(true);
            break;
        }
        else if(m_board[row][col]->isBlocked()){
            if(col==(j)){
                break;

        }
            else{ 
                if(isVizinhos(row-1, col-1)){
                m_board[row-1][col-1]->setPlayable(true);

                }
                break;
            }
        }
        if(col==6 || row==6){
            if(isVizinhos(row, col)){
                m_board[row][col]->setPlayable(true);
            }
        }
    
    }
}
void Pursuit:: diagonalECima(int id){
    int j=id%7-1;
    int i=id/7-1;
    int col=j;
    int row=i;
    for( ; col>=0 && row>=0; col--, row--){
        if(m_board[row][col]->player()!=NULL){
            m_board[row][col]->setPlayable(true);
            break;
        }
        else if(m_board[row][col]->isBlocked()){
            if(col==(j)){
                break;
            }
            else{ 
                if(isVizinhos(row+1, col+1)){
                m_board[row+1][col+1]->setPlayable(true);

                }
                break;
            }
        }
        if(col==0 || row==0){
            if(isVizinhos(row, col)){
                m_board[row][col]->setPlayable(true);
            }
        }
    }
    
}
void Pursuit:: diagonalEBaixo(int id){
    int j=id%7-1;
    int i=id/7+1;
    int col=j;
    int row=i;
    for(; col>=0 && row<=6; col--, row++){
        if(m_board[row][col]->player()!=NULL){
            m_board[row][col]->setPlayable(true);
            break;
        }
        else if(m_board[row][col]->isBlocked()){
            if(col==(j)){
                break;
            }
         else{
            if(isVizinhos(row-1, col+1)){
                m_board[row-1][col+1]->setPlayable(true);

            }
            break;
         }
        }
        if(col==0 || row==6){
            if(isVizinhos(row, col)){
                m_board[row][col]->setPlayable(true);
            }
        }
    }
    
}
