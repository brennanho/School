def mapper (key, val):
	s = []

	try:
		MPI_range = float(val['subnational'][0]['MPI National'])
		if 0 <= MPI_range and MPI_range < 0.1:
			MPI_range = 0
		elif 0.1 <= MPI_range and MPI_range < 0.2:
			MPI_range = 0.1
		elif 0.2 <= MPI_range and MPI_range < 0.3:
			MPI_range = 0.2
		elif 0.3 <= MPI_range and MPI_range < 0.4:
			MPI_range = 0.3
		elif 0.4 <= MPI_range and MPI_range < 0.5:
			MPI_range = 0.4
		elif 0.5 <= MPI_range and MPI_range < 0.6:
			MPI_range = 0.5
		elif 0.6 <= MPI_range and MPI_range < 0.7:
			MPI_range = 0.6
		elif 0.7 <= MPI_range and MPI_range < 0.8:
			MPI_range = 0.7
		elif 0.8 <= MPI_range and MPI_range < 0.9:
			MPI_range = 0.8
		elif 0.9 <= MPI_range and MPI_range < 1:
			MPI_range = 0.9 
		s.append((str(MPI_range), 1))
	except:
		pass

	return s
