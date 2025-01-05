import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

ApplicationWindow {
    id: authWindow
    width: 540
    height: Screen.height - 40
    visible: true
    visibility: Window.Windowed
    title: qsTr("Login")

    Material.theme: Material.Dark
    Material.accent: Material.Purple
    color: "#121212"
    
    TabBar {
        id: bar
        width: parent.width
        Material.background: "#1E1E1E"
        currentIndex: authViewModel.currentTab
        enabled: !authViewModel.isLoading
        onCurrentIndexChanged: authViewModel.setCurrentTab(currentIndex)

        TabButton {
            text: qsTr("Login")
            enabled: !authViewModel.isLoading
        }
        TabButton {
            text: qsTr("Register")
            enabled: !authViewModel.isLoading
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
            enabled: !authViewModel.isLoading
            background: Rectangle { color: "#121212" }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 20
                width: parent.width * 0.8

                Label {
                    text: qsTr("Welcome to Socket Chat!")
                    font.pixelSize: 20
                    color: "#FFFFFF"
                    Layout.alignment: Qt.AlignHCenter
                }

                TextField {
                    id: usernameField
                    placeholderText: qsTr("Username")
                    Layout.fillWidth: true
                    enabled: !authViewModel.isLoading
                    Material.accent: Material.Purple
                }

                TextField {
                    id: passwordField
                    placeholderText: qsTr("Password")
                    echoMode: TextInput.Password
                    Layout.fillWidth: true
                    enabled: !authViewModel.isLoading
                    Material.accent: Material.Purple
                }

                Button {
                    text: authViewModel.isLoading ? qsTr("Logging in...") : qsTr("Login")
                    Layout.fillWidth: true
                    Material.background: Material.Purple
                    enabled: !authViewModel.isLoading && usernameField.text && passwordField.text
                    onClicked: {
                        authViewModel.sendRequestLogin(usernameField.text, passwordField.text)
                    }
                }
            }
        }

        // Register page
        Page {
            id: registerPage
            enabled: !authViewModel.isLoading
            background: Rectangle { color: "#121212" }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 20
                width: parent.width * 0.8

                Label {
                    text: qsTr("Create Account")
                    font.pixelSize: 20
                    color: "#FFFFFF"
                    Layout.alignment: Qt.AlignHCenter
                }

                TextField {
                    id: regUsernameField
                    placeholderText: qsTr("Username")
                    Layout.fillWidth: true
                    enabled: !authViewModel.isLoading
                    Material.accent: Material.Purple
                }

                TextField {
                    id: regPasswordField
                    placeholderText: qsTr("Password")
                    echoMode: TextInput.Password
                    Layout.fillWidth: true
                    enabled: !authViewModel.isLoading
                    Material.accent: Material.Purple
                }

                TextField {
                    id: regConfirmPasswordField
                    placeholderText: qsTr("Confirm Password")
                    echoMode: TextInput.Password
                    Layout.fillWidth: true
                    enabled: !authViewModel.isLoading
                    Material.accent: Material.Purple
                }

                Button {
                    text: authViewModel.isLoading ? qsTr("Registering...") : qsTr("Register")
                    Layout.fillWidth: true
                    Material.background: Material.Purple
                    enabled: !authViewModel.isLoading && 
                            regUsernameField.text && 
                            regPasswordField.text && 
                            regPasswordField.text === regConfirmPasswordField.text
                    onClicked: {
                        authViewModel.sendRequestRegister(regUsernameField.text, regPasswordField.text)
                    }
                }
            }
        }
    }

    // Message display
    Rectangle {
        id: messageBox
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 20
        width: messageText.width + 40
        height: messageText.height + 20
        radius: 4
        color: "#323232"
        visible: authViewModel.message !== ""
        opacity: 0.9

        Label {
            id: messageText
            anchors.centerIn: parent
            text: authViewModel.message
            color: "white"
        }
    }
    
    Connections {
        target: authViewModel
        function onLoggedInChanged() {
            if (authViewModel.isLoggedIn) {
                var comp = Qt.createComponent("qrc:/src/views/MainWindow.qml")
                if (comp.status === Component.Ready) {
                    comp.createObject(null)
                    authWindow.close()
                } else {
                    console.log("MainWindow.qml load error:", comp.errorString())
                }
            }
        }
    }
}