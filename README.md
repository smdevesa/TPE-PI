# TPE-PI
a project by  *smdevesa* (Santiago Manuel Devesa) & *jrambau* (Junior Rambau).

## Description

This program is designed for reading .csv files with data about bike sharing stations and the rides they host, then
it does the three following queries using the data:

####  -  Query 1: Total trips started by members for every station.

This query shows total trips started by members for every station sorted by
amount of trips in descending order.
The output is a .csv file and a .html file. Both have the information of the query in
a table with the next two fields:

  1. Station (station name)
  2. StartedTrips (amount of trips started by members in the station)
     
####  - Query 2: Total trips between all stations.

This query shows total non-circular (starting station is different to ending station)
trips between all stations.
The output is a .csv file and a .html file. Both have the information of the query in
a table with the next four fields:

  1. StationA (name of station A)
  2. StationB (name of station B)
  3. Trips A->B (amount of trips started at A and finished at B)
  4. Trips B->A (amount of trips started at B and finished at A)
  
####  - Query 3: Total trips per month for every station.

This query shows total trips per month for every station in the file. If there aren't trips
in a month it will show zero.
The output is a .csv file and a .html file. Both have the information of the query in
a table with the next thirteen fields:

  1. J (total trips started in January)
  2. F (total trips started in Febraury)
  3. M (total trips started in March)
  4. A (total trips started in April)
  5. M (total trips started in May)
  6. J (total trips started in June)
  7. J (total trips started in July)
  8. A (total trips started in August)
  9. S (total trips started in September)
  10. O (total trips started in October)
  11. N (total trips started in November)
  12.
  13.

## Installation and use

##### - New York program:

- Compiling:
  `make nyc/bikeSharingNYC.out`

- Executing:
  `./nyc/bikeSharingNYC.out BIKESFILENAME.csv STATIONSFILENAME.csv`

##### - Montreal program:

- Compiling:
  `make mon/bikeSharingMON.out`

- Executing:
  `./mon/bikeSharingMON.out BIKESFILENAME.csv STATIONSFILENAME.csv`
