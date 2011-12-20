#!/usr/bin/env ruby19 -wKU
require "yaml"

DB = File.expand_path "~/Library/Application Support/Media/Media.db"
SQL = <<-SQL
	Select * from TypesByYear
	Order by Year
SQL
data = `sqlite3 "#{DB}" '#{SQL}'`.split "\n"
data.map! do |e|
	e.split '|'
end

# open('_typesByYear.yaml', 'w') { |f| YAML.dump(data, f) }
# data={}
# open('_typesByYear.yaml', 'r') { |f| data = YAML.load(f) }

all     = (data[0][0]..data[-1][0]).to_a
missing = all - data.map { |e| e[0] } 
 
missing.each do |e|
	data << [e] + [0] *6 
end

data.sort!

puts data[0][0]..data[-1][0]

puts %w{Movie Music ONA OVA Special TV}.join "\t"
puts data.map { |e|  e[1..-1].join "\t" }.join "\n"