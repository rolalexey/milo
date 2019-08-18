import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Window 2.10
import QtQuick.Dialogs.qml 1.0

Item {
    id: element
    width: 400
    height: 400

    UI {
        id: uI

        ColumnLayout {
            id: columnLayout
            anchors.fill: parent

            TabBar {
                id: tabBar
                width: 240
                height: 49
                Layout.fillWidth: true

                Row {
                    id: row
                    visible: true
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.fill: parent
                    spacing: 6

                    TabButton {
                        id: tabButton1
                        text: qsTr("Tab Button")
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    TabButton {
                        id: tabButton
                        text: qsTr("Tab Button")
                    }
                }
            }

            ColumnLayout {
                id: columnLayout1
                width: 100
                height: 100
                Layout.fillWidth: true
                Layout.fillHeight: true

                ListView {
                    id: listView
                    width: 268
                    height: 243
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    delegate: Item {
                        x: 5
                        width: 80
                        height: 40
                        Row {
                            id: row1
                            Rectangle {
                                width: 40
                                height: 40
                                color: colorCode
                            }

                            Text {
                                text: name
                                anchors.verticalCenter: parent.verticalCenter
                                font.bold: true
                            }
                            spacing: 10
                        }
                    }
                    model: ListModel {
                        ListElement {
                            name: "Grey"
                            colorCode: "grey"
                        }

                        ListElement {
                            name: "Red"
                            colorCode: "red"
                        }

                        ListElement {
                            name: "Blue"
                            colorCode: "blue"
                        }

                        ListElement {
                            name: "Green"
                            colorCode: "green"
                        }
                    }

                    ItemDelegate {
                        id: itemDelegate
                        x: 114
                        y: 102
                        text: qsTr("Item Delegate")
                    }
                }
            }
        }

        UI {
            id: uI1
        }
    }
}





/*##^## Designer {
    D{i:4;anchors_height:400;anchors_width:200;anchors_x:0;anchors_y:293}D{i:2;anchors_height:100;anchors_width:100;anchors_x:20;anchors_y:22}
}
 ##^##*/
