import Qt 4.7

Rectangle {
   // anchors.fill: parent
    id: setupScreen
    x: 0
    y: 0
    width: parent.width
    height: parent.height
    color: "white"
    property int counter: 0
    property int direction: 1

    function cqValue(name,value)
    {
	scalpmap.cqValue(name,value);
	setupScreen.counter = (setupScreen.counter + direction)
	if (setupScreen.counter == 100)
	    direction = -1
	if (setupScreen.counter == 0)
	    direction = 1
    }





    Rectangle
    {
	color: parent.color
	border.color: "darkGrey"
	border.width: 4
	anchors.centerIn: parent
	width: parent.width - 10
	height: parent.height - 10
    }

     Scalpmap{id:scalpmap; anchors.right: parent.right; anchors.bottom: parent.bottom; anchors.margins: 10}

    Text {
	x: 20;
	y: 20;
	color: "black"
	text: page.title
	font.family: "Helvetica"
	font.pointSize: 27
	font.bold: true
    }

    Text {
	id: time
	text: " "
	property int move : 0

	Component.onCompleted: set()

	function set()
	{
	    var currentTime = new Date();
	    var hours = currentTime.getHours();
	    var minutes = currentTime.getMinutes();
	    var seconds = currentTime.getSeconds();
	    var timeString="" ;
	    if (minutes < 10)
	    {
		minutes = "0" + minutes
	    }
	    if (hours < 10)
	    {
		hours = "0" + hours
	    }
	    if (seconds < 10)
	    {
		seconds = "0" + seconds
	    }


	    timeString+=hours + ":" + minutes + ":" + seconds;

	    time.text = timeString;
	    time.move = seconds;


	}

	Timer
	{
	    id: timeTimer
	    interval: 1000
	    repeat: true
	    running: true
	    onTriggered: {time.set(); }
	}

	x: 20
	y: page.height - 40
	color: "grey"
	font.family: "Helvetica"
	font.pointSize: 27
	font.bold: true


	Rectangle
	{
	    color: "grey"
	    anchors.top: parent.bottom
	    anchors.topMargin: 0

	    width: parent.width
	    height: 10
	    Behavior on width {
		NumberAnimation {easing.type: Easing.InOutBounce}
	    }
	    Rectangle
	    {
		color: "black"
		height: parent.height
		width: 10
		x: time.move/ 60.0 * parent.width - width
		opacity: time.move/ 60.0

		Behavior on x
		{
		    NumberAnimation {easing.type: Easing.InOutBounce}
		}
	    }
	}



    }




    MyTextInput {id: user; desc: "user"; x: 20; y: 100}
    MyTextInput {id: description; desc: "description"; x: 20; y: 230}
    Button {id: quit; desc: "quit"; y: 80; x: page.width-quit.width}
    Button {id: start; desc: "start"; y: 330; x: 0}



    Rectangle
    {
	id: displayLitSwitch
	color: {if (turnedOn) return "black"; return "white" }
	property int turnedOn: 1
	border.width: 1
	border.color: "black"
	anchors.verticalCenter: user.verticalCenter
	x: page.width/2
	width: 40
	height: width

	Text
	{
	    anchors.horizontalCenter: parent.horizontalCenter
	    anchors.bottom: parent.top
	    text: "keep display on"
	}

	MouseArea
	{
	    anchors.fill: parent
	    onClicked:
	    {
		parent.turnedOn = !parent.turnedOn;
		page.sendSignal("displaylit");


	    }
	}
    }


    Rectangle
    {
	id: rotationSwitch
	color: {if (turnedOn) return "black"; return "white" }
	property int turnedOn: 1
	border.width: 1
	border.color: "black"
	anchors.verticalCenter: description.verticalCenter
	x: page.width/2
	width: 40
	height: width

	Text
	{
	    anchors.horizontalCenter: parent.horizontalCenter
	    anchors.bottom: parent.top
	    text: "real rotation test"
	}

	MouseArea
	{
	    anchors.fill: parent
	    onClicked:
	    {
		parent.turnedOn = !parent.turnedOn;
		page.emp = !parent.turnedOn;


	    }
	}
    }


    Rectangle
    {
	id: aliveIndicator
	width: 15* (setupScreen.counter+1)/100.0
	height: start.height
	color: "black"
	anchors.bottom: start.bottom
	anchors.left: start.right

    }


    Behavior on opacity {
	NumberAnimation { duration: 300; easing.type: Easing.InOutBounce }
    }
    Behavior on x {
	NumberAnimation { duration: 200; easing.type: Easing.InOutBounce }
    }



}
