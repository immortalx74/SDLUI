enum op{op_none, op_digit, op_eq, op_add, op_sub, op_mul, op_div, op_dot};
static op last_op;
static bool dot_was_set;
static i32 fp_digits;
static double old_result;

void insert_digit(i32 d, SDLUI_String &disp, double &result)
{
	if(disp.length > 18)
	{
		return;
	}

	// if(last_op != op::op_digit)
	// {
	// 	old_result = result;
	// 	result = 0;
	// 	dot_was_set = false;
	// }

	if(!dot_was_set)
	{
		if(result == 0)
		{
			result = d;
		}
		else
		{
			result = (result * 10) + d;
		}

		char back[50];
		sprintf(back, "%.0lf", result);
		disp.modify(back);
	}
	else
	{
		double fractpart = 0, intpart;

		if(fractpart == 0)
		{
			fractpart = d;
		}
		else
		{
			fractpart = modf(result, &intpart);
		}

		fp_digits++;
		result = result + (fractpart * (1 / pow(10, fp_digits)));
		char back[50];
		sprintf(back, "%.*f", fp_digits, result);
		disp.modify(back);
	}

	last_op = op::op_digit;
}

double  do_calculation(double num1, double num2)
{
	switch(last_op)
	{
		case op::op_add:
		{
			return	num1 + num2;
		}
		break;

		case op::op_sub:
		{

		}
		break;

		case op::op_mul:
		{

		}
		break;
	}
}

void calculator(i32 operation, i32 digit, double &result, SDLUI_String &disp)
{
	switch(operation)
	{
		case op_digit:
		{
			insert_digit(digit, disp, result);
		}
		break;

		case op_dot:
		{
			if(!dot_was_set)
			{
				std::string cur = disp.data;
				cur += ".";
				disp.modify(cur.c_str());
				dot_was_set = true;
			}
		}
		break;

		case op_add:
		{
			std::cout << result << std::endl;
			old_result = result;
			result = 0;
			// if(old_result != 0)
			// {
				result = do_calculation(old_result, result);

				char back[50];
				sprintf(back, "%.*f", fp_digits, result);
				disp.modify(back);
				//old_result = result;
				result = 0;
			// }
			//last_op = op::op_add;
		}
		break;

		case op_sub:
		{
			// last_op = op::op_sub;
			// if(old_result != 0)
			// {
			// 	result = old_result - result;
			// 	char back[50];
			// 	sprintf(back, "%.*f", fp_digits, result);
			// 	disp.modify(back);
			// }
		}
		break;
	}
}