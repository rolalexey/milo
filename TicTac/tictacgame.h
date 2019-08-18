#ifndef TICTACGAME_H
#define TICTACGAME_H

#include <QObject>
#include <QVector>
#include <QSharedPointer>
#include "tic_model.h"
#include "cell.h"


typedef     std::tuple<int, int, int>   WinLine;

class QQuickWindow;

/*!
 * \brief The TicTacGame class provides a TicTacToe game engine.
 *
 * it hold all game logic. Ui of realised in QML.
 */
class TicTacGame : public QObject
{
    Q_OBJECT
public:

    explicit TicTacGame(QObject *parent = nullptr);
    virtual ~TicTacGame();

    /*!
     * \brief set_qmlwindow set Qml object for game engine.
     * \param win pointer to QQuickWindow.
     *
     * Function connects all signals/slots between c++/qml parts
     */
    void        set_qmlwindow(QQuickWindow * win);


    TicModel *       model()    { return & m_model; }


signals:

public slots:

    /*!
     * \brief startGame set UI to consistent initial state
     */
    void        startGame();

    /*!
     * \brief newGame clear game board and start new round
     */
    void        newGame();

    /*!
     * \brief resetScore clears win counters
     */
    void        resetScore();

    /*!
     * \brief clickCell performs change in game field cell
     * \param index cell number
     *
     * if cell is occupied do nothing
     */
    void        clickCell(int index);
    void        nextStep();
    void        setAImode(bool mode);

    /*!
     * \brief fieldState returns all cell state as QVector
     * \return
     */
    QVector<Cell::State>        fieldState();

protected slots:

    /*!
     * \brief playerWins handle Win of player
     * \param player - winning player
     * \param winpos - number of winning line
     */
    void        playerWins(Cell::State player, int winpos);

protected:

    /*!
     * \brief initBoard - create game cells
     */
    void        initBoard();

    /*!
     * \brief setCurrentMove set which player move. UI notified
     */
    void        setCurrentMove(Cell::State);

    /*!
     * \brief checkWinPossible check winline for WIN possibility.
     * \param line Winline typle to check
     * \param player player type
     * \return true if win for player is possible
     */
    bool        checkWinPossible(const WinLine &line, Cell::State player);


    /*!
     * \brief ai_move  make a MOVE for computer player
     */
    void        ai_move();


    /*!
     * \brief setUIProperty sets property on UI side
     * \param prop name of property
     * \param value
     */
    void        setUIProperty(const char * prop, const QVariant &value);

    /*!
     * \brief invokeUImethod invoke function on UI side
     * \param prop name of function
     * \param value parameter to pass
     */
    void        invokeUImethod(const char * prop, const QVariant &value);


private:


    /*!
     * \brief checkWin check for WIN of current player after his MOVE.
     * \return true if case of win.
     */
    std::tuple<Cell::State, int>     checkWin();

    /*!
     * \brief checkDraw check for DRAW
     * \return true when no move can lead to win
     */
    bool            checkDraw();



    //! Score counters
    int             p1_score;
    int             p2_score;

    //! delegate model
    TicModel        m_model;

    //! game round flag. Clears on game finish (win/draw).
    //! Prevents clicks after win.
    int             m_round;

    //! current player
    Cell::State     m_gameStep;


    QQuickWindow *  m_qmlwindow;

    //!     AI mode flag
    bool            m_aimode;

    //! Winlines combinations
    QVector< std::tuple<int, int, int> > m_winlines;
};

#endif // TICTACGAME_H
