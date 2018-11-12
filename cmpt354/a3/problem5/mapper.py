def mapper (key, val):
	s = []
	try:
		for region in val['subnational']:
			range_HRU = 0
			range_IDU = 0
			HRU = float(region['Headcount Ratio Regional'])
			IPU = float(region['Intensity of deprivation Regional'])
			for i in range(10):
				if i*10 <= HRU and HRU < (i+1)*10:
					range_HRU = i*10
				if i*10 <= IPU and IPU < (i+1)*10:
					range_IPU = i*10

			s.append(("H:" + str(range_HRU), [val['Country'], region['Sub-national region'], region['Headcount Ratio Regional']]))
			s.append(("I:" + str(range_IPU), [val['Country'], region['Sub-national region'], region['Intensity of deprivation Regional']]))
	except:
		pass

	return s
