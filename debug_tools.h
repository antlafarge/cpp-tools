inline void setConsoleColor(uint16_t color)
{
#if defined(_MSC_VER)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#else
	(void)color;
#endif
}

std::string pad(std::string txt, std::size_t size = 9)
{
	if (txt.size() > size)
	{
		txt.resize(size);
	}
	else if (txt.size() < size)
	{
		while (txt.size() < size)
		{
			txt.push_back(' ');
		}
	}
	return txt;
}

std::string toReadableNumber(uint64_t number)
{
	std::string str = std::to_string(number);
	for (int64_t i = str.size() - 3; i > 0; i -= 3)
	{
		str.insert((size_t)i, 0, '\'');
	}
	return str;
}
