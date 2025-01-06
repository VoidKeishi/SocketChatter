import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ApplicationWindow {
    id: mainWindow
    width: 1280
    height: Screen.height - 40
    visible: true
    visibility: Window.Windowed

    // Enable Material theme with dark mode
    Material.theme: Material.Dark
    Material.accent: Material.Purple

    title: qsTr("Main Window")

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

            ContactPage {
                id: contactPage
                anchors.fill: parent

                onContactClicked: function(contactName) {
                    // Update the conversation page
                    conversationPage.inConversationWith = contactName
                    conversationPage.visible = true
                    manageContactsPage.visible = false
                    welcomeMessage.visible = false
                }

                onNavigateToManageContacts: function() {
                    // Show manage contacts page
                    manageContactsPage.visible = true
                    conversationPage.visible = false
                    welcomeMessage.visible = false
                }

                onNavigateToManageGroups: function() {
                    manageGroupsPage.visible = true
                    conversationPage.visible = false
                    manageContactsPage.visible = false
                    welcomeMessage.visible = false
                }
            }
        }

        // Separator line
        Rectangle {
            width: 1
            Layout.fillHeight: true
            color: "#333333"
        }

        // Conversation area (right panel)
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Material.background

            // Include the ConversationPage component
            ConversationPage {
                id: conversationPage
                anchors.fill: parent
                visible: false // Initially hidden
                inConversationWith: contactName
            }

            // Include the ManageContactsPage component
            ManageContactsPage {
                id: manageContactsPage
                anchors.fill: parent
                visible: false // Initially hidden

                onNavigateBack: function() {
                    manageContactsPage.visible = false
                    welcomeMessage.visible = true
                }
            }

            ManageGroupsPage {
                id: manageGroupsPage
                anchors.fill: parent
                visible: false

                onNavigateBack: function() {
                    manageGroupsPage.visible = false
                    welcomeMessage.visible = true
                }
            }

            // Welcome message when no conversation is open and manage contacts is not open
            Column {
                id: welcomeMessage
                anchors.centerIn: parent
                spacing: 20
                visible: true

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