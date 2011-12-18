#!/usr/bin/env ruby -wKU

DB = File.expand_path "/Users/bilalh/Library/Application Support/Media/Media.db"
SQL = <<-SQL
Select si.Total, si.Score From SeriesData si 
Where si.Score IS NOT NULL And si.Score > 0 And si.Total IS NOT NULL 
Order by si.Total
SQL

puts `sqlite3 "#{DB}" '#{SQL};' | sed 's/|/ /'`
