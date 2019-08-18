import QtQuick 2.0



Item  {
    id: container
    width:board.width/3; height: board.height/3;

    Image {
        width:boardImage.width/4; height: boardImage.height/4;
        anchors.centerIn: container.Center
        source: model.stateurl
    }

    MouseArea {
        anchors.fill: parent
        onClicked: cellClicked(index)
    }
}
