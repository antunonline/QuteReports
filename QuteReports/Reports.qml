import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import hr.com.unix 1.0;

Item {
    SqlQueryModel {
        id: reportsModel
        dbName: "Default"
        sqlQuery: "select id, name, description, report from reports order by id;"
        columnNames: ["id","name","description", "report"]
        Component.onCompleted: exec();
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.margins: 10
            height: 40

            Text {
                id: reportTitleLbl
                text: "Naziv izvještaja:"
                anchors.verticalCenter: parent.verticalCenter
            }
            TextField {
                Layout.minimumWidth: 200
                anchors.left: reportTitleLbl.right
                anchors.verticalCenter: parent.vertic
            }
        }


        TableView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: reportsModel

            TableViewColumn {
                role: "name"
                title: "Naziv"
                width: 200
            }

            TableViewColumn {
                role: "description"
                title: "Opis"
                width: 400
            }
            onDoubleClicked: function(rowId){
                var reportQmlCode = reportsModel.data(reportsModel.index(rowId, 3));
                var reportName = reportsModel.data(reportsModel.index(rowId, 1));

                console.log(reportQmlCode)
                console.log(reportName)
            }
        }
    }
}
