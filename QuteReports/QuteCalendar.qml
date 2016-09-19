import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Rectangle {
    id: cal
    width: 250
    height: 200
    border.width: 2
    border.color: "black"

    signal dateSelected(int year, int month, int day);
    property int year: parseInt(new Date().toLocaleDateString(Qt.locale(),"yyyy"))
    property int month : parseInt(new Date().toLocaleDateString(Qt.locale(), "M"))
    property var callback : null;
    property var date : new Date();

    Binding{target: monthGrid; property: "year" ;  value: parseInt(yearSpinBox.value)}
    Binding{target: monthGrid; property: "month" ;  value: parseInt(monthSpinBox.value)}


    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Label{
                anchors.verticalCenter: parent.verticalCenter
                Layout.fillWidth: true
                height: 30
                text:  "zatvori"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                MouseArea {
                    anchors.fill: parent
                    onClicked: function(){
                        cal.visible=false
                    }
                }
            }
        }

        Calendar {
            selectedDate: cal.date
        }
    }


}
