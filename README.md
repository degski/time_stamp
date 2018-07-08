
# gettimestamp

Utility generating a time- (UTC is default) or a date-stamp. Stamps existing files (add timestamp to the current-name).

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

  The generated timestamps are not terminated, usefull for batch file inclusion.
