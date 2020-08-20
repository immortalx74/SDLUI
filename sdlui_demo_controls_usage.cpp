SDLUI_Window(wnd1);
SDLUI_Window(wnd2);
SDLUI_Window(wnd3);
if(SDLUI_Button(btn1)) {std::cout << "btn1" << std::endl;}
if(SDLUI_SliderInt(si1)) {std::cout << si1->value << std::endl;}
if(SDLUI_SliderInt(si2)) {std::cout << si2->value << std::endl;}
if(SDLUI_CheckBox(chk1)) {std::cout << "chk1" << std::endl;}
if(SDLUI_Button(btn2)) {std::cout << "btn2" << std::endl;}
if(SDLUI_CheckBox(chk2)) {std::cout << "chk2" << std::endl;}
SDLUI_Text(txt1);
SDLUI_ScrollArea(sa);

if(SDLUI_RadioButton(rb1)) {std::cout << "rb1" << std::endl;}
if(SDLUI_RadioButton(rb2)) {std::cout << "rb2" << std::endl;}
if(SDLUI_RadioButton(rb3)) {std::cout << "rb3" << std::endl;}
if(SDLUI_RadioButton(rb4)) {std::cout << "rb4" << std::endl;}
SDLUI_RadioButton(rb5);
SDLUI_RadioButton(rb6);

if(SDLUI_ToggleButton(tb1)) {std::cout << "tb1" << std::endl;}
if(SDLUI_Button(btn3)) {std::cout << "btn3" << std::endl;}
(SDLUI_TabContainer(tbc1));