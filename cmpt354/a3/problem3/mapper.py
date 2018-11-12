def mapper (key, val):
	s = []
	try:
		range_HRU = 0
		range_IDU = 0
		HRU = float(val['Headcount Ratio Urban'])
		IPU = float(val['Intensity of Deprivation Urban'])
		for i in range(10):
			if i*10 <= HRU and HRU < (i+1)*10:
				range_HRU = i*10
			if i*10 <= IPU and IPU < (i+1)*10:
				range_IPU = i*10

		s.append(("H:" + str(range_HRU), [val['Country'], val['Headcount Ratio Urban']]))
		s.append(("I:" + str(range_IPU), [val['Country'], val['Intensity of Deprivation Urban']]))
	except:
		pass

	return s
