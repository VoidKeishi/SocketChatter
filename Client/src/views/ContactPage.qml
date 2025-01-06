import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Page {
    id: contactPage
    signal contactClicked(string contactName)
    signal navigateToManageContacts()
    signal groupClicked(string groupName)
    signal navigateToManageGroups()

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Label {
            text: qsTr("Contacts")
            font.pixelSize: 20
            color: "#FFFFFF"
            Layout.leftMargin: 48
            Layout.topMargin: 20
            Layout.bottomMargin: 20
            Layout.alignment: Qt.AlignHCenter
        }

        TabBar {
            id: tabBar
            Layout.fillWidth: true
            TabButton { text: qsTr("Contacts") }
            TabButton { text: qsTr("Groups") }
        }

        StackLayout {
            currentIndex: tabBar.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Contacts Tab
            Item {
                ListView {
                    id: contactsListView
                    anchors.fill: parent
                    topMargin: 16
                    leftMargin: 48
                    rightMargin: 48
                    bottomMargin: 48
                    spacing: 20
                    model: contactViewModel.friends   
                    delegate: ItemDelegate {
                        id: contactDelegate
                        width: contactsListView.width - contactsListView.leftMargin - contactsListView.rightMargin
                        height: avatar.implicitHeight
                        leftPadding: avatar.implicitWidth + 32

                        onClicked: {
                            console.log("Contact clicked:", modelData) 
                            contactPage.contactClicked(modelData)
                        }

                        RowLayout {
                            anchors.fill: parent
                            spacing: 16

                            Image {
                                id: avatar
                                source: "qrc:/src/views/assets/man" + (index % 5) + ".png"
                                width: 40
                                height: 40
                                sourceSize.width: width
                                sourceSize.height: height
                                fillMode: Image.PreserveAspectFit
                            }

                            Label {
                                text: modelData
                                font.pixelSize: 16
                                Layout.fillWidth: true
                                color: "#FFFFFF"
                            }
                        }
                    }
                }

                Button {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 20
                    Material.background: Material.Purple
                    onClicked: contactPage.navigateToManageContacts()
                    contentItem: Text {
                        text: qsTr("Manage Contacts")
                        color: "#0F0F0F"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    padding: 10
                }
            }

            // Groups Tab
            Item {
                ListView {
                    id: groupsListView
                    anchors.fill: parent
                    topMargin: 16
                    leftMargin: 48
                    rightMargin: 48
                    bottomMargin: 48
                    spacing: 20
                    model: contactViewModel.groups   
                    delegate: ItemDelegate {
                        id: groupDelegate
                        width: groupsListView.width - groupsListView.leftMargin - groupsListView.rightMargin
                        height: avatar.implicitHeight
                        leftPadding: avatar.implicitWidth + 32

                        onClicked: {
                            console.log("Group clicked:", modelData) 
                            contactPage.groupClicked(modelData)
                        }

                        RowLayout {
                            anchors.fill: parent
                            spacing: 16

                            Image {
                                id: avatar
                                source: "qrc:/src/views/assets/group.png"
                                width: 40
                                height: 40
                                sourceSize.width: width
                                sourceSize.height: height
                                fillMode: Image.PreserveAspectFit
                            }

                            Label {
                                text: modelData
                                font.pixelSize: 16
                                Layout.fillWidth: true
                                color: "#FFFFFF"
                            }
                        }
                    }
                }

                Button {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 20
                    Material.background: Material.Green
                    onClicked: contactPage.navigateToManageGroups()
                    contentItem: Text {
                        text: qsTr("Manage Groups")
                        color: "#0F0F0F"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    padding: 10
                }
            }
        }
    }
}