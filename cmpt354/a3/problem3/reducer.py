def reducer (key, list_of_vals):
	max_MPI = float(list_of_vals[0][1])
	tuple_MPI = list_of_vals[0]
	for country in list_of_vals:
		if float(country[1]) > max_MPI:
			max_MPI = float(country[1])
			tuple_MPI = country

	return [((key[0], key[2:]), (tuple_MPI[0], tuple_MPI[1]))] 