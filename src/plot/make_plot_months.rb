#!/usr/bin/env ruby -wKU

def addEp(years, yearA, monthA, dayA)
	
	years[yearA] = Hash.new unless years[yearA]
	year = years[yearA]
	
	year[monthA] = Hash.new unless year[monthA]
	month = year[monthA]
	
	month[dayA]  = 0 unless month[dayA]
	month[dayA] += 1
	
end


rows = `sqlite3 "/Users/bilalh/Library/Application Support/Media/Media.db" "Select Date From History"`.split "\n"

years = {}

rows.each do |row|
	match = row.match(/^(\d+)-(\d+)-(\d+)/)
	addEp(years,*match[1..3])
end


arr = []
ans = []
years.each do |y, year|
	year.each do |m, month|
		total = 0 
		month.each { |d, day| total += day }
		ans << sprintf("%2s %4d\n", m, total)
		 # printf("%s %2s %4d\n",y, m, total)
	end
end
puts ans


