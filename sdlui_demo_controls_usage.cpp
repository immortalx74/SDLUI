SDLUI_Window(wnd1);
SDLUI_Window(wnd2);
SDLUI_Window(wnd3);
if(SDLUI_Button(btn1)){std::cout << "button" << std::endl;;}
SDLUI_SliderInt(si);
SDLUI_SliderInt(si2);
SDLUI_CheckBox(chk1);
SDLUI_Button(btn2);
if(SDLUI_CheckBox(chk2)) {std::cout << "check" << std::endl;}
SDLUI_Text(txt1);
SDLUI_RadioButton(rb1);
SDLUI_RadioButton(rb2);
SDLUI_RadioButton(rb3);
SDLUI_RadioButton(rb4);
SDLUI_RadioButton(rb5);
SDLUI_RadioButton(rb6);

if(SDLUI_ToggleButton(tb1)) {std::cout << "toggle" << std::endl;}
if(SDLUI_Button(btn3)) {std::cout << "btn3" << std::endl;}
(SDLUI_TabContainer(tbc1));