import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ApplicationWindow {
    id: main
    width: 1280
    height: 960
    visible: true

    // Enable Material theme with dark mode
    Material.theme: Material.Dark
    Material.accent: Material.Purple

    // Dark background
    color: "#121212"

    header: ToolBar {
        background: Rectangle {
            color: "#1E1E1E"
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Contact list (left panel)
        Rectangle {
            Layout.preferredWidth: 320
            Layout.fillHeight: true
            color: "#1E1E1E"
            border.width: 1
            border.color: "#333333"

            Loader {
                id: contactLoader
                anchors.fill: parent
                sourceComponent: ContactPage {
                    onContactClicked: (contactName) => {
                        conversationLoader.sourceComponent = undefined
                        conversationLoader.setSource("ConversationPage.qml", { inConversationWith: contactName })
                        conversationLoader.visible = true
                        manageContactsLoader.visible = false
                    }
                    onNavigateToManageContacts: () => {
                        manageContactsLoader.setSource("ManageContactsPage.qml")
                        manageContactsLoader.visible = true
                        conversationLoader.visible = false
                    }
                }
            }
        }

        // Conversation area (right panel)
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1E1E1E"

            Loader {
                id: conversationLoader
                anchors.fill: parent
                visible: false // Initially hidden
            }

            // Manage Contacts area
            Loader {
                id: manageContactsLoader
                anchors.fill: parent
                visible: false // Initially hidden
                onLoaded: {
                    if (item && item.navigateBack) {
                        item.navigateBack.connect(function() {
                            manageContactsLoader.visible = false
                            manageContactsLoader.sourceComponent = undefined
                            conversationLoader.visible = false
                        })
                    }
                }
            }

            // Welcome message when no conversation is open and manage contacts is not open
            Column {
                anchors.centerIn: parent
                spacing: 20
                visible: !conversationLoader.item && !manageContactsLoader.item

                Image {
                    source: "qrc:/src/views/assets/ChatIcon.png"
                    width: 120
                    height: 120
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Label {
                    text: qsTr("Let's start connecting with people")
                    font.pixelSize: 18
                    color: "#666666"
                }
            }
        }
    }
}