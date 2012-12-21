// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
//import QtQuick 1.1
import QtQuick 1.0

Rectangle {
    id: scalpmap
    property int scalling: 2
    width: 800 / scalling
    height: 480 / scalling

    function cqValue(name,value)
    {
    //console.log(name + " "+value)
    for (var i=0; i<children.length; ++i)
    {
        if (children[i].name == name)
        children[i].value = value
        //console.log(children[i].name)
    }
    }

    Image {
    id: scalpimage
    source: "qrc:/scalpmap.png"
    anchors.fill: parent
    }

    Electrode{name: "P7"; x: 291/scalling- 5; y: 316/scalling}
    Electrode{name: "FC6"; x: 540/scalling- 5; y: 202/scalling}
    Electrode{name: "T7"; x: 240/scalling- 5; y: 246/scalling}
    Electrode{name: "P8"; x: 534/scalling- 5; y: 316/scalling}
    Electrode{name: "O2"; x: 450/scalling- 5; y: 343/scalling}
    Electrode{name: "O1"; x: 373/scalling- 5; y: 343/scalling}
    Electrode{name: "FC5"; x: 291/scalling- 5; y: 202/scalling}
    Electrode{name: "F3"; x: 348/scalling- 5; y: 171/scalling}
    Electrode{name: "F4"; x: 484/scalling- 5; y: 171/scalling}
    Electrode{name: "T8"; x: 590/scalling- 5; y: 246/scalling}
    Electrode{name: "F7"; x: 269/scalling- 5; y: 154/scalling}
    Electrode{name: "F8"; x: 562/scalling- 5; y: 154/scalling}
    Electrode{name: "AF4"; x: 464/scalling- 5; y: 137/scalling}
    Electrode{name: "AF3"; x: 366/scalling- 5; y: 137/scalling}

}
