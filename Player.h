#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QPixmap>
#include <Cell.h>

class Player : public QObject {
    Q_OBJECT

public:
    enum Type {
        Red,
        Blue
    };
    Q_ENUM(Type)

    virtual ~Player();
    static Player* player(Player::Type type);

    Player::Type type() const { return m_type; }
    const QString& name() const { return m_name; }
    const QPixmap& pixmap() const { return m_pixmap; }

    Player* other() const;
    void setCellAnterior(Cell* cell);
    Cell* getCellAnterior();

private:
    static Player* m_players[2];
    static QMutex& m_mutex;

    Player::Type m_type;
    QString m_name;
    QPixmap m_pixmap;

    explicit Player(Player::Type type);
    Q_DISABLE_COPY(Player);


};

#endif // PLAYER_H
