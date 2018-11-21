def mapper (key, val):
	s = []
	try:
		s.append((val['Country'], [val['MPI Urban'], val['MPI Rural'], val['subnational'][0]['MPI National']]))
	except:
		pass

	return s
