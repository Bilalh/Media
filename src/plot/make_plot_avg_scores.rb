#!/usr/bin/env ruby -wKU

DB = File.expand_path "/Users/bilalh/Library/Application Support/Media/Media.db"
SQL = <<-SQL
	Select Score, count(Score) as Count
	From AllSeries
	Where Score is NOT NULL
	GROUP by Score
SQL
puts `sqlite3 "#{DB}" '#{SQL};' | sed 's/|/ /'`
