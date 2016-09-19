import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Item {

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            id: r1
            Layout.fillHeight: true
            Layout.margins: 10


            Label {
                text: "Korisnik"
                Layout.leftMargin: 20
            }

            TextField {
                width: 200
                Layout.leftMargin: 20
            }

           Label {
               Layout.leftMargin: 20
               text: "Od:"
           }

           TextField {
               id: from
               width: 100
               onFocusChanged:  function(){
                   if(this.focus){
                       qtCal.x = this.x
                       qtCal.y = this.y
                       qtCal.callback = function(year, month, day){
                           from.text = day + "."+month+"."+year+"."
                       }

                       qtCal.visible = true
                       this.focus = false;
                   }
               }
           }

           Label {
               Layout.leftMargin: 20
               text: "Do:"
           }

           TextField {
               id: to
               width: 100
               onFocusChanged:  function(){
                   if(this.focus){
                       qtCal.x = this.x
                       qtCal.y = this.y
                       qtCal.callback = function(year, month, day){
                           to.text = day + "."+month+"."+year+"."
                       }

                       qtCal.visible = true
                       this.focus = false;
                   }
               }
           }

            Button {
                Layout.leftMargin: 20
                text: "Prikaži"
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }
        }

        TableView {
            Layout.fillHeight: true
            Layout.fillWidth: true


            TableViewColumn {
                title: "Column1"
                width: 100
            }

            TableViewColumn {
                title: "Column 2"
                width: 200
            }

        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 10

            Button {
                text: "Izvezi u Excel"
            }

        }
    }
    QuteCalendar {
        id: qtCal
        visible: false
        x:  40
        y: 40
        z: 100

    }

}
