#!/usr/bin/env ruby -wKU

DB = File.expand_path "~/Library/Application Support/Media/Media.db"
SQL = <<-SQL
	Select Year, count(Year) as Count
	From MalData
	Where Year is NOT NULL
	GROUP by Year
SQL
data = `sqlite3 "#{DB}" '#{SQL}'`.split "\n"
data.map! do |e|
	e.split '|'
end

all     = (data[0][0]..data[-1][0]).to_a
missing = all - data.map { |e| e[0] } 
 
missing.each do |e|
	data << [e, 0] 
end

data.sort!

puts data.map { |e|  e.join "\t" }.join "\n"