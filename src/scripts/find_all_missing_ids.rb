#!/usr/bin/env ruby19 -wKU
# Bilal Syed Hussain
require "sqlite3"
require_relative 'find_title_and_id.rb'

n  = (n = ARGV[0].to_f) > 0 && n < 100 ? n : 60.0
n /= 100

DB = SQLite3::Database.new( File.expand_path"~/Library/Application Support/Media/Media.db" )

Query = <<-SQL
Select si.Title
From SeriesInfo si 
where si.IsAnime = 1 and  ( (si.Total ISNULL  and (si.Dropped = 0 or si.ID isNULL) ) or (si.ID isNULL and si.Dropped = 0) )
SQL

rows =  DB.execute Query


rows.each do |row|
	puts "\n*** #{row[0]} ***\n"
	find_name_and_id row[0], true,true, n
end