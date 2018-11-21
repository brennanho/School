def reducer (key, list_of_vals):
	mpis = [float(list_of_vals[i][1]) for i in range(len(list_of_vals))]
	return [(key, sum(mpis)/len(mpis))]