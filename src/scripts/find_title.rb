#!/usr/bin/env ruby19 -wKU
# Bilal Syed Hussain
require "sqlite3"

(puts "#{File.basename $0} synonym"; exit) if ARGV.length != 1
DB = SQLite3::Database.new( File.expand_path"~/Library/Application Support/Media/Media.db" )

Query = <<-SQL
Select a.Title, a.Id, s.Synonym
From AllSeries a join MalSynonyms s on a.Id = s.SeriesId
where s.Synonym = ?
SQL

DB.results_as_hash = true;
rows =  DB.execute Query, ARGV[0]

(puts "No Results Found"; exit) if rows.length == 0

rows.each do |row|
	puts "#{row["Synonym"]} => #{row["Title"]} (id:#{row["Id"]})"
end