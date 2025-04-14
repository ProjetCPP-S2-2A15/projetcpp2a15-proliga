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

        property var startCoordinate: null
        property var pathCoordinates: []
        property real distance: 0

        MapPolyline {
            id: pathLine
            line.width: 4
            line.color: "darkblue"
            path: map.pathCoordinates
        }

        MapQuickItem {
            id: startMarker
            coordinate: map.startCoordinate ? map.startCoordinate : QtPositioning.coordinate(0, 0)
            anchorPoint.x: 8
            anchorPoint.y: 8
            visible: map.startCoordinate !== null
            sourceItem: Rectangle {
                width: 16
                height: 16
                radius: 8
                color: "orange"
                border.color: "white"
                border.width: 1
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // Clicking the orange marker does nothing to the path
                        console.log("Clicked on orange marker, no path change")
                    }
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            property bool dragging: false
            property real lastX: 0
            property real lastY: 0
            property real pressTime: 0

            onPressed: (mouse) => {
                if (mouse.button === Qt.LeftButton) {
                    dragging = false
                    lastX = mouse.x
                    lastY = mouse.y
                    pressTime = new Date().getTime()
                }
            }

            onPositionChanged: (mouse) => {
                if (mouse.buttons & Qt.LeftButton) {
                    var currentTime = new Date().getTime()
                    // Start dragging only if mouse has moved significantly or held for a short time
                    if (!dragging && (Math.abs(mouse.x - lastX) > 5 || Math.abs(mouse.y - lastY) > 5 || currentTime - pressTime > 200)) {
                        dragging = true
                    }
                    if (dragging) {
                        var dx = lastX - mouse.x
                        var dy = lastY - mouse.y
                        map.pan(dx, dy)
                        lastX = mouse.x
                        lastY = mouse.y
                    }
                }
            }

            onReleased: (mouse) => {
                if (mouse.button === Qt.LeftButton && !dragging) {
                    // Place marker only if it was a quick click without dragging
                    var coord = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                    map.startCoordinate = coord
                    map.pathCoordinates = [] // Clear path when setting new start point
                    map.distance = 0 // Reset distance
                    console.log("Start point set:", coord.latitude, coord.longitude)
                }
                dragging = false
            }

            onWheel: (wheel) => {
                // Wheel scrolling for zooming, always active
                if (wheel.angleDelta.y > 0) {
                    map.zoomLevel += 1
                } else if (wheel.angleDelta.y < 0) {
                    map.zoomLevel -= 1
                }
                wheel.accepted = true
            }
        }

        MapItemView {
            model: stadiumModel
            delegate: MapQuickItem {
                coordinate: QtPositioning.coordinate(modelData.latitude, modelData.longitude)
                anchorPoint.x: 8
                anchorPoint.y: 8
                sourceItem: Column {
                    spacing: 1
                    Rectangle {
                        width: 16
                        height: 16
                        radius: 8
                        color: "green"
                        border.color: "white"
                        border.width: 1
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (map.startCoordinate) {
                                    // Click on stadium: draw path, keep start marker, calculate distance
                                    var endCoord = QtPositioning.coordinate(modelData.latitude, modelData.longitude)
                                    map.pathCoordinates = [map.startCoordinate, endCoord]
                                    map.distance = map.startCoordinate.distanceTo(endCoord) / 1000 // Convert to kilometers
                                    console.log("Path set from", map.startCoordinate, "to", endCoord, "Distance:", map.distance, "km")
                                }
                            }
                        }
                    }
                    Text {
                        text: modelData.name || "Nom inconnu"
                        font.pixelSize: 8
                        color: "black"
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
                Component.onCompleted: {
                    console.log("Marqueur:", modelData.name, "lat:", modelData.latitude, "lon:", modelData.longitude)
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
                bottomMargin: 60
            }
            onClicked: {
                map.zoomLevel += 1
            }
        }
    }

    Text {
        id: distanceText
        anchors {
            left: parent.left
            top: parent.top
            margins: 10
        }
        text: map.distance > 0 ? "Distance: " + map.distance.toFixed(2) + " km" : ""
        font.pixelSize: 14
        color: "black"
    }

    Component.onCompleted: {
        console.log("Nombre de stades:", stadiumModel.length)
        for (var i = 0; i < stadiumModel.length; i++) {
            console.log("Stade", i, ":", JSON.stringify(stadiumModel[i]))
        }
        if (stadiumModel.length > 0) {
            map.center = QtPositioning.coordinate(stadiumModel[0].latitude, stadiumModel[0].longitude)
        }
    }
}
