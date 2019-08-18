import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2

//import Ticmodel 1.0

Window {
    visible: true
    width: 700
    height: 480
    title: qsTr("TicTac")

    signal cellClicked(int idx)
    signal newGame()
    signal resetScore()
    signal nextPlayer()
    signal setAImode(bool mode)

    property string currentMove: ""
    property string winline: ""
    property int score_p1: 0
    property int score_p2: 0

    function showWinner(string)
    {
        roundmessage.text = string
    }

    Item {
        id: game


        width: display.width; height: display.height

        Dialog {
            id: winner
            standardButtons: Dialog.Ok

            Text {
                id: wintext
                anchors.fill: parent
            }
        }


        Image {
            id: boardImage
            source: "board.png"
            width: 500
            height: 400
        }


        Row {
            id: display

            Column {



                Grid {
                    id: board
                    width: boardImage.width; height: boardImage.height
                    columns: 3

                    Repeater {
                        model: Ticmodel
                        delegate: Cell {

                        }
                    }
                } // Grid
                Text {
                    id: roundmessage
                    text: "Win text"
                    font.pixelSize: 30; font.bold: true
                }
            }
            Frame {
                Column {

                    Column {

                        Text {

                            text: "Current Move"
                            font.pixelSize: 20; font.bold: true
                        }

                        Image {
                            width: 60
                            height: 60
                            source: currentMove
                        }
                    }
                    Column {
                        Text {
                            text: "Player X score"
                            font.pixelSize: 20; font.bold: true
                        }

                        Text {
                            id: player1score
                            font.pixelSize: 50; font.bold: true
                            color: "blue"
                            text: score_p1
                        }
                    }
                    Column {
                        Text {
                            text: "Player O score"
                            font.pixelSize: 20; font.bold: true
                        }

                        Text {
                            id: player2score
                            font.pixelSize: 50; font.bold: true
                            text: score_p2
                            color: "green"
                        }
                    }

                    Column {
                        spacing: 4
                        Button {
                            text: "New Game"
                            onClicked: { newGame() }
                        }
                        Button {
                            text: "Reset Score"
                            onClicked: { resetScore() }
                        }

                        Button {
                            text: "Next player"
                            onClicked: { nextPlayer() }
                        }
                        CheckBox {
                            text: "AI Player"
                            onCheckStateChanged: setAImode(checked)

                        }
                    }

                }

            }

        }

        Image {
            id: winningLine
            width: boardImage.width
            height: boardImage.height
            opacity: 50
            source: winline

        }
    } //row
}

