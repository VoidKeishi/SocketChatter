import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects

Page {
    id: manageContactsPage
    signal navigateBack()
    Material.theme: Material.Dark
    Material.accent: Material.Purple

    Component.onCompleted: {
        contactViewModel.fetchFriends()
        contactViewModel.fetchSentRequests()
        contactViewModel.fetchReceivedRequests()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Header
        ToolBar {
            id: header
            height: 56
            background: Rectangle { color: Material.background }
            RowLayout {
                spacing: 16
                anchors.verticalCenter: parent.verticalCenter
                Button {
                    icon.source: "qrc:/src/views/assets/arrow_back.png"
                    onClicked: manageContactsPage.navigateBack()
                }
                Label {
                    text: qsTr("Manage Contacts")
                    font.pixelSize: 20
                    color: Material.foreground
                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }

        TabBar {
            id: tabBar
            Layout.fillWidth: true
            TabButton { text: qsTr("Send Request") }
            TabButton { text: qsTr("Received Requests") }
            TabButton { text: qsTr("Friends") }
        }

        StackLayout {
            id: stackLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            // Send Request Tab
            ColumnLayout {
                Layout.fillHeight: true
                Layout.margins: 16
                Layout.alignment: Qt.AlignTop

                TextField {
                    id: sendRequestField
                    placeholderText: qsTr("Enter username")
                    Layout.fillWidth: true
                    Layout.topMargin: 16
                }

                Button {
                    text: qsTr("Send Friend Request")
                    icon.source: "qrc:/src/views/assets/person_add.png"
                    onClicked: {
                        contactViewModel.sendFriendRequest(sendRequestField.text)
                        sendRequestField.clear()
                    }
                    Layout.leftMargin: 16
                }

                Label {
                    text: qsTr("Sent Requests")
                    font.pixelSize: 20
                    color: Material.foreground
                    Layout.leftMargin: 16
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: contactViewModel.sentRequests
                    delegate: ItemDelegate {
                        width: ListView.view.width
                        height: 60 
                        contentItem: RowLayout {
                            spacing: 16
                            Image {
                                source: "qrc:/src/views/assets/person_outline.png"
                                width: 40
                                height: 40
                                layer.enabled: true
                                layer.effect: MultiEffect {
                                    colorization: 1.0
                                    colorizationColor: Material.foreground
                                }
                            }
                            Label {
                                text: modelData
                                font.pixelSize: 16
                                color: Material.foreground
                                Layout.fillWidth: true
                            }
                            Button {
                                text: qsTr("Cancel")
                                icon.source: "qrc:/src/views/assets/cancel.png"
                                onClicked: {
                                    contactViewModel.cancelFriendRequest(modelData)
                                }
                            }
                        }
                    }
                }
            }

            // Received Requests Tab
            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: contactViewModel.receivedRequests
                delegate: ItemDelegate {
                    width: ListView.view.width
                    contentItem: RowLayout {
                        spacing: 16
                        Image {
                            source: "qrc:/src/views/assets/person_add.png"
                            width: 40
                            height: 40
                        }
                        Label {
                            text: modelData
                            font.pixelSize: 16
                            color: Material.foreground
                            Layout.fillWidth: true
                        }
                        Button {
                            text: qsTr("Accept")
                            icon.source: "qrc:/src/views/assets/check_circle.png"
                            onClicked: {
                                contactViewModel.respondFriendRequest(modelData, true)
                            }
                        }
                        Button {
                            text: qsTr("Reject")
                            icon.source: "qrc:/src/views/assets/remove_circle.png"
                            onClicked: {
                                contactViewModel.respondFriendRequest(modelData, false)
                            }
                        }
                    }
                }
            }

            // Friends Tab
            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: contactViewModel.friends
                delegate: ItemDelegate {
                    width: ListView.view.width
                    contentItem: RowLayout {
                        spacing: 16
                        Image {
                            source: "qrc:/src/views/assets/person.png"
                            width: 40
                            height: 40
                        }
                        Label {
                            text: modelData
                            font.pixelSize: 16
                            color: Material.foreground
                            Layout.fillWidth: true
                        }
                        Button {
                            text: qsTr("Delete")
                            icon.source: "qrc:/src/views/assets/delete.png"
                            onClicked: {
                                contactViewModel.deleteFriend(modelData)
                            }
                        }
                    }
                }
            }
        }
    }
}