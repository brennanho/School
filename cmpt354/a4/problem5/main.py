import sys
import csv
import mapper
import reducer
from collections import defaultdict



d = {}





csvfile = open('data/MPI_national.csv', 'r')
fieldnames = ("ISO", "Country", "MPI Urban", "Headcount Ratio Urban", "Intensity of Deprivation Urban", "MPI Rural", "Headcount Ratio Rural", "Intensity of Deprivation Rural")

next(csvfile)
reader = csv.DictReader(csvfile, fieldnames)

for row in reader:
	d[row['ISO']] = row


csvfile2 = open('data/MPI_subnational.csv', 'r')
fieldnames2 = ("ISO country code", "Country", "Sub-national region", "World region", "MPI National", "MPI Regional", "Headcount Ratio Regional", "Intensity of deprivation Regional")

next(csvfile2)
reader2 = csv.DictReader( csvfile2, fieldnames2)
for row in reader2:
	
	try:
		d[row['ISO country code']]['subnational'].append(row)
	except:
		d[row['ISO country code']]['subnational'] = [row]


t = []
for k in d:
	t.extend(mapper.mapper(k, d[k]))
	

res = defaultdict(list)
for x in t:
	res[x[0]].append(x[1])

for x in res:
	for y in reducer.reducer(x, res[x]):
		print (y)








