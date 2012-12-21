import Qt 4.7

Rectangle {
    width: 180
    height: 60
    color: "black"
    property string desc
    x: 0
    y: 0



    Text {
	color: "white"
	text: parent.desc
	font.pointSize: 20
	anchors.centerIn: parent
	font.bold: true
    }

    MouseArea
    {
	anchors.fill: parent
	onClicked:
	{
	    if (parent.desc == "quit")
		Qt.quit();
	    if (parent.desc == "start")
	    {
		page.state = "show";
		page.event("STARTED;"+ user.text + ";"+description.text);
	    }
	}
    }
}
