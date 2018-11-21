def mapper (key, val):
	s = []
	if float(val['MPI Urban']) >= 0.1 and float(val['MPI Rural']) >= 0.2:
		s.append((val['Country'], [val['MPI Urban'], val['MPI Rural']]))
	return s
