import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import Client 1.0

Page {
    id: manageContactsPage
    signal navigateBack()
    Material.theme: Material.Dark
    Material.accent: Material.Purple

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Header
        ToolBar {
            id: header
            height: 56
            background: Rectangle {
                color: "#1E1E1E"
            }
            contentItem: RowLayout {
                spacing: 16
                anchors.verticalCenter: parent.verticalCenter

                // Back Button
                Button {
                    icon.source: "qrc:/src/views/assets/arrow_back.png"
                    onClicked: manageContactsPage.navigateBack()
                }

                Label {
                    text: qsTr("Manage Contacts")
                    font.pixelSize: 20
                    color: "#FFFFFF"
                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }

        // Tab Bar
        TabBar {
            id: tabBar
            Layout.fillWidth: true

            TabButton {
                text: qsTr("Send Request")
            }
            TabButton {
                text: qsTr("Sent Requests")
            }
            TabButton {
                text: qsTr("Received Requests")
            }
            TabButton {
                text: qsTr("Friends")
            }
        }

        // Stack Layout
        StackLayout {
            id: stackLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            // Send Request Tab
            ScrollView {
                ColumnLayout {
                    spacing: 16
                    anchors.margins: 24
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop

                    TextField {
                        id: sendRequestField
                        placeholderText: qsTr("Enter username")
                        Layout.fillWidth: true
                    }

                    Button {
                        text: qsTr("Send Friend Request")
                        icon.source: "qrc:/src/views/assets/person_add.png"
                        onClicked: {
                            manageContactsController.sendFriendRequest(sendRequestField.text)
                            sendRequestField.clear()
                        }
                    }
                }
            }

            // Sent Requests Tab
            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: sentRequestModel

                delegate: ItemDelegate {
                    width: parent.width
                    contentItem: RowLayout {
                        spacing: 16
                        Image {
                            source: "qrc:/src/views/assets/person_outline.png"
                            width: 40
                            height: 40
                        }
                        Label {
                            text: model.username
                            font.pixelSize: 16
                            color: "#FFFFFF"
                            Layout.fillWidth: true
                        }
                        Button {
                            text: qsTr("Cancel")
                            icon.source: "qrc:/src/views/assets/cancel.png"
                            onClicked: {
                                clientController.cancelFriendRequest(model.username)
                            }
                        }
                    }
                }
            }

            // Received Requests Tab
            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: receivedRequestModel

                delegate: ItemDelegate {
                    width: parent.width
                    contentItem: RowLayout {
                        spacing: 16
                        Image {
                            source: "qrc:/src/views/assets/person_add.png"
                            width: 40
                            height: 40
                        }
                        Label {
                            text: model.username
                            font.pixelSize: 16
                            color: "#FFFFFF"
                            Layout.fillWidth: true
                        }
                        Button {
                            text: qsTr("Accept")
                            icon.source: "qrc:/src/views/assets/check_circle.png"
                            onClicked: {
                                clientController.acceptFriendRequest(model.username)
                            }
                        }
                        Button {
                            text: qsTr("Reject")
                            icon.source: "qrc:/src/views/assets/remove_circle.png"
                            onClicked: {
                                clientController.rejectFriendRequest(model.username)
                            }
                        }
                    }
                }
            }

            // Friends Tab
            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: friendsModel

                delegate: ItemDelegate {
                    width: parent.width
                    contentItem: RowLayout {
                        spacing: 16
                        Image {
                            source: "qrc:/src/views/assets/person.png"
                            width: 40
                            height: 40
                        }
                        Label {
                            text: model.username
                            font.pixelSize: 16
                            color: "#FFFFFF"
                            Layout.fillWidth: true
                        }
                        Button {
                            text: qsTr("Delete")
                            icon.source: "qrc:/src/views/assets/delete.png"
                            onClicked: {
                                clientController.deleteFriend(model.username)
                            }
                        }
                    }
                }
            }
        }
    }

    // Models
    ListModel {
        id: sentRequestModel
        ListElement { username: "Alice" }
        ListElement { username: "Bob" }
    }

    ListModel {
        id: receivedRequestModel
        ListElement { username: "Charlie" }
        ListElement { username: "Dave" }
    }

    ListModel {
        id: friendsModel
        ListElement { username: "Eve" }
        ListElement { username: "Frank" }
    }
}