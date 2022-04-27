# how-to-read-csv-timestamps
with c++, cmake, hhdate

Reads CSV header line, then consumes all the lines;
Converts 4th field from string representation of form 'YYYY-mm-dd HH:MM:SS' to an internal numeric double representing seconds since UNIX epoch.

## input file (CSV)
```csv
dummy1,dummy2,value1,timestamp
22,xyz,0.45,2020-01-30 12:33:01
10,abc,0.99,2020-01-30 12:33:02
```

## sample run
```
$ ./test-1 tst.csv 
CSV filename=tst.csv; number of fields=4
field number 1: name: dummy1
field number 2: name: dummy2
field number 3: name: value1
field number 4: name: timestamp
got a CSV line; 4th field (timestamp) value='2020-01-30 12:33:01'; numeric value=1580387581
got a CSV line; 4th field (timestamp) value='2020-01-30 12:33:02'; numeric value=1580387582
```
