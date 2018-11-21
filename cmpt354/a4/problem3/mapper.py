def mapper (key, val):
	s = []
	try:
		s.append((val['subnational'][0]['World region'], [val['Country'], val['subnational'][0]['MPI National']]))
	except:
		pass

	return s
