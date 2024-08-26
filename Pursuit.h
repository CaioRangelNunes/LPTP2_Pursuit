#ifndef PURSUIT_H
#define PURSUIT_H


#include <QMainWindow>
#include <QMessageBox>

class Cell;
class Player;

QT_BEGIN_NAMESPACE
namespace Ui { class Pursuit; }
QT_END_NAMESPACE

class Pursuit : public QMainWindow {
    Q_OBJECT

public:
    Pursuit(QWidget *parent = nullptr);
    virtual ~Pursuit();


signals:
    void turnEnded();
    void positionChanged();


private:
    bool boolean=true;
    int phase=1;
    Ui::Pursuit *ui;
    Player* m_player;
    Cell* m_board[7][7];
    Cell* cellAux;
    Cell* cellRed;
    Cell* cellBlue;
    int player=1;
    bool enable=false;
    bool enable2=false;
    QMessageBox msgBox;

private slots:
    void play(int id);
    void move(int id);
    void linhaDireita(int id);
    void linhaEsquerda(int id);
    void colunaBaixo(int id);
    void colunaCima(int id);
    void diagonalDbaixo(int id);
    void diagonalDCima(int id);
    void diagonalEBaixo(int id);
    void diagonalECima(int id);
    void switchPlayer();
    void reset();
    void changePosition(Cell* cell);
    void changePhase();

    void showAbout();
    void updateStatusBar();
    bool checkgamerOver(int x);
    bool isVizinhos(int row, int col);
    bool isCheckVizinho(int row, int col);
    void VizinhosBloked(int row, int col);
    void remove(int id);
    void resetPlayable();
    void endGame(Player* player);
};

#endif // PURSUIT_H
