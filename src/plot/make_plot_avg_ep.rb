#!/usr/bin/env ruby -wKU

DB = File.expand_path "~/Library/Application Support/Media/Media.db"
SQL = <<-SQL
	Select Total, count(Total) as Count
	From AllSeries
	Where Total is NOT NULL
	GROUP by Total
SQL
puts `sqlite3 "#{DB}" '#{SQL};' | sed 's/|/ /'`
