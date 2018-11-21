def reducer (key, list_of_vals):
	avg = sum(list_of_vals)/len(list_of_vals)
	return [(key, avg)]