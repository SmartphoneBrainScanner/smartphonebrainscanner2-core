import Qt 4.7

Item
{

    x: 100
    y: 100
    property string desc
    property string text : textInput.text
    property int fakeWidth: 260
    property int fakeHeight: 80


    Rectangle
    {

	id: input
	width: parent.fakeWidth
	height: parent.fakeHeight
	color: "lightGray"
	border.color: "black"

	Rectangle{
	    width: 40
	    height: width
	    color: parent.color
	    border.width: 1
	    border.color: "black"
	    anchors.left: parent.right
	    anchors.verticalCenter: parent.verticalCenter
	    Text
	    {
		anchors.verticalCenter: parent.verticalCenter
		anchors.horizontalCenter: parent.horizontalCenter
		text: "X"
		color: "black"
	    }
	    MouseArea
	    {
		anchors.fill: parent
		onClicked:
		{
		    textInput.text = ""
		}
	    }
	}

	TextInput
	{
	    id: textInput

	    color: "black"
	    font.bold: true
	    font.pointSize: 20
	    cursorVisible: false

	    smooth: true
	    anchors.fill: parent
	    anchors.leftMargin: 10
	    anchors.topMargin: 30




	}
    }
    Rectangle
    {
	width: 140
	height: 40
	color: "gray"
	border.color: "black"
	Text
	{
	    text: {return String(parent.parent.desc); }
	    anchors.centerIn: parent
	}
	anchors.bottom: parent.top
	anchors.bottomMargin: -5
	anchors.left: parent.left
	anchors.leftMargin: 5
    }
}
