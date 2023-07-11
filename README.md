# TPE-PI
a project by  *smdevesa* (Santiago Manuel Devesa) & *jrambau* (Junior Rambau).

## Prerrequisites

Must install before using this program:

  1. Make
  2. GCC

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
trips between all stations except the cases when there are no trips between stations.
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
  12. D (total trips started in December)
  13. Station (name of the station)

## Installation and use

#### - New York program:

##### - Compiling:
  1. Use makefile:
     `make nyc`

##### - Executing:

1. Get into nyc directory:
   `cd nyc`

2. Execute:
   `./nyc/bikeSharingNYC.out BIKESFILENAME.csv STATIONSFILENAME.csv`

    Replace BIKESFILENAME.csv and STATIONSFILENAME.csv with the path of .csv files with the data.

The bikes.csv must have the following format:
`started_at;start_station_id;ended_at;end_station_id;rideable_type;member_casual`

- started_at = date and hour of the start of the rent in format yyyy-MM-dd HH:mm:ss (string)
- start_station_id = id of the station where the rent started (integer)
- ended_at = date and hour of the end of the rent in format yyyy-MM-dd HH:mm:ss (string)
- end_station_id = id of the station where the rent ended (integer)
- rideable_type = type of bike of the rent (string)
- member_casual = "member" if the user was a member, "casual" if the user was a casual (string)

The stations.csv must have the following format:
`station_name;latitude;longitude;id`

- station_name = name of the station (string)
- latitude = latitude of the ubication of the station (double)
- longitude = longitude of the ubication of the station (double)
- id = id of the station (integer)

#### - Montreal program:

##### - Compiling:
  1. Use makefile:
      `make mon`

##### - Executing:

  1. Get into mon directory:
     `cd mon`
  
  2. Execute:
     `./nyc/bikeSharingMON.out BIKESFILENAME.csv STATIONSFILENAME.csv`

  Replace BIKESFILENAME.csv and STATIONSFILENAME.csv with the path of .csv files with the data.

The bikes.csv must have the following format:
`start_date;emplacement_pk_start;end_date;emplacement_pk_end;is_member`

- start_date = date and hour of the start of the rent in format yyyy-MM-dd HH:mm:ss (string)
- emplacement_pk_start = id of the station where the rent started (integer)
- end_date = date and hour of the end of the rent in format yyyy-MM-dd HH:mm:ss (string)
- emplacement_pk_end = id of the station where the rent ended (integer)
- is_member = 1 if the user was a member, 0 if the user was a casual (integer)

The stations.csv must have the following format:
`pk;name;latitude;longitude`

- pk = id of the station (integer)
- name = name of the station (string)
- latitude = latitude of the ubication of the station (double)
- longitude = longitude of the ubication of the station (double)

#### - Observations:

If you want to use both programs you can compile them together with Makefile with:
`make all`

## Files and directories

### /TPE-PI/

#### - Makefile

This file is used to generate executables for NYC and MON programs.

### /TPE-PI/adt/

#### - stationsADT.h

This ADT contract shows the functions used to create a stations of bicycles
and add the rides from one station to another one.
Also, it shows the struct and functions used for each of the queries created.

#### - stationsADT.c

Source code with structure of stationsADT, auxiliar and main functions.

### /TPE-PI/lib/

#### - frontLib.h

This header shows frontend functions for bikeSharingMON.c and bikeSharingNYC.c.
Also it includes all the constants for each one of the fields of the output files.

#### - frontLib.c

Source code of frontLib functions.

#### - htmlTable.h

This header shows backend functions that create the .html output files
for queries.

#### - htmlTable.c

Source code for htmlTable library.

### /TPE-PI/mon/

#### - bikeSharingMON.c

This code creates the .csv and .html files for each query for 
the city of Montreal, Canada.

### /TPE-PI/nyc/

#### - bikeSharingNYC.c

This code creates the .csv and .html files for each query for 
the city of New York, United States.

##
