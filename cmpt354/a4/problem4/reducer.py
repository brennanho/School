def reducer (key, list_of_vals):
	mpis_urban = [float(list_of_vals[i][1]) for i in range(len(list_of_vals))]
	mpis_rural = [float(list_of_vals[i][2]) for i in range(len(list_of_vals))]

	return [(key, sum(mpis_urban)/len(mpis_urban), sum(mpis_rural)/len(mpis_rural))]