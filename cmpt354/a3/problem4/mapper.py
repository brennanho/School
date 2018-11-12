def mapper (key, val):
	s = []
	try:
		MPIs = []
		world_region = val['subnational'][0]['World region']
		for region in val['subnational']:
			MPIs.append(float(region['MPI Regional']))

		avg = float(sum(MPIs) / len(MPIs))
		s.append((world_region, avg))

	except Exception as e:
		pass

	return s
