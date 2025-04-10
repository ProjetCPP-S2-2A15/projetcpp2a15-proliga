import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15

Item {
    width: 800
    height: 600

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(36.8065, 10.1815)
        zoomLevel: 12

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            property bool dragging: false
            property real lastX: 0
            property real lastY: 0

            onPressed: (mouse) => {
                dragging = true;
                lastX = mouse.x;
                lastY = mouse.y;
            }

            onPositionChanged: (mouse) => {
                if (dragging) {
                    var dx = lastX - mouse.x;
                    var dy = lastY - mouse.y;
                    map.pan(dx, dy);
                    lastX = mouse.x;
                    lastY = mouse.y;
                }
            }

            onReleased: {
                dragging = false;
            }

            onWheel: (wheel) => {
                if (wheel.angleDelta.y > 0) {
                    map.zoomLevel += 1;
                } else {
                    map.zoomLevel -= 1;
                }
            }
        }

        MapItemView {
            model: stadiumModel
            delegate: MapQuickItem {
                coordinate: QtPositioning.coordinate(modelData.latitude, modelData.longitude)
                anchorPoint.x: 12
                anchorPoint.y: 12
                sourceItem: Column {
                    spacing: 2
                    Rectangle {
                        width: 24; height: 24
                        radius: 12
                        color: "red"
                        border.color: "white"
                        border.width: 2
                    }
                    Text {
                        text: modelData.name || "Nom inconnu"
                        font.pixelSize: 10
                        color: "black"
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
                Component.onCompleted: {
                    console.log("Marqueur:", modelData.name, "lat:", modelData.latitude, "lon:", modelData.longitude);
                }
            }
        }

        Button {
            text: "-"
            width: 40
            height: 40
            anchors {
                right: parent.right
                bottom: parent.bottom
                margins: 10
            }
            onClicked: {
                map.zoomLevel -= 1
            }
        }

        Button {
            text: "+"
            width: 40
            height: 40
            anchors {
                right: parent.right
                bottom: parent.bottom
                margins: 10
                bottomMargin: 60 // Added margin to create distance
            }
            onClicked: {
                map.zoomLevel += 1
            }
        }
    }

    Component.onCompleted: {
        console.log("Nombre de stades:", stadiumModel.length);
        for (var i = 0; i < stadiumModel.length; i++) {
            console.log("Stade", i, ":", JSON.stringify(stadiumModel[i]));
        }
        if (stadiumModel.length > 0) {
            map.center = QtPositioning.coordinate(stadiumModel[0].latitude, stadiumModel[0].longitude);
        }
    }
}
