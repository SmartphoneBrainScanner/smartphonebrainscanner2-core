#include <sbs2common.h>

Sbs2Common* Sbs2Common::m_pInstance = NULL;
int Sbs2Common::hardwareSet = 0;
QString Sbs2Common::hardware = "emotiv";

int Sbs2Common::normalize(int value)
{
    if (value < 0)
    {
	return 256 + value;
    }
    return value;
}


void Sbs2Common::setHardware(QString hardware_)
{
    hardwareSet = 1;
    hardware = hardware_;
    Sbs2Common::New()->generateHardware();

}

void Sbs2Common::setHardware(QString hardware_, Sbs2Common *me)
{
    hardwareSet = 1;
    hardware = hardware_;
    me->generateHardware();
}

QString Sbs2Common::getCurrentHardware()
{
    return Sbs2Common::New()->hardware;
}

void Sbs2Common::generateEmotiv()
{
    channelsNumber = 14;
    samplingRateValue = 128;
    verticesNumber = 1028;
    rawDataSizeVar = 32;


    channelNames.append(QString("P7"));
    channelNames.append(QString("FC6"));
    channelNames.append(QString("T7"));
    channelNames.append(QString("P8"));
    channelNames.append(QString("O2"));
    channelNames.append(QString("O1"));
    channelNames.append(QString("FC5"));
    channelNames.append(QString("F3"));
    channelNames.append(QString("F4"));
    channelNames.append(QString("T8"));
    channelNames.append(QString("F7"));
    channelNames.append(QString("F8"));
    channelNames.append(QString("AF4"));
    channelNames.append(QString("AF3"));
    channelNames.append(QString("CQ"));


    (channels["F3"]).append(10);
    (channels["F3"]).append(11);
    (channels["F3"]).append(12);
    (channels["F3"]).append(13);
    (channels["F3"]).append(14);
    (channels["F3"]).append(15);
    (channels["F3"]).append(0);
    (channels["F3"]).append(1);
    (channels["F3"]).append(2);
    (channels["F3"]).append(3);
    (channels["F3"]).append(4);
    (channels["F3"]).append(5);
    (channels["F3"]).append(6);
    (channels["F3"]).append(7);

    (channels["FC6"]).append(214);
    (channels["FC6"]).append(215);
    (channels["FC6"]).append(200);
    (channels["FC6"]).append(201);
    (channels["FC6"]).append(202);
    (channels["FC6"]).append(203);
    (channels["FC6"]).append(204);
    (channels["FC6"]).append(205);
    (channels["FC6"]).append(206);
    (channels["FC6"]).append(207);
    (channels["FC6"]).append(192);
    (channels["FC6"]).append(193);
    (channels["FC6"]).append(194);
    (channels["FC6"]).append(195);



    (channels["P7"]).append(84);
    (channels["P7"]).append(85);
    (channels["P7"]).append(86);
    (channels["P7"]).append(87);
    (channels["P7"]).append(72);
    (channels["P7"]).append(73);
    (channels["P7"]).append(74);
    (channels["P7"]).append(75);
    (channels["P7"]).append(76);
    (channels["P7"]).append(77);
    (channels["P7"]).append(78);
    (channels["P7"]).append(79);
    (channels["P7"]).append(64);
    (channels["P7"]).append(65);




    (channels["T8"]).append(160);
    (channels["T8"]).append(161);
    (channels["T8"]).append(162);
    (channels["T8"]).append(163);
    (channels["T8"]).append(164);
    (channels["T8"]).append(165);
    (channels["T8"]).append(166);
    (channels["T8"]).append(167);
    (channels["T8"]).append(152);
    (channels["T8"]).append(153);
    (channels["T8"]).append(154);
    (channels["T8"]).append(155);
    (channels["T8"]).append(156);
    (channels["T8"]).append(157);




    (channels["F7"]).append(48);
    (channels["F7"]).append(49);
    (channels["F7"]).append(50);
    (channels["F7"]).append(51);
    (channels["F7"]).append(52);
    (channels["F7"]).append(53);
    (channels["F7"]).append(54);
    (channels["F7"]).append(55);
    (channels["F7"]).append(40);
    (channels["F7"]).append(41);
    (channels["F7"]).append(42);
    (channels["F7"]).append(43);
    (channels["F7"]).append(44);
    (channels["F7"]).append(45);



    (channels["F8"]).append(178);
    (channels["F8"]).append(179);
    (channels["F8"]).append(180);
    (channels["F8"]).append(181);
    (channels["F8"]).append(182);
    (channels["F8"]).append(183);
    (channels["F8"]).append(168);
    (channels["F8"]).append(169);
    (channels["F8"]).append(170);
    (channels["F8"]).append(171);
    (channels["F8"]).append(172);
    (channels["F8"]).append(173);
    (channels["F8"]).append(174);
    (channels["F8"]).append(175);



    (channels["T7"]).append(66);
    (channels["T7"]).append(67);
    (channels["T7"]).append(68);
    (channels["T7"]).append(69);
    (channels["T7"]).append(70);
    (channels["T7"]).append(71);
    (channels["T7"]).append(56);
    (channels["T7"]).append(57);
    (channels["T7"]).append(58);
    (channels["T7"]).append(59);
    (channels["T7"]).append(60);
    (channels["T7"]).append(61);
    (channels["T7"]).append(62);
    (channels["T7"]).append(63);



    (channels["P8"]).append(158);
    (channels["P8"]).append(159);
    (channels["P8"]).append(144);
    (channels["P8"]).append(145);
    (channels["P8"]).append(146);
    (channels["P8"]).append(147);
    (channels["P8"]).append(148);
    (channels["P8"]).append(149);
    (channels["P8"]).append(150);
    (channels["P8"]).append(151);
    (channels["P8"]).append(136);
    (channels["P8"]).append(137);
    (channels["P8"]).append(138);
    (channels["P8"]).append(139);



    (channels["AF4"]).append(196);
    (channels["AF4"]).append(197);
    (channels["AF4"]).append(198);
    (channels["AF4"]).append(199);
    (channels["AF4"]).append(184);
    (channels["AF4"]).append(185);
    (channels["AF4"]).append(186);
    (channels["AF4"]).append(187);
    (channels["AF4"]).append(188);
    (channels["AF4"]).append(189);
    (channels["AF4"]).append(190);
    (channels["AF4"]).append(191);
    (channels["AF4"]).append(176);
    (channels["AF4"]).append(177);



    (channels["F4"]).append(216);
    (channels["F4"]).append(217);
    (channels["F4"]).append(218);
    (channels["F4"]).append(219);
    (channels["F4"]).append(220);
    (channels["F4"]).append(221);
    (channels["F4"]).append(222);
    (channels["F4"]).append(223);
    (channels["F4"]).append(208);
    (channels["F4"]).append(209);
    (channels["F4"]).append(210);
    (channels["F4"]).append(211);
    (channels["F4"]).append(212);
    (channels["F4"]).append(213);



    (channels["AF3"]).append(46);
    (channels["AF3"]).append(47);
    (channels["AF3"]).append(32);
    (channels["AF3"]).append(33);
    (channels["AF3"]).append(34);
    (channels["AF3"]).append(35);
    (channels["AF3"]).append(36);
    (channels["AF3"]).append(37);
    (channels["AF3"]).append(38);
    (channels["AF3"]).append(39);
    (channels["AF3"]).append(24);
    (channels["AF3"]).append(25);
    (channels["AF3"]).append(26);
    (channels["AF3"]).append(27);



    (channels["O2"]).append(140);
    (channels["O2"]).append(141);
    (channels["O2"]).append(142);
    (channels["O2"]).append(143);
    (channels["O2"]).append(128);
    (channels["O2"]).append(129);
    (channels["O2"]).append(130);
    (channels["O2"]).append(131);
    (channels["O2"]).append(132);
    (channels["O2"]).append(133);
    (channels["O2"]).append(134);
    (channels["O2"]).append(135);
    (channels["O2"]).append(120);
    (channels["O2"]).append(121);



    (channels["O1"]).append(102);
    (channels["O1"]).append(103);
    (channels["O1"]).append(88);
    (channels["O1"]).append(89);
    (channels["O1"]).append(90);
    (channels["O1"]).append(91);
    (channels["O1"]).append(92);
    (channels["O1"]).append(93);
    (channels["O1"]).append(94);
    (channels["O1"]).append(95);
    (channels["O1"]).append(80);
    (channels["O1"]).append(81);
    (channels["O1"]).append(82);
    (channels["O1"]).append(83);




    (channels["FC5"]).append(28);
    (channels["FC5"]).append(29);
    (channels["FC5"]).append(30);
    (channels["FC5"]).append(31);
    (channels["FC5"]).append(16);
    (channels["FC5"]).append(17);
    (channels["FC5"]).append(18);
    (channels["FC5"]).append(19);
    (channels["FC5"]).append(20);
    (channels["FC5"]).append(21);
    (channels["FC5"]).append(22);
    (channels["FC5"]).append(23);
    (channels["FC5"]).append(8);
    (channels["FC5"]).append(9);




    (channels["CQ"]).append(104);
    (channels["CQ"]).append(105);
    (channels["CQ"]).append(106);
    (channels["CQ"]).append(107);
    (channels["CQ"]).append(108);
    (channels["CQ"]).append(109);
    (channels["CQ"]).append(110);
    (channels["CQ"]).append(111);
    (channels["CQ"]).append(96);
    (channels["CQ"]).append(97);
    (channels["CQ"]).append(98);
    (channels["CQ"]).append(99);
    (channels["CQ"]).append(100);
    (channels["CQ"]).append(101);


    cqs["P7"]=0;
    cqs["FC6"]=0;
    cqs["T7"]=0;
    cqs["P8"]=0;
    cqs["O2"]=0;
    cqs["O1"]=0;
    cqs["FC5"]=0;
    cqs["F3"]=0;
    cqs["F4"]=0;
    cqs["T8"]=0;
    cqs["F7"]=0;
    cqs["F8"]=0;
    cqs["AF4"]=0;
    cqs["AF3"]=0;


    cqsMapping.push_back("F3");
    cqsMapping.push_back("FC5");
    cqsMapping.push_back("AF3");
    cqsMapping.push_back("F7");
    cqsMapping.push_back("T7");
    cqsMapping.push_back("P7");
    cqsMapping.push_back("O1");
    cqsMapping.push_back("O2");
    cqsMapping.push_back("P8");
    cqsMapping.push_back("T8");
    cqsMapping.push_back("F8");
    cqsMapping.push_back("AF4");
    cqsMapping.push_back("FC6");
    cqsMapping.push_back("F4");


}

void Sbs2Common::generateEmocap()
{
    channelsNumber = 14;
    samplingRateValue = 128;
    verticesNumber = 1028;
    rawDataSizeVar = 32;

    channelNames.append(QString("TP10"));
    channelNames.append(QString("FZ"));
    channelNames.append(QString("P3"));
    channelNames.append(QString("CZ"));
    channelNames.append(QString("C4"));
    channelNames.append(QString("TP9"));
    channelNames.append(QString("PZ"));
    channelNames.append(QString("P4"));
    channelNames.append(QString("F3"));
    channelNames.append(QString("C3"));
    channelNames.append(QString("O1"));
    channelNames.append(QString("F4"));
    channelNames.append(QString("FPZ"));
    channelNames.append(QString("O2"));
    channelNames.append(QString("CQ"));


    (channels["P4"]).append(10);
    (channels["P4"]).append(11);
    (channels["P4"]).append(12);
    (channels["P4"]).append(13);
    (channels["P4"]).append(14);
    (channels["P4"]).append(15);
    (channels["P4"]).append(0);
    (channels["P4"]).append(1);
    (channels["P4"]).append(2);
    (channels["P4"]).append(3);
    (channels["P4"]).append(4);
    (channels["P4"]).append(5);
    (channels["P4"]).append(6);
    (channels["P4"]).append(7);

    (channels["FZ"]).append(214);
    (channels["FZ"]).append(215);
    (channels["FZ"]).append(200);
    (channels["FZ"]).append(201);
    (channels["FZ"]).append(202);
    (channels["FZ"]).append(203);
    (channels["FZ"]).append(204);
    (channels["FZ"]).append(205);
    (channels["FZ"]).append(206);
    (channels["FZ"]).append(207);
    (channels["FZ"]).append(192);
    (channels["FZ"]).append(193);
    (channels["FZ"]).append(194);
    (channels["FZ"]).append(195);



    (channels["TP10"]).append(84);
    (channels["TP10"]).append(85);
    (channels["TP10"]).append(86);
    (channels["TP10"]).append(87);
    (channels["TP10"]).append(72);
    (channels["TP10"]).append(73);
    (channels["TP10"]).append(74);
    (channels["TP10"]).append(75);
    (channels["TP10"]).append(76);
    (channels["TP10"]).append(77);
    (channels["TP10"]).append(78);
    (channels["TP10"]).append(79);
    (channels["TP10"]).append(64);
    (channels["TP10"]).append(65);




    (channels["C3"]).append(160);
    (channels["C3"]).append(161);
    (channels["C3"]).append(162);
    (channels["C3"]).append(163);
    (channels["C3"]).append(164);
    (channels["C3"]).append(165);
    (channels["C3"]).append(166);
    (channels["C3"]).append(167);
    (channels["C3"]).append(152);
    (channels["C3"]).append(153);
    (channels["C3"]).append(154);
    (channels["C3"]).append(155);
    (channels["C3"]).append(156);
    (channels["C3"]).append(157);




    (channels["O1"]).append(48);
    (channels["O1"]).append(49);
    (channels["O1"]).append(50);
    (channels["O1"]).append(51);
    (channels["O1"]).append(52);
    (channels["O1"]).append(53);
    (channels["O1"]).append(54);
    (channels["O1"]).append(55);
    (channels["O1"]).append(40);
    (channels["O1"]).append(41);
    (channels["O1"]).append(42);
    (channels["O1"]).append(43);
    (channels["O1"]).append(44);
    (channels["O1"]).append(45);



    (channels["F4"]).append(178);
    (channels["F4"]).append(179);
    (channels["F4"]).append(180);
    (channels["F4"]).append(181);
    (channels["F4"]).append(182);
    (channels["F4"]).append(183);
    (channels["F4"]).append(168);
    (channels["F4"]).append(169);
    (channels["F4"]).append(170);
    (channels["F4"]).append(171);
    (channels["F4"]).append(172);
    (channels["F4"]).append(173);
    (channels["F4"]).append(174);
    (channels["F4"]).append(175);



    (channels["P3"]).append(66);
    (channels["P3"]).append(67);
    (channels["P3"]).append(68);
    (channels["P3"]).append(69);
    (channels["P3"]).append(70);
    (channels["P3"]).append(71);
    (channels["P3"]).append(56);
    (channels["P3"]).append(57);
    (channels["P3"]).append(58);
    (channels["P3"]).append(59);
    (channels["P3"]).append(60);
    (channels["P3"]).append(61);
    (channels["P3"]).append(62);
    (channels["P3"]).append(63);



    (channels["CZ"]).append(158);
    (channels["CZ"]).append(159);
    (channels["CZ"]).append(144);
    (channels["CZ"]).append(145);
    (channels["CZ"]).append(146);
    (channels["CZ"]).append(147);
    (channels["CZ"]).append(148);
    (channels["CZ"]).append(149);
    (channels["CZ"]).append(150);
    (channels["CZ"]).append(151);
    (channels["CZ"]).append(136);
    (channels["CZ"]).append(137);
    (channels["CZ"]).append(138);
    (channels["CZ"]).append(139);



    (channels["FPZ"]).append(196);
    (channels["FPZ"]).append(197);
    (channels["FPZ"]).append(198);
    (channels["FPZ"]).append(199);
    (channels["FPZ"]).append(184);
    (channels["FPZ"]).append(185);
    (channels["FPZ"]).append(186);
    (channels["FPZ"]).append(187);
    (channels["FPZ"]).append(188);
    (channels["FPZ"]).append(189);
    (channels["FPZ"]).append(190);
    (channels["FPZ"]).append(191);
    (channels["FPZ"]).append(176);
    (channels["FPZ"]).append(177);



    (channels["F3"]).append(216);
    (channels["F3"]).append(217);
    (channels["F3"]).append(218);
    (channels["F3"]).append(219);
    (channels["F3"]).append(220);
    (channels["F3"]).append(221);
    (channels["F3"]).append(222);
    (channels["F3"]).append(223);
    (channels["F3"]).append(208);
    (channels["F3"]).append(209);
    (channels["F3"]).append(210);
    (channels["F3"]).append(211);
    (channels["F3"]).append(212);
    (channels["F3"]).append(213);



    (channels["O2"]).append(46);
    (channels["O2"]).append(47);
    (channels["O2"]).append(32);
    (channels["O2"]).append(33);
    (channels["O2"]).append(34);
    (channels["O2"]).append(35);
    (channels["O2"]).append(36);
    (channels["O2"]).append(37);
    (channels["O2"]).append(38);
    (channels["O2"]).append(39);
    (channels["O2"]).append(24);
    (channels["O2"]).append(25);
    (channels["O2"]).append(26);
    (channels["O2"]).append(27);



    (channels["C4"]).append(140);
    (channels["C4"]).append(141);
    (channels["C4"]).append(142);
    (channels["C4"]).append(143);
    (channels["C4"]).append(128);
    (channels["C4"]).append(129);
    (channels["C4"]).append(130);
    (channels["C4"]).append(131);
    (channels["C4"]).append(132);
    (channels["C4"]).append(133);
    (channels["C4"]).append(134);
    (channels["C4"]).append(135);
    (channels["C4"]).append(120);
    (channels["C4"]).append(121);



    (channels["TP9"]).append(102);
    (channels["TP9"]).append(103);
    (channels["TP9"]).append(88);
    (channels["TP9"]).append(89);
    (channels["TP9"]).append(90);
    (channels["TP9"]).append(91);
    (channels["TP9"]).append(92);
    (channels["TP9"]).append(93);
    (channels["TP9"]).append(94);
    (channels["TP9"]).append(95);
    (channels["TP9"]).append(80);
    (channels["TP9"]).append(81);
    (channels["TP9"]).append(82);
    (channels["TP9"]).append(83);




    (channels["PZ"]).append(28);
    (channels["PZ"]).append(29);
    (channels["PZ"]).append(30);
    (channels["PZ"]).append(31);
    (channels["PZ"]).append(16);
    (channels["PZ"]).append(17);
    (channels["PZ"]).append(18);
    (channels["PZ"]).append(19);
    (channels["PZ"]).append(20);
    (channels["PZ"]).append(21);
    (channels["PZ"]).append(22);
    (channels["PZ"]).append(23);
    (channels["PZ"]).append(8);
    (channels["PZ"]).append(9);




    (channels["CQ"]).append(104);
    (channels["CQ"]).append(105);
    (channels["CQ"]).append(106);
    (channels["CQ"]).append(107);
    (channels["CQ"]).append(108);
    (channels["CQ"]).append(109);
    (channels["CQ"]).append(110);
    (channels["CQ"]).append(111);
    (channels["CQ"]).append(96);
    (channels["CQ"]).append(97);
    (channels["CQ"]).append(98);
    (channels["CQ"]).append(99);
    (channels["CQ"]).append(100);
    (channels["CQ"]).append(101);


    cqs["TP10"]=0;
    cqs["FZ"]=0;
    cqs["P3"]=0;
    cqs["CZ"]=0;
    cqs["C4"]=0;
    cqs["TP9"]=0;
    cqs["PZ"]=0;
    cqs["P4"]=0;
    cqs["F3"]=0;
    cqs["C3"]=0;
    cqs["O1"]=0;
    cqs["F4"]=0;
    cqs["FPZ"]=0;
    cqs["O2"]=0;


    cqsMapping.push_back("P4");
    cqsMapping.push_back("PZ");
    cqsMapping.push_back("O2");
    cqsMapping.push_back("O1");
    cqsMapping.push_back("P3");
    cqsMapping.push_back("TP10");
    cqsMapping.push_back("TP9");
    cqsMapping.push_back("C4");
    cqsMapping.push_back("CZ");
    cqsMapping.push_back("C3");
    cqsMapping.push_back("F4");
    cqsMapping.push_back("FPZ");
    cqsMapping.push_back("FZ");
    cqsMapping.push_back("F3");


}

void Sbs2Common::generateEmocap28()
{
    channelsNumber = 28;
    samplingRateValue = 128;
    verticesNumber = 1028;
    rawDataSizeVar = 64;

    channelNames.append(QString("TP10"));
    channelNames.append(QString("FZ"));
    channelNames.append(QString("P3"));
    channelNames.append(QString("CZ"));
    channelNames.append(QString("C4"));
    channelNames.append(QString("TP9"));
    channelNames.append(QString("PZ"));
    channelNames.append(QString("P4"));
    channelNames.append(QString("F3"));
    channelNames.append(QString("C3"));
    channelNames.append(QString("O1"));
    channelNames.append(QString("F4"));
    channelNames.append(QString("FPZ"));
    channelNames.append(QString("O2"));


    channelNames.append(QString("TP10_2"));
    channelNames.append(QString("FZ_2"));
    channelNames.append(QString("P3_2"));
    channelNames.append(QString("CZ_2"));
    channelNames.append(QString("C4_2"));
    channelNames.append(QString("TP9_2"));
    channelNames.append(QString("PZ_2"));
    channelNames.append(QString("P4_2"));
    channelNames.append(QString("F3_2"));
    channelNames.append(QString("C3_2"));
    channelNames.append(QString("O1_2"));
    channelNames.append(QString("F4_2"));
    channelNames.append(QString("FPZ_2"));
    channelNames.append(QString("O2_2"));

    channelNames.append(QString("CQ"));
    channelNames.append(QString("CQ_2"));


    (channels["P4"]).append(10);
    (channels["P4"]).append(11);
    (channels["P4"]).append(12);
    (channels["P4"]).append(13);
    (channels["P4"]).append(14);
    (channels["P4"]).append(15);
    (channels["P4"]).append(0);
    (channels["P4"]).append(1);
    (channels["P4"]).append(2);
    (channels["P4"]).append(3);
    (channels["P4"]).append(4);
    (channels["P4"]).append(5);
    (channels["P4"]).append(6);
    (channels["P4"]).append(7);

    (channels["FZ"]).append(214);
    (channels["FZ"]).append(215);
    (channels["FZ"]).append(200);
    (channels["FZ"]).append(201);
    (channels["FZ"]).append(202);
    (channels["FZ"]).append(203);
    (channels["FZ"]).append(204);
    (channels["FZ"]).append(205);
    (channels["FZ"]).append(206);
    (channels["FZ"]).append(207);
    (channels["FZ"]).append(192);
    (channels["FZ"]).append(193);
    (channels["FZ"]).append(194);
    (channels["FZ"]).append(195);



    (channels["TP10"]).append(84);
    (channels["TP10"]).append(85);
    (channels["TP10"]).append(86);
    (channels["TP10"]).append(87);
    (channels["TP10"]).append(72);
    (channels["TP10"]).append(73);
    (channels["TP10"]).append(74);
    (channels["TP10"]).append(75);
    (channels["TP10"]).append(76);
    (channels["TP10"]).append(77);
    (channels["TP10"]).append(78);
    (channels["TP10"]).append(79);
    (channels["TP10"]).append(64);
    (channels["TP10"]).append(65);




    (channels["C3"]).append(160);
    (channels["C3"]).append(161);
    (channels["C3"]).append(162);
    (channels["C3"]).append(163);
    (channels["C3"]).append(164);
    (channels["C3"]).append(165);
    (channels["C3"]).append(166);
    (channels["C3"]).append(167);
    (channels["C3"]).append(152);
    (channels["C3"]).append(153);
    (channels["C3"]).append(154);
    (channels["C3"]).append(155);
    (channels["C3"]).append(156);
    (channels["C3"]).append(157);




    (channels["O1"]).append(48);
    (channels["O1"]).append(49);
    (channels["O1"]).append(50);
    (channels["O1"]).append(51);
    (channels["O1"]).append(52);
    (channels["O1"]).append(53);
    (channels["O1"]).append(54);
    (channels["O1"]).append(55);
    (channels["O1"]).append(40);
    (channels["O1"]).append(41);
    (channels["O1"]).append(42);
    (channels["O1"]).append(43);
    (channels["O1"]).append(44);
    (channels["O1"]).append(45);



    (channels["F4"]).append(178);
    (channels["F4"]).append(179);
    (channels["F4"]).append(180);
    (channels["F4"]).append(181);
    (channels["F4"]).append(182);
    (channels["F4"]).append(183);
    (channels["F4"]).append(168);
    (channels["F4"]).append(169);
    (channels["F4"]).append(170);
    (channels["F4"]).append(171);
    (channels["F4"]).append(172);
    (channels["F4"]).append(173);
    (channels["F4"]).append(174);
    (channels["F4"]).append(175);



    (channels["P3"]).append(66);
    (channels["P3"]).append(67);
    (channels["P3"]).append(68);
    (channels["P3"]).append(69);
    (channels["P3"]).append(70);
    (channels["P3"]).append(71);
    (channels["P3"]).append(56);
    (channels["P3"]).append(57);
    (channels["P3"]).append(58);
    (channels["P3"]).append(59);
    (channels["P3"]).append(60);
    (channels["P3"]).append(61);
    (channels["P3"]).append(62);
    (channels["P3"]).append(63);



    (channels["CZ"]).append(158);
    (channels["CZ"]).append(159);
    (channels["CZ"]).append(144);
    (channels["CZ"]).append(145);
    (channels["CZ"]).append(146);
    (channels["CZ"]).append(147);
    (channels["CZ"]).append(148);
    (channels["CZ"]).append(149);
    (channels["CZ"]).append(150);
    (channels["CZ"]).append(151);
    (channels["CZ"]).append(136);
    (channels["CZ"]).append(137);
    (channels["CZ"]).append(138);
    (channels["CZ"]).append(139);



    (channels["FPZ"]).append(196);
    (channels["FPZ"]).append(197);
    (channels["FPZ"]).append(198);
    (channels["FPZ"]).append(199);
    (channels["FPZ"]).append(184);
    (channels["FPZ"]).append(185);
    (channels["FPZ"]).append(186);
    (channels["FPZ"]).append(187);
    (channels["FPZ"]).append(188);
    (channels["FPZ"]).append(189);
    (channels["FPZ"]).append(190);
    (channels["FPZ"]).append(191);
    (channels["FPZ"]).append(176);
    (channels["FPZ"]).append(177);



    (channels["F3"]).append(216);
    (channels["F3"]).append(217);
    (channels["F3"]).append(218);
    (channels["F3"]).append(219);
    (channels["F3"]).append(220);
    (channels["F3"]).append(221);
    (channels["F3"]).append(222);
    (channels["F3"]).append(223);
    (channels["F3"]).append(208);
    (channels["F3"]).append(209);
    (channels["F3"]).append(210);
    (channels["F3"]).append(211);
    (channels["F3"]).append(212);
    (channels["F3"]).append(213);



    (channels["O2"]).append(46);
    (channels["O2"]).append(47);
    (channels["O2"]).append(32);
    (channels["O2"]).append(33);
    (channels["O2"]).append(34);
    (channels["O2"]).append(35);
    (channels["O2"]).append(36);
    (channels["O2"]).append(37);
    (channels["O2"]).append(38);
    (channels["O2"]).append(39);
    (channels["O2"]).append(24);
    (channels["O2"]).append(25);
    (channels["O2"]).append(26);
    (channels["O2"]).append(27);



    (channels["C4"]).append(140);
    (channels["C4"]).append(141);
    (channels["C4"]).append(142);
    (channels["C4"]).append(143);
    (channels["C4"]).append(128);
    (channels["C4"]).append(129);
    (channels["C4"]).append(130);
    (channels["C4"]).append(131);
    (channels["C4"]).append(132);
    (channels["C4"]).append(133);
    (channels["C4"]).append(134);
    (channels["C4"]).append(135);
    (channels["C4"]).append(120);
    (channels["C4"]).append(121);



    (channels["TP9"]).append(102);
    (channels["TP9"]).append(103);
    (channels["TP9"]).append(88);
    (channels["TP9"]).append(89);
    (channels["TP9"]).append(90);
    (channels["TP9"]).append(91);
    (channels["TP9"]).append(92);
    (channels["TP9"]).append(93);
    (channels["TP9"]).append(94);
    (channels["TP9"]).append(95);
    (channels["TP9"]).append(80);
    (channels["TP9"]).append(81);
    (channels["TP9"]).append(82);
    (channels["TP9"]).append(83);




    (channels["PZ"]).append(28);
    (channels["PZ"]).append(29);
    (channels["PZ"]).append(30);
    (channels["PZ"]).append(31);
    (channels["PZ"]).append(16);
    (channels["PZ"]).append(17);
    (channels["PZ"]).append(18);
    (channels["PZ"]).append(19);
    (channels["PZ"]).append(20);
    (channels["PZ"]).append(21);
    (channels["PZ"]).append(22);
    (channels["PZ"]).append(23);
    (channels["PZ"]).append(8);
    (channels["PZ"]).append(9);




    (channels["CQ"]).append(104);
    (channels["CQ"]).append(105);
    (channels["CQ"]).append(106);
    (channels["CQ"]).append(107);
    (channels["CQ"]).append(108);
    (channels["CQ"]).append(109);
    (channels["CQ"]).append(110);
    (channels["CQ"]).append(111);
    (channels["CQ"]).append(96);
    (channels["CQ"]).append(97);
    (channels["CQ"]).append(98);
    (channels["CQ"]).append(99);
    (channels["CQ"]).append(100);
    (channels["CQ"]).append(101);


    (channels["P4_2"]).append(10);
    (channels["P4_2"]).append(11);
    (channels["P4_2"]).append(12);
    (channels["P4_2"]).append(13);
    (channels["P4_2"]).append(14);
    (channels["P4_2"]).append(15);
    (channels["P4_2"]).append(0);
    (channels["P4_2"]).append(1);
    (channels["P4_2"]).append(2);
    (channels["P4_2"]).append(3);
    (channels["P4_2"]).append(4);
    (channels["P4_2"]).append(5);
    (channels["P4_2"]).append(6);
    (channels["P4_2"]).append(7);

    (channels["FZ_2"]).append(214);
    (channels["FZ_2"]).append(215);
    (channels["FZ_2"]).append(200);
    (channels["FZ_2"]).append(201);
    (channels["FZ_2"]).append(202);
    (channels["FZ_2"]).append(203);
    (channels["FZ_2"]).append(204);
    (channels["FZ_2"]).append(205);
    (channels["FZ_2"]).append(206);
    (channels["FZ_2"]).append(207);
    (channels["FZ_2"]).append(192);
    (channels["FZ_2"]).append(193);
    (channels["FZ_2"]).append(194);
    (channels["FZ_2"]).append(195);



    (channels["TP10_2"]).append(84);
    (channels["TP10_2"]).append(85);
    (channels["TP10_2"]).append(86);
    (channels["TP10_2"]).append(87);
    (channels["TP10_2"]).append(72);
    (channels["TP10_2"]).append(73);
    (channels["TP10_2"]).append(74);
    (channels["TP10_2"]).append(75);
    (channels["TP10_2"]).append(76);
    (channels["TP10_2"]).append(77);
    (channels["TP10_2"]).append(78);
    (channels["TP10_2"]).append(79);
    (channels["TP10_2"]).append(64);
    (channels["TP10_2"]).append(65);




    (channels["C3_2"]).append(160);
    (channels["C3_2"]).append(161);
    (channels["C3_2"]).append(162);
    (channels["C3_2"]).append(163);
    (channels["C3_2"]).append(164);
    (channels["C3_2"]).append(165);
    (channels["C3_2"]).append(166);
    (channels["C3_2"]).append(167);
    (channels["C3_2"]).append(152);
    (channels["C3_2"]).append(153);
    (channels["C3_2"]).append(154);
    (channels["C3_2"]).append(155);
    (channels["C3_2"]).append(156);
    (channels["C3_2"]).append(157);




    (channels["O1_2"]).append(48);
    (channels["O1_2"]).append(49);
    (channels["O1_2"]).append(50);
    (channels["O1_2"]).append(51);
    (channels["O1_2"]).append(52);
    (channels["O1_2"]).append(53);
    (channels["O1_2"]).append(54);
    (channels["O1_2"]).append(55);
    (channels["O1_2"]).append(40);
    (channels["O1_2"]).append(41);
    (channels["O1_2"]).append(42);
    (channels["O1_2"]).append(43);
    (channels["O1_2"]).append(44);
    (channels["O1_2"]).append(45);



    (channels["F4_2"]).append(178);
    (channels["F4_2"]).append(179);
    (channels["F4_2"]).append(180);
    (channels["F4_2"]).append(181);
    (channels["F4_2"]).append(182);
    (channels["F4_2"]).append(183);
    (channels["F4_2"]).append(168);
    (channels["F4_2"]).append(169);
    (channels["F4_2"]).append(170);
    (channels["F4_2"]).append(171);
    (channels["F4_2"]).append(172);
    (channels["F4_2"]).append(173);
    (channels["F4_2"]).append(174);
    (channels["F4_2"]).append(175);



    (channels["P3_2"]).append(66);
    (channels["P3_2"]).append(67);
    (channels["P3_2"]).append(68);
    (channels["P3_2"]).append(69);
    (channels["P3_2"]).append(70);
    (channels["P3_2"]).append(71);
    (channels["P3_2"]).append(56);
    (channels["P3_2"]).append(57);
    (channels["P3_2"]).append(58);
    (channels["P3_2"]).append(59);
    (channels["P3_2"]).append(60);
    (channels["P3_2"]).append(61);
    (channels["P3_2"]).append(62);
    (channels["P3_2"]).append(63);



    (channels["CZ_2"]).append(158);
    (channels["CZ_2"]).append(159);
    (channels["CZ_2"]).append(144);
    (channels["CZ_2"]).append(145);
    (channels["CZ_2"]).append(146);
    (channels["CZ_2"]).append(147);
    (channels["CZ_2"]).append(148);
    (channels["CZ_2"]).append(149);
    (channels["CZ_2"]).append(150);
    (channels["CZ_2"]).append(151);
    (channels["CZ_2"]).append(136);
    (channels["CZ_2"]).append(137);
    (channels["CZ_2"]).append(138);
    (channels["CZ_2"]).append(139);



    (channels["FPZ_2"]).append(196);
    (channels["FPZ_2"]).append(197);
    (channels["FPZ_2"]).append(198);
    (channels["FPZ_2"]).append(199);
    (channels["FPZ_2"]).append(184);
    (channels["FPZ_2"]).append(185);
    (channels["FPZ_2"]).append(186);
    (channels["FPZ_2"]).append(187);
    (channels["FPZ_2"]).append(188);
    (channels["FPZ_2"]).append(189);
    (channels["FPZ_2"]).append(190);
    (channels["FPZ_2"]).append(191);
    (channels["FPZ_2"]).append(176);
    (channels["FPZ_2"]).append(177);



    (channels["F3_2"]).append(216);
    (channels["F3_2"]).append(217);
    (channels["F3_2"]).append(218);
    (channels["F3_2"]).append(219);
    (channels["F3_2"]).append(220);
    (channels["F3_2"]).append(221);
    (channels["F3_2"]).append(222);
    (channels["F3_2"]).append(223);
    (channels["F3_2"]).append(208);
    (channels["F3_2"]).append(209);
    (channels["F3_2"]).append(210);
    (channels["F3_2"]).append(211);
    (channels["F3_2"]).append(212);
    (channels["F3_2"]).append(213);



    (channels["C4_2"]).append(140);
    (channels["C4_2"]).append(141);
    (channels["C4_2"]).append(142);
    (channels["C4_2"]).append(143);
    (channels["C4_2"]).append(128);
    (channels["C4_2"]).append(129);
    (channels["C4_2"]).append(130);
    (channels["C4_2"]).append(131);
    (channels["C4_2"]).append(132);
    (channels["C4_2"]).append(133);
    (channels["C4_2"]).append(134);
    (channels["C4_2"]).append(135);
    (channels["C4_2"]).append(120);
    (channels["C4_2"]).append(121);



    (channels["TP9_2"]).append(102);
    (channels["TP9_2"]).append(103);
    (channels["TP9_2"]).append(88);
    (channels["TP9_2"]).append(89);
    (channels["TP9_2"]).append(90);
    (channels["TP9_2"]).append(91);
    (channels["TP9_2"]).append(92);
    (channels["TP9_2"]).append(93);
    (channels["TP9_2"]).append(94);
    (channels["TP9_2"]).append(95);
    (channels["TP9_2"]).append(80);
    (channels["TP9_2"]).append(81);
    (channels["TP9_2"]).append(82);
    (channels["TP9_2"]).append(83);




    (channels["PZ_2"]).append(28);
    (channels["PZ_2"]).append(29);
    (channels["PZ_2"]).append(30);
    (channels["PZ_2"]).append(31);
    (channels["PZ_2"]).append(16);
    (channels["PZ_2"]).append(17);
    (channels["PZ_2"]).append(18);
    (channels["PZ_2"]).append(19);
    (channels["PZ_2"]).append(20);
    (channels["PZ_2"]).append(21);
    (channels["PZ_2"]).append(22);
    (channels["PZ_2"]).append(23);
    (channels["PZ_2"]).append(8);
    (channels["PZ_2"]).append(9);




    (channels["CQ_2"]).append(104);
    (channels["CQ_2"]).append(105);
    (channels["CQ_2"]).append(106);
    (channels["CQ_2"]).append(107);
    (channels["CQ_2"]).append(108);
    (channels["CQ_2"]).append(109);
    (channels["CQ_2"]).append(110);
    (channels["CQ_2"]).append(111);
    (channels["CQ_2"]).append(96);
    (channels["CQ_2"]).append(97);
    (channels["CQ_2"]).append(98);
    (channels["CQ_2"]).append(99);
    (channels["CQ_2"]).append(100);
    (channels["CQ_2"]).append(101);

    (channels["O2_2"]).append(46);
    (channels["O2_2"]).append(47);
    (channels["O2_2"]).append(32);
    (channels["O2_2"]).append(33);
    (channels["O2_2"]).append(34);
    (channels["O2_2"]).append(35);
    (channels["O2_2"]).append(36);
    (channels["O2_2"]).append(37);
    (channels["O2_2"]).append(38);
    (channels["O2_2"]).append(39);
    (channels["O2_2"]).append(24);
    (channels["O2_2"]).append(25);
    (channels["O2_2"]).append(26);
    (channels["O2_2"]).append(27);



    cqs["TP10"]=0;
    cqs["FZ"]=0;
    cqs["P3"]=0;
    cqs["CZ"]=0;
    cqs["C4"]=0;
    cqs["TP9"]=0;
    cqs["PZ"]=0;
    cqs["P4"]=0;
    cqs["F3"]=0;
    cqs["C3"]=0;
    cqs["O1"]=0;
    cqs["F4"]=0;
    cqs["FPZ"]=0;
    cqs["O2"]=0;

    cqs["TP10_2"]=0;
    cqs["FZ_2"]=0;
    cqs["P3_2"]=0;
    cqs["CZ_2"]=0;
    cqs["C4_2"]=0;
    cqs["TP9_2"]=0;
    cqs["PZ_2"]=0;
    cqs["P4_2"]=0;
    cqs["F3_2"]=0;
    cqs["C3_2"]=0;
    cqs["O1_2"]=0;
    cqs["F4_2"]=0;
    cqs["FPZ_2"]=0;

    cqsMapping.push_back("P4");
    cqsMapping.push_back("PZ");
    cqsMapping.push_back("O2");
    cqsMapping.push_back("O1");
    cqsMapping.push_back("P3");
    cqsMapping.push_back("TP10");
    cqsMapping.push_back("TP9");
    cqsMapping.push_back("C4");
    cqsMapping.push_back("CZ");
    cqsMapping.push_back("C3");
    cqsMapping.push_back("F4");
    cqsMapping.push_back("FPZ");
    cqsMapping.push_back("FZ");
    cqsMapping.push_back("F3");

}


void Sbs2Common::generateHardware()
{

    channelNames.clear();
    channels.clear();
    cqs.clear();
    cqsMapping.clear();

    if (hardware == "emotiv")
    {
	generateEmotiv();
    }
    if (hardware == "emocap")
    {
	generateEmocap();
    }
    if (hardware == "emocap28")
    {
	generateEmocap28();
    }

}


Sbs2Common::Sbs2Common()
{
    //Default hardware
    if (!hardwareSet)
	setHardware("emotiv",this);

}



QMap<QString, QVector<int> >* Sbs2Common::getChannels()
{
    return &(Sbs2Common::New()->channels);
}

QVector<QString>* Sbs2Common::getChannelNames()
{
    return &(Sbs2Common::New()->channelNames);
}

QMap<QString,int>* Sbs2Common::getCqs()
{
    return &(Sbs2Common::New()->cqs);
}

QVector<QString>* Sbs2Common::getCqsMapping()
{
    return &(Sbs2Common::New()->cqsMapping);
}

int Sbs2Common::channelsNo()
{
    return Sbs2Common::New()->channelsNumber;
}

int Sbs2Common::samplingRate()
{
    return Sbs2Common::New()->samplingRateValue;
}

int Sbs2Common::verticesNo()
{
    return Sbs2Common::New()->verticesNumber;
}

int Sbs2Common::rawDataSize()
{
    return Sbs2Common::New()->rawDataSizeVar;
}

Sbs2Common* Sbs2Common::New()
{
    if (!m_pInstance)
	m_pInstance = new Sbs2Common();

    return m_pInstance;
}


QString Sbs2Common::setRootAppPath(QString rootAppPath_)
{
    if (rootAppPath_.right(1) != "/")
	rootAppPath_.append("/");
    Sbs2Common::New()->rootAppPath = rootAppPath_;
    return Sbs2Common::New()->rootAppPath;
}

QString Sbs2Common::getRootAppPath()
{
    return Sbs2Common::New()->rootAppPath;
}

QString Sbs2Common::setCatalogPath(QString catalogPath_)
{
    if (catalogPath_.right(1) != "/")
	catalogPath_.append("/");
    Sbs2Common::New()->catalogPath = catalogPath_;
    return Sbs2Common::New()->catalogPath;
}

QString Sbs2Common::getCatalogPath()
{
    return Sbs2Common::New()->catalogPath;
}

QString Sbs2Common::setDefaultRootAppPath()
{
#ifdef Q_OS_LINUX
    Sbs2Common::setRootAppPath(QDir::toNativeSeparators(QDir::homePath())+"/smartphonebrainscanner2_data/");
#endif
#ifdef Q_OS_ANDROID
    Sbs2Common::setRootAppPath("/sdcard/smartphonebrainscanner2_data/");
#endif
#if defined(Q_WS_MAEMO_5)
    Sbs2Common::setRootAppPath("/home/user/smartphonebrainscanner2_data/");
#endif
#ifdef Q_OS_MAC

    if (DEPLOYMENT)
	Sbs2Common::setRootAppPath("sbs2brain3d.app/Contents/Resources/smartphonebrainscanner2_data/");
    else
	Sbs2Common::setRootAppPath(QDir::toNativeSeparators(QDir::homePath())+"/smartphonebrainscanner2_data/");

#endif
#if defined(MEEGO_EDITION_HARMATTAN)
    Sbs2Common::setRootAppPath(QDir::toNativeSeparators(QDir::homePath())+"/MyDocs/smartphonebrainscanner2_data/");
#endif

    return Sbs2Common::getRootAppPath();
}

QString Sbs2Common::setDefaultCatalogPath()
{
#ifdef Q_WS_MAEMO_5
    Sbs2Common::setCatalogPath("/home/user/MyDocs/smartphonebrainscanner2_readings/");
#elif defined(Q_OS_ANDROID)
    Sbs2Common::setCatalogPath("/sdcard/smartphonebrainscanner2_readings/");
#else
    Sbs2Common::setCatalogPath(QDir::toNativeSeparators(QDir::homePath())+"/smartphonebrainscanner2_readings/");
#endif

    return Sbs2Common::getCatalogPath();
}


