#!/usr/bin/env ruby -wKU

DB = File.expand_path "~/Library/Application Support/Media/Media.db"
SQL = <<-SQL
	Select d.Type, count(d.Type) as Count
	from AllSeries a 
	Join MalData d  on a.Id = d.Id
	group by d.Type
SQL
puts `sqlite3 "#{DB}" '#{SQL};' | sed 's/|/ /'`
