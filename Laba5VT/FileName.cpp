clock_t CPP_(unsigned char* buffer, unsigned width, unsigned height)
{
	clock_t time;
	time = clock();
	short val;
	unsigned char dif;
	unsigned int y;
	dif = 0;

	// Изменил начальное значение j и шаг цикла
	for (int j = height - 2; j >= 0; j -= 2) {
		y = (j * width) * 4 + 1;

		for (int i = 0; i < width; y = y + 4, i++) {

			buffer[(j * width + i) * 4 + 0] += 40;
			buffer[(j * width + i) * 4 + 1] += 40;
		}
	}

	time = clock() - time;
	return time;
}