def reducer (key, list_of_vals):
	max_val = float(list_of_vals[0][2])
	country_val = list_of_vals[0]
	for country in list_of_vals:
		if float(country[2]) > max_val:
			max_val = float(country[2])
			country_val = country

	return [((key[0], key[2:]), (country_val[0], country_val[1], country_val[2]))] 