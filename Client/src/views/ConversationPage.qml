import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Page {
    id: conversationPage
    property string inConversationWith
    Material.theme: Material.Dark
    Material.accent: Material.Purple

    header: ToolBar {
        background: Rectangle {
            color: "#1E1E1E"
        }
        Label {
            id: pageTitle
            text: conversationPage.inConversationWith
            font.pixelSize: 20
            anchors.centerIn: parent
            color: "#FFFFFF"
        }
    }

    Component.onCompleted: {
        conversationViewModel.setCurrentContact(inConversationWith)
    }

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: pane.leftPadding + messageField.leftPadding
            displayMarginBeginning: 40
            displayMarginEnd: 40
            verticalLayoutDirection: ListView.TopToBottom
            spacing: 12
            model: conversationViewModel

            delegate: Column {
                id: conversationDelegate
                spacing: 6
                width: listView.width
                
                required property string author
                required property string content 
                required property date timestamp
                required property bool sentByMe

                anchors {
                    right: sentByMe ? listView.contentItem.right : undefined
                    left: sentByMe ? undefined : listView.contentItem.left
                    rightMargin: sentByMe ? 6 : 0
                    leftMargin: sentByMe ? 0 : 6
                }

                Row {
                    id: messageRow
                    spacing: 6
                    anchors.right: conversationDelegate.sentByMe ? parent.right : undefined

                    Rectangle {
                        id: avatar
                        width: 40
                        height: 40
                        radius: width/2
                        visible: !conversationDelegate.sentByMe
                        
                        Image {
                            anchors.fill: parent
                            source: !conversationDelegate.sentByMe 
                                ? "qrc:/src/views/assets/" + "man1" + ".png" 
                                : ""
                            fillMode: Image.PreserveAspectFit
                        }
                    }

                    Rectangle {
                        id: messageBubble
                        width: Math.min(messageText.implicitWidth + 24, 
                            listView.width - (!conversationDelegate.sentByMe ? avatar.width + messageRow.spacing : 0))
                        height: messageText.implicitHeight + 24
                        radius: 15
                        color: conversationDelegate.sentByMe ? "#d094dc" : "#E8E8E8"

                        Label {
                            id: messageText
                            anchors {
                                fill: parent
                                margins: 12
                            }
                            text: conversationDelegate.content
                            wrapMode: Label.Wrap
                            color: "black"
                        }
                    }
                }

                Label {
                    id: timestampText
                    text: Qt.formatDateTime(conversationDelegate.timestamp, "d MMM hh:mm")
                    color: "#666666"
                    anchors.right: conversationDelegate.sentByMe ? parent.right : undefined
                    anchors.left: !conversationDelegate.sentByMe ? parent.left : undefined
                    anchors.leftMargin: !conversationDelegate.sentByMe ? avatar.width + messageRow.spacing : 0
                    font.pixelSize: 12
                }
            }

            ScrollBar.vertical: ScrollBar {}
        }

        Pane {
            id: pane
            Layout.fillWidth: true
            Layout.fillHeight: false
            background: Rectangle {
                color: "#1E1E1E"
            }

            RowLayout {
                width: parent.width
                spacing: 8

                Flickable {
                    id: flickable
                    Layout.fillWidth: true
                    Layout.preferredHeight: Math.min(contentHeight, 120)
                    contentHeight: messageField.implicitHeight
                    clip: true
                    
                    TextArea {
                        id: messageField
                        width: parent.width
                        height: Math.min(implicitHeight, 120)
                        anchors.fill: parent
                        placeholderText: qsTr("Type a message...")
                        wrapMode: TextArea.Wrap
                        selectByMouse: true
                        color: "#FFFFFF"
                        
                        Keys.onReturnPressed: (event)=> {
                            if (event.modifiers & Qt.ShiftModifier) {
                                event.accepted = false
                            } else {
                                sendButton.clicked()
                                event.accepted = true
                            }
                        }

                    }

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }
                }

                Button {
                    id: sendButton
                    text: qsTr("Send")
                    enabled: messageField.length > 0
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 50
                    Material.background: Material.Purple
                    onClicked: {
                        conversationViewModel.sendMessage(messageField.text)
                        messageField.text = ""
                    }
                }
            }
        }
    }
    onInConversationWithChanged: {
        conversationViewModel.setCurrentContact(inConversationWith)
    }
}