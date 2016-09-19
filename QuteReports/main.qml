import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.4


ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Izvještaji")

    TabView{
        anchors.fill: parent
        Tab{
            title: "Dostupni izvještaji"
            Reports{}
        }

        Tab {
            title: "Izvještaj 1"
            QuteFormReport1{}
        }
    }
}
