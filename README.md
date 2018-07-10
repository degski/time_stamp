
# gettimestamp

Utility generating a time- or date-stamp. Stamps existing files or 
folders (appends a timestamp (of your choice) to the current file- or folder-name).

## Manual

      gettimestamp.exe 4.0 - (c) degski 2018

      options:
      -h [ --help ]         This help screen
      -f [ --file ] arg     File(s) or directories, to be time-stamped
      -l [ --local ]        Local Time (default UTC) (yyyymmddhhmmss)
      -u [ --utc ]          UTC (hh::mm:ss)
      -d [ --date ]         Date (dd::mm:yyyy)
      -m [ --month ]        Month (mm)
      -y [ --year ]         Year (yyyy)
      -r [ --release ]      Year and Month (yymm) for release stamping

* The -f [ --file ] option, combines with options -l through -r.
* The generated timestamps are not terminated, usefull for batch file inclusion.
