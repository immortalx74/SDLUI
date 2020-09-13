void insert_digit(i32 d, SDLUI_String &disp)
{
	if(disp.length > 18)
	{
		return;
	}
	double num = atof(disp.data) * 10;
	num += d;
	char back[100];
	sprintf(back, "%.3lf", num);
	disp.modify(back);
}

void calculator(i32 operation, i32 digit, double &result, SDLUI_String &disp)
{
	enum op{op_digit, op_eq, op_add, op_sub, op_mul, op_div};

	switch(operation)
	{
		case op_digit:
		{
			insert_digit(digit, disp);
		}
		break;
	}
}