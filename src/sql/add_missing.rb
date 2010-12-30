#!/usr/bin/env ruby
require "rubygems"
require "libXML"

include LibXML

doc = XML::Parser.file("/Users/bilalh/Downloads/animelist_1293680139_-_68221.xml").parse

doc.find("//anime[my_status='Completed' ]").each do |an|
	
	title  =  an.find('series_title/text()')[0].content
		result = `sqlite3 "/Users/bilalh/Library/Application Support/Media/Media.db" "Select EXISTS (SELECT * FROM SeriesInfo WHERE Title = '#{title.gsub /'/, "''"}')"`
		
		result2 =  `sqlite3 "/Users/bilalh/Library/Application Support/Media/Media.db" "Select EXISTS (SELECT * FROM OldSeries WHERE Title = '#{title.gsub /'/, "''"}')"`
		
		if result =~ /0/ and result2 =~ /0/ then
			total = an.find('series_episodes/text()')[0].content
			printf "%-60s %s  \n", title, total
			puts `sqlite3 "/Users/bilalh/Library/Application Support/Media/Media.db" "Insert into OldSeries(Title,Total) Values('#{title.gsub /'/, "''"}', #{total}) "`
		end
	
end