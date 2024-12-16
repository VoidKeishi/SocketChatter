// LoginWindow.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material


ApplicationWindow {
    signal loginRequested(string username, string password)
    signal loginResult(bool success, string message)
    signal registerRequested(string username, string password)
    signal registerResult(bool success, string message)
    id: authWindow
    width: 540
    height: 960
    visible: true
    title: qsTr("Login")

    // Enable Material dark theme
    Material.theme: Material.Dark
    Material.accent: Material.Purple
    color: "#121212"
    
    TabBar {
        id: bar
        width: parent.width
        Material.background: "#1E1E1E"

        TabButton {
            text: qsTr("Login")
        }
        TabButton {
            text: qsTr("Register")
        }
    }

    StackLayout {
        width: parent.width
        anchors.top: bar.bottom
        anchors.bottom: parent.bottom
        currentIndex: bar.currentIndex

        // Login page
        Page {
            id: loginPage
            background: Rectangle { color: "#121212" }

            Label {
                text: qsTr("Welcome to Socket Chat!")
                font.pixelSize: 20
                color: "#FFFFFF"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 70
            }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 20
                width: parent.width * 0.8

                TextField {
                    id: usernameField
                    placeholderText: qsTr("Username")
                    Layout.fillWidth: true
                    Material.accent: Material.Purple
                }

                TextField {
                    id: passwordField
                    placeholderText: qsTr("Password")
                    echoMode: TextInput.Password
                    Layout.fillWidth: true
                    Material.accent: Material.Purple
                }

                Button {
                    text: qsTr("Login")
                    Layout.fillWidth: true
                    Material.background: Material.Purple
                    onClicked: {
                        loginController.handleLoginRequest(usernameField.text, passwordField.text)
                    }
                }
            }
        }

        // Register page
        Page {
            id: registerPage
            background: Rectangle { color: "#121212" }

            Label {
                text: qsTr("Welcome to Socket Chat!")
                font.pixelSize: 20
                color: "#FFFFFF"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 70
            }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 20
                width: parent.width * 0.8

                TextField {
                    id: regUsernameField
                    placeholderText: qsTr("Username")
                    Layout.fillWidth: true
                    Material.accent: Material.Purple
                }

                TextField {
                    id: regPasswordField
                    placeholderText: qsTr("Password")
                    echoMode: TextInput.Password
                    Layout.fillWidth: true
                    Material.accent: Material.Purple
                }

                TextField {
                    id: regConfirmPasswordField
                    placeholderText: qsTr("Confirm Password")
                    echoMode: TextInput.Password
                    Layout.fillWidth: true
                    Material.accent: Material.Purple
                }

                Button {
                    text: qsTr("Register")
                    Layout.fillWidth: true
                    Material.background: Material.Purple
                    enabled: regPasswordField.text === regConfirmPasswordField.text
                    onClicked: {
                        registerController.handleRegisterRequest(regUsernameField.text, regPasswordField.text)
                    }
                }
            }
        }
    }

    Connections {
        target: loginController
        function onLoginResult(success, message) {
            if (success) {
                var component = Qt.createComponent("qrc:/src/views/MainWindow.qml")
                if (component.status === Component.Ready) {
                    var window = component.createObject(null)
                    if (window) {
                        window.show()
                        authWindow.close()
                    } else {
                        console.error("Error creating window:", component.errorString())
                    }
                } else {
                    console.error("Error loading component:", component.errorString())
                }
            } else {
                console.error("Login failed:", message)
            }
        }
    }

    Connections {
        target: registerController
        function onRegisterResult(success, message) {
            if (success) {
                bar.currentIndex = 0
            } else {
                console.error("Registration failed:", message)
            }
        }
    }
}