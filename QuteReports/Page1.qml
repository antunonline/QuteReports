import QtQuick 2.7
import QtQuick.Dialogs 1.2
import hr.com.unix 1.0

Page1Form {

    ListModel {
        id: fruitModel

        ListElement {
            name: "Apple"
            cost: 2.45
            buckus: "Woot?"
        }
        ListElement {
            name: "Orange"
            cost: 3.25
            buckus: "Woot?"
        }
        ListElement {
            name: "Banana"
            cost: 1.95
            buckus: "Woot?"
        }
    }

    ItemModelXlsExporter {
        id: xlsxPorter
    }

    FileDialog {
        id: fd
        selectExisting: false
        onSelectionAccepted: function(){
            console.log(fd.fileUrl)
            xlsxPorter.exportQmlModel(fd.fileUrl, fruitModel)
        }
    }

    button1.onClicked: {
        fd.open();

    }
    button2.onClicked: {
        console.log("Button 2 clicked.");
    }
}
