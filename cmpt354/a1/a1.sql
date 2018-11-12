CREATE TABLE MPI_national (
	ISO text PRIMARY KEY,
	country text,
	MPI_urban real CHECK(MPI_urban >=0 AND MPI_urban <= 1),
	headcount_ratio_urban real CHECK(headcount_ratio_urban >= 0 AND headcount_ratio_urban <= 100),
	intensity_deprivation_urban real CHECK(intensity_deprivation_urban >= 0 AND intensity_deprivation_urban <= 100),
	MPI_rural real CHECk(MPI_rural >=0 AND MPI_rural <= 1),
	headcount_ratio_rural real CHECK(headcount_ratio_rural >= 0 AND headcount_ratio_rural <= 100),
	intensity_deprivation_rural real CHECK(intensity_deprivation_urban >= 0 AND intensity_deprivation_rural <= 100)
);

CREATE TABLE MPI_subnational (
	ISO text,
	country text,
	subnational_region text,
	world_region text CHECK(world_region == "Sub-Saharan Africa"
		OR world_region == "Latin America and Caribbean"
		OR world_region == "East Asia and the Pacific"
		OR world_region == "Arab States"
		OR world_region == "South Asia"
		OR world_region == "Europe and Central Asia"),
	MPI_national real CHECK(MPI_national >= 0 AND MPI_national <= 1),
	MPI_regional real CHECK(MPI_regional >= 0 AND MPI_regional <= 1),
	headcount_ratio_regional real CHECK(headcount_ratio_regional >= 0 AND headcount_ratio_regional <= 100),
	intensity_deprivation_regional real CHECK(intensity_deprivation_regional >= 0 AND intensity_deprivation_regional <= 100),
	PRIMARY KEY (ISO, subnational_region)
);