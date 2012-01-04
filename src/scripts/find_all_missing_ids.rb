#!/usr/bin/env ruby19 -wKU
# Bilal Syed Hussain
require "sqlite3"
require_relative 'find_title_and_id.rb'

DB = SQLite3::Database.new( File.expand_path"~/Library/Application Support/Media/Media.db" )

Query = <<-SQL
Select si.Title
From SeriesInfo si 
where si.Total ISNULL or (si.ID isNULL and si.Finished = 0)
SQL

rows =  DB.execute Query

rows.each do |row|
	puts "\n*** #{row[0]} ***\n"
	find_name_and_id row[0]
end