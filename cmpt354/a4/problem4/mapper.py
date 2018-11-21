def mapper (key, val):
	s = []
	try:
		s.append((val['subnational'][0]['World region'], [val['Country'], val['MPI Urban'], val['MPI Rural']]))
	except:
		pass

	return s
