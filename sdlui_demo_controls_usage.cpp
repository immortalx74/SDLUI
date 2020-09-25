// We call each controls' usage function. Most of them return a boolean representing user interaction.
// (a click of a button, a slider's change in value, etc.)

SDLUI_Window(wnd1);
SDLUI_Window(wnd2);
SDLUI_Window(wnd3);
SDLUI_Window(wnd4);

if(SDLUI_Button(btn1)) {std::cout << "btn1" << std::endl;}
if(SDLUI_TextBox(tbx1)) {std::cout << "tbx1" << std::endl;}
if(SDLUI_SliderInt(si1)) {std::cout << si1->value << std::endl;}
if(SDLUI_SliderInt(si2)) {std::cout << si2->value << std::endl;}

SDLUI_ScrollArea(sa1);

// This is how we "feed" the list with elements. It's simply a loop where we pass the element caption,
// the total count of elements, and current iteration index.
for (int i = 0; i < list_items.size(); ++i)
{
	if(SDLUI_List(lst1, list_items[i].c_str(), list_items.size(), i))
	{
		std::cout << lst1->selected_index << std::endl;
	}
}

if(SDLUI_CheckBox(chk1)) {std::cout << "chk1" << std::endl;}

if(SDLUI_Button(btn2)) {std::cout << "btn2" << std::endl;}
if(SDLUI_CheckBox(chk2)) {std::cout << "chk2" << std::endl;}
if(SDLUI_ScrollArea(sa2)) {;}

if(SDLUI_RadioButton(rb1)) {std::cout << "rb1" << std::endl;}
if(SDLUI_RadioButton(rb2)) {std::cout << "rb2" << std::endl;}
if(SDLUI_RadioButton(rb3)) {std::cout << "rb3" << std::endl;}
if(SDLUI_RadioButton(rb4)) {std::cout << "rb4" << std::endl;}
if(SDLUI_RadioButton(rb5)) {std::cout << "rb5" << std::endl;}
if(SDLUI_RadioButton(rb6)) {std::cout << "rb6" << std::endl;}

if(SDLUI_ToggleButton(tb1)) {std::cout << "tb1" << std::endl;}
if(SDLUI_Button(btn3)) {std::cout << "btn3" << std::endl;}
SDLUI_TabContainer(tbc1);

// Copy the current theme in clipboard. This can be used to create an instance of an SDLUI_Theme struct
// for a custom color theme definition.
if(SDLUI_Button(btn_copy))
{
	SDLUI_Control_SliderInt *cur;
	std::string cb =
	"SDLUI_Theme my_theme = {\n\t{";

	for (int i = 0; i < num_sliders; ++i)
	{
		cur = (SDLUI_Control_SliderInt*)color_sliders.data[i];

		cb += std::to_string(cur->value);

		if((i + 1) % 3 == 0)
		{
			if(i < num_sliders - 1)
			{
				cb += ", 255},\n\t{";
			}
			else
			{
				cb += ", 255}\n};";
			}
		}
		else
		{
			cb += ", ";
		}
	}

	SDL_SetClipboardText(cb.c_str());
}

SDLUI_Control_SliderInt *col_slider;

SDLUI_Text(txt01);
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[0];
if(SDLUI_SliderInt(col_slider)) {SDLUI_Core.theme.col_active_window_bar.r = col_slider->value; SDLUI_ForceReDraw();}
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[1];
if(SDLUI_SliderInt(col_slider)) {SDLUI_Core.theme.col_active_window_bar.g = col_slider->value; SDLUI_ForceReDraw();}
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[2];
if(SDLUI_SliderInt(col_slider)) {SDLUI_Core.theme.col_active_window_bar.b = col_slider->value; SDLUI_ForceReDraw();}

SDLUI_Text(txt02);
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[3];
if(SDLUI_SliderInt(col_slider)) {SDLUI_Core.theme.col_inactive_window_bar.r = col_slider->value; SDLUI_ForceReDraw();}
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[4];
if(SDLUI_SliderInt(col_slider)) {SDLUI_Core.theme.col_inactive_window_bar.g = col_slider->value; SDLUI_ForceReDraw();}
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[5];
if(SDLUI_SliderInt(col_slider)) {SDLUI_Core.theme.col_inactive_window_bar.b = col_slider->value; SDLUI_ForceReDraw();}

SDLUI_Text(txt03);
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[6];
if(SDLUI_SliderInt(col_slider)) {SDLUI_Core.theme.col_window_bg.r = col_slider->value; SDLUI_ForceReDraw();}
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[7];
if(SDLUI_SliderInt(col_slider)) {SDLUI_Core.theme.col_window_bg.g = col_slider->value; SDLUI_ForceReDraw();}
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[8];
if(SDLUI_SliderInt(col_slider)) {SDLUI_Core.theme.col_window_bg.b = col_slider->value; SDLUI_ForceReDraw();}

SDLUI_Text(txt04);
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[9];
if(SDLUI_SliderInt(col_slider)) {SDLUI_Core.theme.col_highlight.r = col_slider->value; SDLUI_ForceReDraw();}
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[10];
if(SDLUI_SliderInt(col_slider)) {SDLUI_Core.theme.col_highlight.g = col_slider->value; SDLUI_ForceReDraw();}
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[11];
if(SDLUI_SliderInt(col_slider)) {SDLUI_Core.theme.col_highlight.b = col_slider->value; SDLUI_ForceReDraw();}


// TODO: Make a tiny calc app

SDLUI_Window(calc);

SDLUI_Text(display);

if(SDLUI_Button(calc_btnc)) {;}

if(SDLUI_Button(calc_btn7)) {;}
if(SDLUI_Button(calc_btn8)) {;}
if(SDLUI_Button(calc_btn9)) {;}
if(SDLUI_Button(calc_btndiv)) {;}

if(SDLUI_Button(calc_btn4)) {;}
if(SDLUI_Button(calc_btn5)) {;}
if(SDLUI_Button(calc_btn6)) {;}
if(SDLUI_Button(calc_btnmul)) {;}

if(SDLUI_Button(calc_btn1)) {;}
if(SDLUI_Button(calc_btn2)) {;}
if(SDLUI_Button(calc_btn3)) {;}
if(SDLUI_Button(calc_btnsub)) {;}

if(SDLUI_Button(calc_btn0)) {;}
if(SDLUI_Button(calc_btndot)) {;}
if(SDLUI_Button(calc_btneq)) {;}
if(SDLUI_Button(calc_btnadd)) {;}

