import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects

Page {
    id: manageGroupsPage
    signal navigateBack()
    Material.theme: Material.Dark
    Material.accent: Material.Purple

    Component.onCompleted: {
        groupViewModel.fetchGroups()
        groupViewModel.fetchGroupsInvitations()
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
                    onClicked: manageGroupsPage.navigateBack()
                }
                Label {
                    text: qsTr("Manage Groups")
                    font.pixelSize: 20
                    color: Material.foreground
                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }

        TabBar {
            id: tabBar
            Layout.fillWidth: true
            TabButton { text: qsTr("Create Group") }
            TabButton { text: qsTr("Received Group Invitations") }
        }

        StackLayout {
            id: stackLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            // Create Group Tab
            ColumnLayout {
                Layout.fillHeight: true
                Layout.margins: 16
                Layout.alignment: Qt.AlignTop

                TextField {
                    id: sendRequestField
                    placeholderText: qsTr("Enter group name")
                    Layout.fillWidth: true
                    Layout.topMargin: 16
                }

                Button {
                    text: qsTr("Create group")
                    icon.source: "qrc:/src/views/assets/group.png"
                    onClicked: {
                        groupViewModel.sendGroupCreateRequest(sendRequestField.text)
                        sendRequestField.clear()
                    }
                    Layout.leftMargin: 16
                }

                Label {
                    text: qsTr("Invite friends to your group")
                    font.pixelSize: 20
                    color: Material.foreground
                    Layout.leftMargin: 16
                }

                TextField {
                    id: sendInviteGroupNameField
                    placeholderText: qsTr("Enter group name")
                    Layout.fillWidth: true
                    Layout.topMargin: 16
                }

                TextField {
                    id: sendInviteFriendNameField
                    placeholderText: qsTr("Enter friend name to invite")
                    Layout.fillWidth: true
                    Layout.topMargin: 16
                }

                Button {
                    text: qsTr("Inivte")
                    icon.source: "qrc:/src/views/assets/person_add.png"
                    onClicked: {
                        groupViewModel.sendGroupInvitation(sendInviteGroupNameField.text, sendInviteFriendNameField.text)
                        sendInviteGroupNameField.clear()
                        sendInviteFriendNameField.clear()
                    }
                    Layout.leftMargin: 16
                }
            }

            // Received Invitation Tab
            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: groupViewModel.receivedInvitations
                delegate: ItemDelegate {
                    width: ListView.view.width
                    contentItem: RowLayout {
                        spacing: 16
                        Image {
                            source: "qrc:/src/views/assets/group.png"
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
                                groupViewModel.respondGroupInvitation(modelData, true)
                            }
                        }
                        Button {
                            text: qsTr("Reject")
                            icon.source: "qrc:/src/views/assets/remove_circle.png"
                            onClicked: {
                                groupViewModel.respondGroupInvitation(modelData, false)
                            }
                        }
                    }
                }
            }
        }
    }
}