import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import Philos 1.0
/*!

  N-Philosopher's dinner

  Philosopher delegate item


  */




Rectangle {
    ColumnLayout {
        id: lay
        spacing: 2
        Image {
            Layout.alignment: Qt.AlignCenter
            height: 50
            //        Layout.fillWidth: true
            source: model.error? "delete2.png" : model.stateurl
        }
        Label {
            Layout.alignment: Qt.AlignCenter
            //        height: 20
            Layout.fillWidth: true
            text: model.error ? "Hello from Model" : model.name
        }
        Label {
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            text: model.error ? "x" : model.state
        }
    }

    Image {
        anchors.right: lay.right
        source: "qrc://images/delete2.png"

        MouseArea {
            anchors.fill: parent
            onClicked: delPerson(index)
        }

    }
}



