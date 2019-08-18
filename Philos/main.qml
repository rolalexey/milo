//import QtQuick 2.9
//import QtQuick.Window 2.2
//import QtQuick.Layouts 1.12
//import QtQuick.Controls 2.2
//import Philos 1.0


import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Material 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12





Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("N-Philosophers")

    signal addPerson(string text)
    signal delPerson(int index)
    signal startDinner()
    signal stopDinner()

    Rectangle {

        anchors.fill: parent
        color: "gray";


        ColumnLayout {

            anchors.fill: parent
            anchors.margins: 20

            Frame {
                Layout.fillWidth: true
                Layout.fillHeight: true

                GridView {
                    anchors.fill: parent
                    model: PHModel
                    focus: true
                    delegate: Philosopher {

                    }
                    highlightFollowsCurrentItem: true

                    highlight: highlight
                    add: Transition {
                              NumberAnimation { properties: "x,y"; from: 100; duration: 1000 }
                          }
                }
            }

            RowLayout {
                Button {
                    text: "Add Person"
                    onClicked: {  pname.text = ""; addDialog.open(); }

                    Dialog {
                        id: addDialog

                        x: (parent.width - width) / 2
                        y: (parent.height - height) / 2
                        parent: Overlay.overlay
                        focus: true
                        modal: true
                        title: "Add Person"
                        standardButtons: Dialog.Ok | Dialog.Cancel

                        ColumnLayout {
                            spacing: 20
                            anchors.fill: parent
                            Label {
                                elide: Label.ElideRight
                                text: "Enter Philosopher's Name:"
                                Layout.fillWidth: true
                            }
                            TextField {
                                id: pname
                                focus: true
                                placeholderText: "Name"
                                Layout.fillWidth: true
                            }

                            RowLayout  {

                                Button {
                                    id: ok
                                    text:  "ok"
                                    onClicked: { addPerson(pname.text); addDialog.accept(); }
                                }
                                Button {
                                    text: "Cancel"
                                    onClicked: addDialog.close()
                                }
                            }
                        }
                    } //Dialog
                } // Button

                Button {
                    text: "Start Dinner"
                    onClicked: { startDinner(); }
                }
                Button {
                    text: "Stop Dinner"
                    onClicked: { stopDinner(); }
                }

            } //RowLayout
        }
    }
}


