#!/usr/bin/env ruby19 -wKU
require "yaml"

DB = File.expand_path "~/Library/Application Support/Media/Mal.db"
SQL = <<-SQL
	Select * from ScoresByYear
	Order by Year
SQL
data = `sqlite3 "#{DB}" '#{SQL}'`.split "\n"
data.map! do |e|
	e.split '|'
end

# open('_scoresByYear.yaml', 'w') { |f| YAML.dump(data, f) }
# data={}
# open('_scoresByYear.yaml', 'r') { |f| data = YAML.load(f) }

all     = (data[0][0]..data[-1][0]).to_a
missing = all - data.map { |e| e[0] } 
 
missing.each do |e|
	data << [e] + [0] *10 
end

data.sort!

# puts data[0][0]..data[-1][0]

puts (1..10).to_a.map { |e| "N#{e}" }.join "\t"
puts data.map { |e|  e[1..-1].join "\t" }.join "\n"