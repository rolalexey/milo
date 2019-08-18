#include <QQuickWindow>

#include "tictacgame.h"
#include "cell.h"
#include <QDebug>


TicTacGame::TicTacGame(QObject *parent) :
    QObject(parent),
    m_round(0),
    m_gameStep(Cell::Xed),
    m_qmlwindow(nullptr),
    m_aimode(false)
{

    /*!
      Combinations if winning cells
      */
    m_winlines = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}};

    initBoard();
    resetScore();
}

TicTacGame::~TicTacGame()
{
    while( m_model.rowCount() )
        m_model.delItem(m_model.rowCount()-1);
}

void TicTacGame::set_qmlwindow(QQuickWindow *win)
{
    m_qmlwindow = win;

    // connect our QML signals to our C++ slot
    QObject::connect(m_qmlwindow,  SIGNAL(cellClicked(int)), this, SLOT(clickCell(int)));
    QObject::connect(m_qmlwindow,  SIGNAL(newGame()),        this, SLOT(newGame()));
    QObject::connect(m_qmlwindow,  SIGNAL(nextPlayer()),     this, SLOT(nextStep()));
    QObject::connect(m_qmlwindow,  SIGNAL(resetScore()),     this, SLOT(resetScore()));
    QObject::connect(m_qmlwindow,  SIGNAL(setAImode(bool)),  this, SLOT(setAImode(bool)));
}


void TicTacGame::startGame()
{
    resetScore();
    newGame();
    setCurrentMove(Cell::Xed);
}

void TicTacGame::newGame()
{
    // clear the field
    for(int i=0; i<9; i++)
        m_model.getItem(i)->resetCell();

    // Cross always first
    setCurrentMove(Cell::Xed);
    m_round = 1;

    // hide winline in UI
    setUIProperty("winline", "");
    invokeUImethod("showWinner", "");
}

void TicTacGame::resetScore()
{
    p1_score = 0;
    p2_score = 0;

    if(!m_qmlwindow)    return;
    setUIProperty("score_p1", 0);
    setUIProperty("score_p2", 0);
}

void TicTacGame::clickCell(int index)
{

    CellPtr cell = m_model.getItem(index);
    Cell::State st = cell->cellState();

    // Block clicks after win/draw and for occupied cells
    if(!m_round || st != Cell::Free) return ;


    cell->setState(m_gameStep);

    /*!
      if Cell wasn't occupied before click, check for Win and Draw
      */

    Cell::State win;
    int wpos;

    std::tie(win, wpos) = checkWin();


    if( win == m_gameStep )
    {
        playerWins(win, wpos);
        return;
    }


    if(win == Cell::Draw)
    {
        invokeUImethod("showWinner", tr("It's'a DRAW!"));
        return;
    }

    // MOVE goes to next player
    nextStep();
}


void TicTacGame::nextStep()
{
    m_gameStep =  (m_gameStep == Cell::Xed) ? Cell::Oed : Cell::Xed;
    setCurrentMove(m_gameStep);

    //! AI allways play for O player
    if(m_aimode && m_gameStep == Cell::Oed)
        ai_move();
}

void TicTacGame::setAImode(bool mode)
{
    m_aimode = mode;
}



QVector<Cell::State> TicTacGame::fieldState()
{
    QVector<Cell::State> state;
    for(int i=0; i<m_model.rowCount(); i++)
        state += m_model.getItem(i)->cellState();

    return state;
}

void TicTacGame::playerWins(Cell::State player, int winpos)
{
    QString winStr;

    m_round = 0;
    if(player == Cell::Xed)
    {
            setUIProperty("score_p1",  ++p1_score);
            winStr = tr("Player X WINS");
    }
    if(player == Cell::Oed)
    {
            setUIProperty("score_p2",  ++p2_score);
            winStr = tr("Player O WINS");
    }

    auto winLineUrl = QString("qrc://images/win_%1.png").arg(winpos);

    setUIProperty("winline",  winLineUrl);
    invokeUImethod("showWinner", winStr);
}



void TicTacGame::initBoard()
{
    for(int i=0; i<9; i++)
    {
        Cell* cell(new Cell());
        m_model.addItem( CellPtr( cell ) );
    }
}

void TicTacGame::setCurrentMove(Cell::State player)
{
    m_gameStep = player;

    auto curModeUrl = QString("qrc://images/o.png");;
    if(m_gameStep == Cell::Xed)
            curModeUrl = QString("qrc://images/x.png");

    setUIProperty("currentMove", curModeUrl);
}

bool TicTacGame::checkWinPossible(const WinLine &line, Cell::State player)
{
    auto state = fieldState();
    int a,b,c;
    std::tie(a,b,c) = line;


    /*!
      win is possible for line if all cells are free or occupied by player
      */
    auto Possible = []( Cell::State actual, Cell::State chk) {
        if( actual == chk || actual == Cell::Free )
            return true;
        return false;
    };

    // win possibility of player on given line
    auto canWin = ( Possible(state[a], player) && Possible(state[b], player) && Possible(state[c], player));

    return canWin;

}

void TicTacGame::ai_move()
{
    auto field = fieldState();

    // first try to make win moves
    foreach(const WinLine &wl, m_winlines)
    {
        auto canwin = checkWinPossible(wl, Cell::Oed);
        if(canwin)
        {
            int idx = std::get<0>(wl);
            if( field[ idx ] != Cell::Free )
                idx = std::get<1>(wl);
            if( field[ idx ] != Cell::Free )
                idx = std::get<2>(wl);

            clickCell(idx);
            return;
        }
    }
    // then any possible
    for(int i=0; i<field.size(); i++)
        if( field[i] == Cell::Free)
        {
            clickCell(i);
            return;
        }

}

void TicTacGame::setUIProperty(const char *prop, const QVariant &value)
{
    if(!m_qmlwindow)
    {
        qDebug() << "QML object not connected";
        return;
    }
    m_qmlwindow->setProperty(prop, value);
}

void TicTacGame::invokeUImethod(const char *prop, const QVariant & value)
{
    QVariant returnedValue;
    QMetaObject::invokeMethod(m_qmlwindow, prop,
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant, value));
}

std::tuple<Cell::State, int> TicTacGame::checkWin()
{
    Cell::State currentMove = m_gameStep;
    QVector<Cell::State> state;
    for(int i=0; i<m_model.rowCount(); i++)
        state += m_model.getItem(i)->cellState();


    int winnerPos = -1;

    //! Lambda  Check if we have a winning combination
    //! on return  winnnerPos will holds number of winning combination
    auto win_checker = [state, currentMove, &winnerPos](WinLine t)
    {
        int a,b,c;
        std::tie(a,b,c) = t;
        winnerPos++;
        if (state[a] == currentMove && state[b] == currentMove && state[c] == currentMove)
            return true;

        return false;
    };

    // Win situation
    auto result = std::find_if(m_winlines.begin(), m_winlines.end(), win_checker);
    if(result != std::end(m_winlines))
        return std::tuple<Cell::State, int>(currentMove, winnerPos);


    auto draw_checker = [state,this](WinLine t)
    {
        int a,b,c;
        std::tie(a,b,c) = t;

         // win possibility of X player
        auto Xtest = checkWinPossible(t, Cell::Xed);
        // win possibility of O player
        auto Otest = checkWinPossible(t, Cell::Oed);

        if ( Xtest || Otest )
            return true;

        // in case of Draw
        return false;
    };

    // Draw check fo all winlines
    auto count = std::count_if(m_winlines.begin(), m_winlines.end(), draw_checker);

    //! Check for Possibility of Win combination   for each player.
    //! If not found - we have a DRAW
    if(!count)
        return std::tuple<Cell::State, int>(Cell::Draw, -1);

    // No win, game continues
    return std::tuple<Cell::State, int>(Cell::Free, -1);
}

