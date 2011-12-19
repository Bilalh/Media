#!/usr/bin/env ruby -wKU

DB = File.expand_path "/Users/bilalh/Library/Application Support/Media/Media.db"
SQL = <<-SQL
Select Total, Score From AllSeries si 
Where Score IS NOT NULL 
	And Score > 0 
	And Total IS NOT NULL 
Order by Total
SQL
puts `sqlite3 "#{DB}" '#{SQL};' | sed 's/|/ /'`
